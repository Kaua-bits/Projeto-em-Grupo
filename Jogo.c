#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bib.h"

int main(){
    
    srand((unsigned int)time(NULL));

    puts("\n---------------------------------- Jogo Perguntado ----------------------------------\n\n ----- Voce possui 01 vida, assim como 01 de cada acao especial para lhe ajudar -----\n");

    Jogo *jogo = malloc(sizeof(Jogo));
    inicia_jogo(jogo);

    Pergunta *perg = malloc(sizeof(Pergunta));
    inicia_pergunta(perg);

    Grupo_de_Perguntas *grupo = malloc(sizeof(Grupo_de_Perguntas));
    grupo->perguntas = malloc(sizeof(Pergunta*)*1);
    populaGrupo(perg, jogo, grupo);
    
    ler_pergunta(jogo,1);
    
    free_tudo(jogo, perg, grupo);
    
    return 0;

}

