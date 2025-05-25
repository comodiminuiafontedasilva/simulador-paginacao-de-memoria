#ifndef PAGINA_H
#define PAGINA_H

typedef struct {
    int presente;
    int frame;
    int modificada;
    int referenciada;
    int tempo_carga;  
    int ultimo_acesso; 
} Pagina;

#endif
