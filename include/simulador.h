#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "processo.h"
#include "memoria_fisica.h"

typedef struct
{
    int tempo_atual;
    int tamanho_pagina;
    int tamanho_memoria_fisica;
    int num_processos;
    Processo *processos;
    MemoriaFisica memoria;

    int total_acessos;
    int page_faults;
    int hits;

    int algoritmo;
} Simulador;

void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento);
int verificar_pagina_presente(Simulador *sim, int pid, int pagina);
int traduzir_endereco(Simulador *sim, int pid, int endereco_virtual);
int carregar_pagina(Simulador *sim, int pid, int pagina);
int substituir_pagina_fifo(Simulador *sim);
int acessar_memoria(Simulador *sim, int pid, int endereco_virtual);
void exibir_memoria_fisica(Simulador *sim);
int substituir_pagina_random(Simulador *sim);

#endif
