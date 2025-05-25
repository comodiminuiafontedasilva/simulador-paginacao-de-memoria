#include <stdlib.h>
#include <stdio.h>

#include "../include/simulador.h"
#include "../include/processo.h"
#include "../include/memoria_fisica.h"
#include "../include/pagina.h"

int main() {
    Simulador sim;
    sim.tamanho_pagina = 4096;
    sim.tamanho_memoria_fisica = 16384;
    sim.tempo_atual = 0;
    sim.total_acessos = 0;
    sim.page_faults = 0;
    sim.algoritmo = 0;

    // memória física (4 frames)
    sim.memoria.num_frames = sim.tamanho_memoria_fisica / sim.tamanho_pagina;
    sim.memoria.frames = calloc(sim.memoria.num_frames, sizeof(int));
    sim.memoria.tempo_carga = calloc(sim.memoria.num_frames, sizeof(int));

    // 1 processo com 2 páginas (8KB)
    sim.num_processos = 1;
    sim.processos = malloc(sizeof(Processo));
    sim.processos[0].pid = 0;
    sim.processos[0].tamanho = 8192;
    sim.processos[0].num_paginas = sim.processos[0].tamanho / sim.tamanho_pagina;
    sim.processos[0].tabela_paginas = calloc(sim.processos[0].num_paginas, sizeof(Pagina));

    // Teste
    int endereco_virtual = 6000;
    int pagina, deslocamento;
    extrair_pagina_deslocamento(&sim, endereco_virtual, &pagina, &deslocamento);

    printf("Endereço virtual: %d\n", endereco_virtual);
    printf("Página: %d, Deslocamento: %d\n", pagina, deslocamento);

    int presente = verificar_pagina_presente(&sim, 0, pagina);
    printf("Página presente? %s\n", presente ? "Sim" : "Não");

    int endereco_fisico = traduzir_endereco(&sim, 0, endereco_virtual);
    if (endereco_fisico == -1) {
        printf("Page fault detectado ao traduzir endereço!\n");
    } else {
        printf("Endereço físico traduzido: %d\n", endereco_fisico);
    }

    // Liberação de memória
    free(sim.processos[0].tabela_paginas);
    free(sim.processos);
    free(sim.memoria.frames);
    free(sim.memoria.tempo_carga);

    return 0;
}
