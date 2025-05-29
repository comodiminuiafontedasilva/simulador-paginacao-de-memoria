#include <stdio.h>
#include <stdlib.h>
#include "../include/simulador.h"
#include "../include/processo.h"
#include "../include/memoria_fisica.h"

void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento)
{
    *pagina = endereco_virtual / sim->tamanho_pagina;
    *deslocamento = endereco_virtual % sim->tamanho_pagina;
}

int verificar_pagina_presente(Simulador *sim, int pid, int pagina)
{
    Processo *proc = &sim->processos[pid];
    return proc->tabela_paginas[pagina].presente;
}

int traduzir_endereco(Simulador *sim, int pid, int endereco_virtual)
{
    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, endereco_virtual, &pagina, &deslocamento);

    Processo *proc = &sim->processos[pid];

    if (!verificar_pagina_presente(sim, pid, pagina))
    {
        sim->page_faults++;
        return -1; // Page fault
    }

    int frame = proc->tabela_paginas[pagina].frame;
    return frame * sim->tamanho_pagina + deslocamento;
}

int carregar_pagina(Simulador *sim, int pid, int pagina)
{
    // Procura por um frame livre
    for (int i = 0; i < sim->memoria.num_frames; i++)
    {
        if ((sim->memoria.frames[i] >> 16) == -1)
        { // frame vazio
            sim->memoria.frames[i] = (pid << 16) | pagina;
            sim->memoria.tempo_carga[i] = sim->tempo_atual;

            Processo *proc = &sim->processos[pid];
            proc->tabela_paginas[pagina].presente = 1;
            proc->tabela_paginas[pagina].frame = i;
            proc->tabela_paginas[pagina].tempo_carga = sim->tempo_atual;

            return i;
        }
    }

    int frame_substituido;
    switch (sim->algoritmo)
    {
    case 0:
        frame_substituido = substituir_pagina_fifo(sim);
        break;
    case 1:
        frame_substituido = substituir_pagina_random(sim);
        break;
    default:
        printf("Erro: algoritmo de substituição não implementado!\n");
        exit(1);
    }

    int pid_antigo = sim->memoria.frames[frame_substituido] >> 16;
    int pagina_antiga = sim->memoria.frames[frame_substituido] & 0xFFFF;

    sim->processos[pid_antigo].tabela_paginas[pagina_antiga].presente = 0;
    sim->processos[pid_antigo].tabela_paginas[pagina_antiga].frame = -1;

    sim->memoria.frames[frame_substituido] = (pid << 16) | pagina;
    sim->memoria.tempo_carga[frame_substituido] = sim->tempo_atual;

    sim->processos[pid].tabela_paginas[pagina].presente = 1;
    sim->processos[pid].tabela_paginas[pagina].frame = frame_substituido;
    sim->processos[pid].tabela_paginas[pagina].tempo_carga = sim->tempo_atual;

    return frame_substituido;
}

int substituir_pagina_fifo(Simulador *sim)
{
    int mais_antigo = sim->tempo_atual + 1;
    int frame_escolhido = -1;

    for (int i = 0; i < sim->memoria.num_frames; i++)
    {
        if (sim->memoria.tempo_carga[i] < mais_antigo)
        {
            mais_antigo = sim->memoria.tempo_carga[i];
            frame_escolhido = i;
        }
    }

    return frame_escolhido;
}

int acessar_memoria(Simulador *sim, int pid, int endereco_virtual)
{
    sim->total_acessos++;

    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, endereco_virtual, &pagina, &deslocamento);

    if (!verificar_pagina_presente(sim, pid, pagina))
    {
        printf("t = %d: [PAGE FAULT] Página %d do Processo %d não está na memória!\n", sim->tempo_atual, pagina, pid);
        carregar_pagina(sim, pid, pagina);
        sim->page_faults++;
    }
    else
    {
        sim->hits++;
        printf("t = %d: [HIT] Página %d do Processo %d está na memória.\n", sim->tempo_atual, pagina, pid);
    }

    int frame = sim->processos[pid].tabela_paginas[pagina].frame;
    int endereco_fisico = frame * sim->tamanho_pagina + deslocamento;

    printf("t = %d: Endereço virtual %d (Página %d) -> Endereço físico %d (Frame %d)\n",
           sim->tempo_atual, endereco_virtual, pagina, endereco_fisico, frame);

    return endereco_fisico;
}

void exibir_memoria_fisica(Simulador *sim)
{
    printf("\nEstado da Memória Física:\n");
    for (int i = 0; i < sim->memoria.num_frames; i++)
    {
        printf("--------\n");
        int valor = sim->memoria.frames[i];
        int pid = valor >> 16;
        int pagina = valor & 0xFFFF;

        if (pid == -1)
        {
            printf("| ---- |\n");
        }
        else
        {
            printf("| P%d-%d |\n", pid, pagina);
        }
    }
    printf("--------\n\n");
}

int substituir_pagina_random(Simulador *sim)
{
    int frame_escolhido = rand() % sim->memoria.num_frames;
    return frame_escolhido;
}
