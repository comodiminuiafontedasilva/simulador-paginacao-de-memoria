#include <stdlib.h>
#include "../include/simulador.h"
#include "../include/processo.h"
#include "../include/memoria_fisica.h"

void extrair_pagina_deslocamento(Simulador *sim, int endereco_virtual, int *pagina, int *deslocamento) {
    *pagina = endereco_virtual / sim->tamanho_pagina;
    *deslocamento = endereco_virtual % sim->tamanho_pagina;
}

int verificar_pagina_presente(Simulador *sim, int pid, int pagina) {
    Processo *proc = &sim->processos[pid];
    return proc->tabela_paginas[pagina].presente;
}

int traduzir_endereco(Simulador *sim, int pid, int endereco_virtual) {
    int pagina, deslocamento;
    extrair_pagina_deslocamento(sim, endereco_virtual, &pagina, &deslocamento);

    Processo *proc = &sim->processos[pid];

    if (!verificar_pagina_presente(sim, pid, pagina)) {
        sim->page_faults++;
        return -1; // Page fault
    }

    int frame = proc->tabela_paginas[pagina].frame;
    return frame * sim->tamanho_pagina + deslocamento;
}