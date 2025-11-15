#ifndef BIB_H
#define BIB_H

typedef struct{
    int perguntas_num;
    struct Pergunta **perguntas; 
} Grupo_de_Perguntas;

typedef struct Pergunta {
    char *enunciado;
    char **alternativas;
    int alternativas_num;
    int resposta;
    char *dica;
    int nivel;
} Pergunta;

typedef struct{
    int vida;
    int nivel;
    int pulo;
    int troca;
    int dica;
    Pergunta *pergunta_atual;
    Grupo_de_Perguntas *gdp;
} Jogo;

void inicia_jogo(Jogo *j);
void inicia_pergunta(Pergunta *p);
int mostraOpcoes(Jogo *j);
void ler_pergunta(Jogo *j, int i);
int mostraMenu(Jogo *j);
int populaGrupo(Pergunta *p, Jogo *j, Grupo_de_Perguntas *gdp);
void free_tudo(Jogo *j, Pergunta *p, Grupo_de_Perguntas *g);
static int calcula_max_nivel(Grupo_de_Perguntas *gdp);
int sorteia_pergunta_nivel(Grupo_de_Perguntas *gdp, int nivel);
int avanca_nivel(Jogo *j, Grupo_de_Perguntas *gdp);
int troca_pergunta_mesmo_nivel(Jogo *j, Grupo_de_Perguntas *gdp);

#endif
