#ifndef PROCESSO_H
#define PROCESSO_H

#include "pagina.h"

typedef struct {
    int pid;
    int tamanho;
    int num_paginas;
    Pagina *tabela_paginas;
} Processo;

#endif
