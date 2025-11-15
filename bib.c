#include "bib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void inicia_jogo(Jogo *j){
    j->vida = 1;
    j->nivel = 1;
    j->pulo = 0;
    j->troca = 0;
    j->dica = 0;
    j->pergunta_atual = NULL;
    j->gdp = NULL;
}

void inicia_pergunta(Pergunta *p){
    p->nivel = 1;
    p->enunciado = NULL;
    p->alternativas_num = 0;
    p->alternativas = NULL;
    p->resposta = 2;
    p->dica = NULL;
}

void ler_pergunta(Jogo *j, int i);

int mostraOpcoes(Jogo *j){
    int resp = 5; char c;
    do{
        printf("\n\n||||||||||||||||||||||| ACOES\n1) Pular\n2) Trocar\n3) Dica\n4) Voltar\n|||||||||||||||||| ESCOLHA: ");
        if (scanf("%d",&resp) != 1) {
            while(((c = getchar()) != '\n') && (c != EOF));
            resp = 5;
        } else {
            while(((c = getchar()) != '\n') && (c != EOF));
        }

        switch(resp){
            case 1: {
                if (j->pulo) {
                    printf("\nVoce ja usou o pulo.\n");
                } else {
                    if (j->gdp && avanca_nivel(j, j->gdp) == 0) {
                        j->pulo = 1;
                        printf("\nPergunta pulada. Avancando para o proximo nivel.\n");
                        if (j->pergunta_atual) ler_pergunta(j,0);
                    } else {
                        printf("\nNao ha pergunta no proximo nivel para pular.\n");
                    }
                }
                break;
            }
            case 2: {
                if (j->troca) {
                    printf("\nVoce ja usou a troca.\n");
                } else {
                    if (j->gdp && troca_pergunta_mesmo_nivel(j, j->gdp) == 0) {
                        j->troca = 1;
                        printf("\nPergunta trocada por outra do mesmo nivel.\n");
                        ler_pergunta(j,0);
                    } else {
                        printf("\nNao ha outra pergunta disponivel neste nivel para trocar.\n");
                    }
                }
                break;
            }
            case 3: {
                if (j->dica) {
                    printf("\nVoce ja usou a dica.\n");
                } else {
                    if (j->pergunta_atual && j->pergunta_atual->dica) {
                        printf("\n~~~~~~~~ DICA: %s\n", j->pergunta_atual->dica);
                        j->dica = 1;
                    } else {
                        printf("\nNenhuma dica disponivel para esta pergunta.\n");
                    }
                }
                break;
            }
            case 4: {
                puts("");
                break;
            }
            default:
                printf("\n! ! ! Opcao invalida ! ! !\n");
                resp = 5;
                break;
        }
    } while (resp > 4);
    return resp;
}

void ler_pergunta(Jogo *j, int i){
    if (!j || !j->pergunta_atual) {
        printf("\nSem pergunta atual.\n");
        return;
    }
    printf("\n||||%s\n", j->pergunta_atual->enunciado ? j->pergunta_atual->enunciado : "<sem enunciado>");
    for(int aux = 0; aux < j->pergunta_atual->alternativas_num; aux++){
        printf("%d) %s    ", aux+1, j->pergunta_atual->alternativas[aux] ? j->pergunta_atual->alternativas[aux] : "");
    }
    puts("");
    if (i == 1){
        mostraMenu(j);
    }
}

