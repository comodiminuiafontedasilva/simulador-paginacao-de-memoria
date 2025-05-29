#include <stdio.h>
#include <stdlib.h>
#include "../include/simulador.h"
#include "../include/processo.h"
#include "../include/memoria_fisica.h"
#include "../include/pagina.h"

int escolher_algoritmo()
{
    int opcao = -1;

    printf("===== MENU DE SIMULACAO =====\n");
    printf("Escolha o algoritmo de substituicao de paginas:\n");
    printf("0 - FIFO\n");
    printf("1 - Random\n");
    printf("Opcao: ");

    while (1)
    {
        scanf("%d", &opcao);
        if (opcao >= 0 && opcao <= 1)
            break;
        printf("Opcao invalida. Tente novamente: ");
    }

    return opcao;
}

int main()
{
    Simulador sim;

    sim.algoritmo = escolher_algoritmo();

    sim.tamanho_pagina = 4096;
    sim.tamanho_memoria_fisica = 16384;
    sim.tempo_atual = 0;
    sim.total_acessos = 0;
    sim.page_faults = 0;

    sim.memoria.num_frames = sim.tamanho_memoria_fisica / sim.tamanho_pagina;
    sim.memoria.frames = malloc(sizeof(int) * sim.memoria.num_frames);
    sim.memoria.tempo_carga = malloc(sizeof(int) * sim.memoria.num_frames);
    for (int i = 0; i < sim.memoria.num_frames; i++)
    {
        sim.memoria.frames[i] = (-1 << 16);
        sim.memoria.tempo_carga[i] = -1;
    }

    sim.num_processos = 3;
    sim.processos = malloc(sizeof(Processo) * sim.num_processos);

    sim.processos[0].pid = 0;
    sim.processos[0].tamanho = 5 * sim.tamanho_pagina;
    sim.processos[0].num_paginas = 5;
    sim.processos[0].tabela_paginas = calloc(sim.processos[0].num_paginas, sizeof(Pagina));

    sim.processos[1].pid = 1;
    sim.processos[1].tamanho = 3 * sim.tamanho_pagina;
    sim.processos[1].num_paginas = 3;
    sim.processos[1].tabela_paginas = calloc(sim.processos[1].num_paginas, sizeof(Pagina));

    sim.processos[2].pid = 2;
    sim.processos[2].tamanho = 4 * sim.tamanho_pagina;
    sim.processos[2].num_paginas = 4;
    sim.processos[2].tabela_paginas = calloc(sim.processos[2].num_paginas, sizeof(Pagina));

    int acessos_p0[] = {1000, 5000, 9000, 13000, 17000, 1000, 5000};
    int acessos_p1[] = {0, 4096, 8192, 0, 4096};
    int acessos_p2[] = {0, 4096, 8192, 12288, 0};

    int n_acessos_p0 = sizeof(acessos_p0) / sizeof(int);
    int n_acessos_p1 = sizeof(acessos_p1) / sizeof(int);
    int n_acessos_p2 = sizeof(acessos_p2) / sizeof(int);

    int max_acessos = n_acessos_p0;
    if (n_acessos_p1 > max_acessos)
        max_acessos = n_acessos_p1;
    if (n_acessos_p2 > max_acessos)
        max_acessos = n_acessos_p2;

    for (int i = 0; i < max_acessos; i++)
    {
        if (i < n_acessos_p0)
        {
            acessar_memoria(&sim, 0, acessos_p0[i]);
            exibir_memoria_fisica(&sim);
            sim.tempo_atual++;
        }
        if (i < n_acessos_p1)
        {
            acessar_memoria(&sim, 1, acessos_p1[i]);
            exibir_memoria_fisica(&sim);
            sim.tempo_atual++;
        }
        if (i < n_acessos_p2)
        {
            acessar_memoria(&sim, 2, acessos_p2[i]);
            exibir_memoria_fisica(&sim);
            sim.tempo_atual++;
        }
    }

    printf("\n===== ESTATÍSTICAS =====\n");
    printf("Total de acessos: %d\n", sim.total_acessos);
    printf("Page faults: %d\n", sim.page_faults);
    float taxa = 100.0 * sim.page_faults / sim.total_acessos;
    printf("Taxa de page fault: %.2f%%\n", taxa);

    // Libera memória
    for (int i = 0; i < sim.num_processos; i++)
    {
        free(sim.processos[i].tabela_paginas);
    }
    free(sim.processos);
    free(sim.memoria.frames);
    free(sim.memoria.tempo_carga);

    return 0;
}
