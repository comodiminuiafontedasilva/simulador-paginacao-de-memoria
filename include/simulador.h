#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "processo.h"
#include "memoria_fisica.h"

typedef struct {
    int tempo_atual;
    int tamanho_pagina;
    int tamanho_memoria_fisica;
    int num_processos;
    Processo *processos;
    MemoriaFisica memoria;

    int total_acessos;
    int page_faults;

    int algoritmo;
} Simulador;

#endif
