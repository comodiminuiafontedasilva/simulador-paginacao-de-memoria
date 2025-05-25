#include <stdio.h>
#include <stdlib.h>
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

    sim.memoria.num_frames = sim.tamanho_memoria_fisica / sim.tamanho_pagina;
    sim.memoria.frames = malloc(sizeof(int) * sim.memoria.num_frames);
    sim.memoria.tempo_carga = malloc(sizeof(int) * sim.memoria.num_frames);

    for (int i = 0; i < sim.memoria.num_frames; i++) {
        sim.memoria.frames[i] = (-1 << 16);
        sim.memoria.tempo_carga[i] = -1;
    }

    sim.num_processos = 1;
    sim.processos = malloc(sizeof(Processo));
    sim.processos[0].pid = 0;
    sim.processos[0].tamanho = 5 * sim.tamanho_pagina;
    sim.processos[0].num_paginas = 5;
    sim.processos[0].tabela_paginas = calloc(5, sizeof(Pagina));

    for (int p = 0; p < 5; p++) {
        printf("t = %d\n", sim.tempo_atual);
        printf("Carregando página %d do processo 0\n", p);
        carregar_pagina(&sim, 0, p);

        printf("Estado dos frames: ");
        for (int i = 0; i < sim.memoria.num_frames; i++) {
            int pid = sim.memoria.frames[i] >> 16;
            int pag = sim.memoria.frames[i] & 0xFFFF;
            if (pid == -1) {
                printf("[----] ");
            } else {
                printf("[P%d-%d] ", pid, pag);
            }
        }
        printf("\n\n");

        sim.tempo_atual++;
    }

    // Liberação de memória
    free(sim.memoria.frames);
    free(sim.memoria.tempo_carga);
    free(sim.processos[0].tabela_paginas);
    free(sim.processos);

    return 0;
}
