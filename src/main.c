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

    // gerar page faults
    int acessos[] = {1000, 5000, 9000, 13000, 17000, 1000, 5000};
    int n_acessos = sizeof(acessos) / sizeof(acessos[0]);

    for (int i = 0; i < n_acessos; i++) {
        acessar_memoria(&sim, 0, acessos[i]);
        sim.tempo_atual++;
    }

    printf("\n===== ESTATÍSTICAS =====\n");
    printf("Total de acessos: %d\n", sim.total_acessos);
    printf("Page faults: %d\n", sim.page_faults);
    float taxa = 100.0 * sim.page_faults / sim.total_acessos;
    printf("Taxa de page fault: %.2f%%\n", taxa);

    // Liberação
    free(sim.memoria.frames);
    free(sim.memoria.tempo_carga);
    free(sim.processos[0].tabela_paginas);
    free(sim.processos);

    return 0;
}