int mostraMenu(Jogo *j){
    int esc = 0; char c; int resp = 1;
    if (!j) return 0;
    while (1) {
        printf("\n|||||||||||||||||||||||| MENU\n1) Responder\n2) Sair do jogo\n3) Acao especial\n4) Estado do Jogo\n5) Mostrar pergunta novamente\n|||||||||||||||||| ESCOLHA: ");
        if (scanf("%d",&esc) != 1) {
            while(((c = getchar()) != '\n') && (c != EOF));
            esc = 0;
        } else {
            while(((c = getchar()) != '\n') && (c != EOF));
        }

        switch (esc){
            case 1: {
                ler_pergunta(j,0);
                printf("||||Resposta: ");
                if (scanf("%d",&resp) != 1) {
                    while ((c = getchar())!= '\n' && (c != EOF));
                    printf("\n! ! ! Opcao invalida ! ! !\n");
                    break;
                }
                while ((c = getchar())!= '\n' && (c != EOF));

                if(resp < 1 || resp > j->pergunta_atual->alternativas_num){
                    printf("\n! ! ! Opcao invalida ! ! !\n");
                    break;
                }

                if(resp == j->pergunta_atual->resposta){
                    printf("\nAcertou!\n");
                    int maxnivel = calcula_max_nivel(j->gdp);
                    if (j->nivel >= maxnivel) {
                        printf("\n\n---------------------------------- Voce venceu! Jogo finalizado com sucesso! ---------------------------------- \n\n");
                        return resp;
                    }
                    if (j->gdp && avanca_nivel(j, j->gdp) == 0) {
                        printf("\nAvancou para o nivel %d.\n", j->nivel);
                        ler_pergunta(j,0);
                        continue;
                    } else {
                        /* se não conseguiu avançar, mas ainda não era o último nível,
                           manter fluxo e informar ao usuário */
                        printf("Nao ha perguntas no proximo nivel. Mantendo nivel atual.\n");
                        continue;
                    }
                } else {
                    printf("Errou, perdeu uma vida.\n\nGAME OVER\n\n");
                    return resp;
                }
            }
            case 2:
                printf("\nSaindo do jogo...\n\n");
                return 0;
            case 3:
                mostraOpcoes(j);
                break;
            case 4:
                printf("\n|||||||||||||||||| ESTADO");
                printf("\nVida: %d", j->vida);
                printf("\nNivel atual: %d", j->nivel);
                printf("\nPulo usado: %d", j->pulo);
                printf("\nTroca usada: %d", j->troca);
                printf("\nDica usada: %d\n", j->dica);
                break;
            case 5:
                ler_pergunta(j,0);
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
    }
}


int populaGrupo(Pergunta *p, Jogo *j, Grupo_de_Perguntas *gdp){
    FILE *json = fopen("perguntas.json","r");
    if(json == NULL){
        printf("Erro na abertura do arquivo");
        return 1;
    }

    const int tam = 1024;
    char * leitura = malloc (tam);
    int num_perg = 0;

    while (fgets(leitura,tam,json) != NULL) {
        for(int k = 0; leitura[k] != '\0'; k++){
            if(leitura[k] == '{') num_perg++;
        }
    }
    gdp->perguntas_num = num_perg;
    gdp->perguntas = malloc(sizeof(Pergunta*) * gdp->perguntas_num);
    for (int i = 0; i < num_perg; i++) gdp->perguntas[i] = NULL;

    rewind(json);

    int num_grupo = 0;
    int i = 1;
    Pergunta *atual = NULL;

    while(fgets(leitura,tam,json) != NULL){

        char *corta_espaco = leitura;
        while(*corta_espaco == ' ' || *corta_espaco == '\t') corta_espaco++;

        int tem = 0;
        for (int k = 0; corta_espaco[k] != '\0'; k++) if (corta_espaco[k] == '{') { tem = 1; break; }
        if (tem == 1) {
            atual = malloc(sizeof(Pergunta));
            if (!atual) break;
            inicia_pergunta(atual);
            i = 1;
            continue;
        }

        char *pbuf = corta_espaco;

        switch(i){
            case 1: {
                while (*pbuf && *pbuf != ':') pbuf++;
                if (*pbuf == ':') {
                    pbuf++;
                    while (*pbuf == ' ' || *pbuf == '\t') pbuf++;
                    atual->nivel = atoi(pbuf);
                }
                break;
            }
            case 2: {
                while (*pbuf && *pbuf != ':') pbuf++;
                if (*pbuf == ':') {
                    pbuf++;
                    while (*pbuf == ' ' || *pbuf == '\t') pbuf++;
                    if (*pbuf == '\"') pbuf++;
                    char tmp_enun[1024]; int ti = 0;
                    while (*pbuf && *pbuf != '\"' && *pbuf != '\n' && ti + 1 < (int)sizeof(tmp_enun))
                        tmp_enun[ti++] = *pbuf++;
                    tmp_enun[ti] = '\0';
                    if (atual->enunciado) free(atual->enunciado);
                    atual->enunciado = malloc(strlen(tmp_enun) + 1);
                    strcpy(atual->enunciado, tmp_enun);
                }
                break;
            }
            case 3: {
                while (*pbuf && *pbuf != ':') pbuf++;
                if (*pbuf == ':') {
                    pbuf++;
                    while (*pbuf == ' ' || *pbuf == '\t') pbuf++;
                    int n = atoi(pbuf);
                    if (n > 0) {
                        if (atual->alternativas) {
                            for (int z = 0; z < atual->alternativas_num; z++) {
                                free(atual->alternativas[z]);
                            }
                            free(atual->alternativas);
                            atual->alternativas = NULL;
                        }
                        atual->alternativas_num = n;
                        atual->alternativas = malloc(sizeof(char*) * atual->alternativas_num);
                        for (int a = 0; a < atual->alternativas_num; a++) atual->alternativas[a] = NULL;
                    }
                }
                break;
            }
            case 4: {
                while (*pbuf && *pbuf != ':') pbuf++;
                if (*pbuf == ':') {
                    pbuf++;
                    while (*pbuf == ' ' || *pbuf == '\t') pbuf++;
                    if (*pbuf == '\"') pbuf++;
                    char tmp_dica[1024]; int di = 0;
                    while (*pbuf && *pbuf != '\"' && *pbuf != '\n' && di + 1 < (int)sizeof(tmp_dica))
                        tmp_dica[di++] = *pbuf++;
                    tmp_dica[di] = '\0';
                    if (atual->dica) free(atual->dica);
                    atual->dica = malloc(strlen(tmp_dica) + 1);
                    strcpy(atual->dica, tmp_dica);
                }
                break;
            }
            case 5: {
                while (*pbuf && *pbuf != ':') pbuf++;
                if (*pbuf == ':') {
                    pbuf++;
                    while (*pbuf == ' ' || *pbuf == '\t') pbuf++;
                    atual->resposta = atoi(pbuf);
                }
                break;
            }
            case 6: {
                char *arr = pbuf;
                int encontrou = 0;
                for (int k = 0; arr[k] != '\0'; k++) if (arr[k] == '[') { arr = &arr[k+1]; encontrou = 1; break; }
                if (!encontrou) {
                    while (fgets(leitura,tam,json) != NULL) {
                        arr = leitura;
                        for (int k = 0; arr[k] != '\0'; k++) if (arr[k] == '[') { arr = &arr[k+1]; encontrou = 1; break; }
                        if (encontrou) break;
                    }
                }
                if (!encontrou) break;

                int aux = 0;
                while (arr && *arr && aux < atual->alternativas_num) {
                    while (*arr == ' ' || *arr == '\t' || *arr == ',') arr++;
                    if (*arr == '\"') {
                        arr++;
                        char tmp_alt[1024];
                        int ai = 0;
                        while (*arr && *arr != '\"' && *arr != '\n' && ai + 1 < (int)sizeof(tmp_alt)) tmp_alt[ai++] = *arr++;
                        tmp_alt[ai] = '\0';

                        if (atual->alternativas[aux]) free(atual->alternativas[aux]);
                        atual->alternativas[aux] = malloc(strlen(tmp_alt) + 1);
                        strcpy(atual->alternativas[aux], tmp_alt);

                        aux++;
                        if (*arr == '\"') arr++;
                        continue;
                    } else if (*arr == ']') {
                        break;
                    } else {
                        if (*arr == '\0' || *arr == '\n') {
                            if (fgets(leitura, tam, json) == NULL) break;
                            arr = leitura;
                            continue;
                        } else break;
                    }
                }
                for (int a = 0; a < atual->alternativas_num; a++) {
                    if (!atual->alternativas[a]) {
                        atual->alternativas[a] = malloc(1);
                        atual->alternativas[a][0] = '\0';
                    }
                }
                break;
            }

        }
        i++;

        int fechou = 0;
        for (int k = 0; corta_espaco[k] != '\0'; k++) if (corta_espaco[k] == '}') { fechou = 1; break; }
        if (fechou == 1) {
            if (num_grupo < gdp->perguntas_num) {
                gdp->perguntas[num_grupo++] = atual;

                if (num_grupo == 1 && p != NULL) {
                    if (p->enunciado) { free(p->enunciado); p->enunciado = NULL; }
                    if (p->dica) { free(p->dica); p->dica = NULL; }
                    if (p->alternativas) {
                        for (int z = 0; z < p->alternativas_num; z++) if (p->alternativas[z]) free(p->alternativas[z]);
                        free(p->alternativas);
                        p->alternativas = NULL;
                    }
                    p->nivel = atual->nivel;
                    if (atual->enunciado) {
                        p->enunciado = malloc(strlen(atual->enunciado) + 1);
                        strcpy(p->enunciado, atual->enunciado);
                    } else p->enunciado = NULL;

                    p->alternativas_num = atual->alternativas_num;
                    p->alternativas = malloc(sizeof(char*) * p->alternativas_num);
                    for (int a = 0; a < p->alternativas_num; a++) {
                        p->alternativas[a] = malloc(strlen(atual->alternativas[a]) + 1);
                        strcpy(p->alternativas[a], atual->alternativas[a]);
                    }

                    p->resposta = atual->resposta;
                    if (atual->dica) {
                        p->dica = malloc(strlen(atual->dica) + 1);
                        strcpy(p->dica, atual->dica);
                    } else p->dica = NULL;
                }

                if (gdp->perguntas_num > 0 && gdp->perguntas[0] != NULL) {
                    j->pergunta_atual = gdp->perguntas[0];
                    j->gdp = gdp;
                }

            } else {

                if (atual->enunciado) free(atual->enunciado);
                if (atual->dica) free(atual->dica);
                if (atual->alternativas) {
                    for (int a = 0; a < atual->alternativas_num; a++) if (atual->alternativas[a]) free(atual->alternativas[a]);
                    free(atual->alternativas);
                }
                free(atual);
            }
            atual = NULL;
            i = 1;
        }
    }

    free(leitura);
    fclose(json);
    return 0;
}

void free_tudo(Jogo *j, Pergunta *p, Grupo_de_Perguntas *g){
    if (j) free(j);

    if (p) {
        if (p->alternativas) {
            for (int i = 0; i < p->alternativas_num; i++) {
                free(p->alternativas[i]);
            }
            free(p->alternativas);
        }
        free(p->enunciado);
        free(p->dica);
        free(p);
    }

    if (g) {
        if (g->perguntas) free(g->perguntas);
        free(g);
    }
}

static int coleta_indices_por_nivel(Grupo_de_Perguntas *gdp, int nivel, int *out_indices, int max_out) {
    if (!gdp || !gdp->perguntas || max_out <= 0) return 0;
    int cnt = 0;
    for (int i = 0; i < gdp->perguntas_num && cnt < max_out; ++i) {
        Pergunta *q = gdp->perguntas[i];
        if (!q) continue;
        if (q->nivel == nivel) out_indices[cnt++] = i;
    }
    return cnt;
}

int sorteia_pergunta_nivel(Grupo_de_Perguntas *gdp, int nivel) {
    if (!gdp || !gdp->perguntas || gdp->perguntas_num <= 0) return -1;

    int *indices = malloc(sizeof(int) * gdp->perguntas_num);
    if (!indices) return -1;
    int n = coleta_indices_por_nivel(gdp, nivel, indices, gdp->perguntas_num);
    if (n <= 0) { free(indices); return -1; }

    int escolha = indices[rand() % n];
    free(indices);
    return escolha;
}

int avanca_nivel(Jogo *j, Grupo_de_Perguntas *gdp) {
    if (!j || !gdp) return -1;

    int novo_nivel = j->nivel + 1;
    int idx = sorteia_pergunta_nivel(gdp, novo_nivel);
    if (idx < 0) return -1;

    j->nivel = novo_nivel;
    j->pergunta_atual = gdp->perguntas[idx];
    return 0;
}

static int calcula_max_nivel(Grupo_de_Perguntas *gdp) {
    if (!gdp || !gdp->perguntas || gdp->perguntas_num <= 0) return 0;
    int max = 0;
    for (int i = 0; i < gdp->perguntas_num; ++i) {
        Pergunta *q = gdp->perguntas[i];
        if (!q) continue;
        if (q->nivel > max) max = q->nivel;
    }
    return max;
}

int troca_pergunta_mesmo_nivel(Jogo *j, Grupo_de_Perguntas *gdp) {
    if (!j || !gdp || !gdp->perguntas || gdp->perguntas_num <= 0) return -1;
    if (!j->pergunta_atual) return -1;

    int nivel = j->pergunta_atual->nivel;

    int *indices = malloc(sizeof(int) * gdp->perguntas_num);
    if (!indices) return -1;
    int n = coleta_indices_por_nivel(gdp, nivel, indices, gdp->perguntas_num);
    if (n <= 0) { free(indices); return -1; }

    int atual_idx = -1;
    for (int i = 0; i < gdp->perguntas_num; ++i) {
        if (gdp->perguntas[i] == j->pergunta_atual) { atual_idx = i; break; }
    }
    if (atual_idx == -1 && j->pergunta_atual->enunciado) {
        for (int i = 0; i < gdp->perguntas_num; ++i) {
            Pergunta *q = gdp->perguntas[i];
            if (!q || !q->enunciado) continue;
            if (strcmp(q->enunciado, j->pergunta_atual->enunciado) == 0) { atual_idx = i; break; }
        }
    }

    int *candidatos = malloc(sizeof(int) * n);
    if (!candidatos) { free(indices); return -1; }
    int nc = 0;
    for (int k = 0; k < n; ++k) {
        int idx = indices[k];
        if (idx < 0 || idx >= gdp->perguntas_num) continue;
        if (idx == atual_idx) continue;
        candidatos[nc++] = idx;
    }
    free(indices);

    if (nc == 0) { free(candidatos); return -1; }

    int escolha_idx = candidatos[rand() % nc];
    free(candidatos);

    j->pergunta_atual = gdp->perguntas[escolha_idx];
    return 0;
}


