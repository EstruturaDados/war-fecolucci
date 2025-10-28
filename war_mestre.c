// DESAFIO: SISTEMA DE CADASTRO DE TERRITÓRIOS - JOGO WAR
// Nível: Aventureiro (com Missão Estratégica - nível mestre)
// Objetivo: acrescentar sistema de missão única do jogador + menu de ações

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes globais
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_QUANTIDADE 5
#define TAM_MISSAO 200

// Estrutura que representa território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Protótipos das funções
void limparBufferEntrada(void);
void tirarQuebraLinha(char *s);
void toLowerCase(char *str);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMissao(const char* missao);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao);
int exibirMenuAcoes(void);

// Função principal (main)
int main() {
    srand((unsigned int)time(NULL));

    Territorio* territorios = (Territorio*)calloc(MAX_QUANTIDADE, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Falha ao alocar memória.\n");
        return 1;
    }

    char* missoes[] = {
        "Dominar todos os territorios (apenas uma cor com tropas > 0)",
        "Eliminar todas as tropas da cor Vermelho",
        "Eliminar todas as tropas da cor Azul",
        "Haver pelo menos 3 territorios com tropas = 0",
        "Reduzir qualquer territorio a 0 tropas"
    };
    int totalMissoes = (int)(sizeof(missoes) / sizeof(missoes[0]));

    char* missao = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missao == NULL) {
        printf("Falha ao alocar memoria para a missao.\n");
        liberarMemoria(territorios, NULL);
        return 1;
    }

    cadastrarTerritorios(territorios, MAX_QUANTIDADE);
    atribuirMissao(missao, missoes, totalMissoes);
    exibirMissao(missao);

    int houveBatalha = 0;
    int acabou = 0;

    while (!acabou) {
        int acao = exibirMenuAcoes();

        if (acao == 0) {
            printf("\nSaindo do jogo...\n");
            break;
        } else if (acao == 2) {
            if (!houveBatalha) {
                printf("\nStatus da Missao: AINDA NAO CUMPRIDA (aguarde a primeira batalha)\n\n");
                continue;
            } else {
                int ok = verificarMissao(missao, territorios, MAX_QUANTIDADE);
                printf("\nStatus da Missao: %s\n\n", ok ? "CUMPRIDA" : "AINDA NAO CUMPRIDA");
                continue;
            }
        }

        exibirTerritorios(territorios, MAX_QUANTIDADE);

        int idxAtacante, idxDefensor;
        printf("Escolha o território ATACANTE (1 a %d, ou 0 para sair): ", MAX_QUANTIDADE);
        scanf("%d", &idxAtacante);
        limparBufferEntrada();
        if (idxAtacante == 0) {
            printf("\nSaindo do jogo...\n");
            break;
        }

        printf("Escolha o território DEFENSOR (1 a %d, ou 0 para sair): ", MAX_QUANTIDADE);
        scanf("%d", &idxDefensor);
        limparBufferEntrada();
        if (idxDefensor == 0) {
            printf("\nSaindo do jogo...\n");
            break;
        }

        if (idxAtacante < 1 || idxAtacante > MAX_QUANTIDADE ||
            idxDefensor  < 1 || idxDefensor  > MAX_QUANTIDADE) {
            printf("Índice(s) inválido(s). Encerrando.\n");
            break;
        }

        Territorio* atacante = &territorios[idxAtacante - 1];
        Territorio* defensor  = &territorios[idxDefensor  - 1];

        if (strncmp(atacante->cor, defensor->cor, MAX_COR) == 0) {
            printf("\nOperação inválida: não é permitido atacar um território da mesma cor (mesmo exército).\n");
            continue;
        }

        if (atacante->tropas <= 0 || defensor->tropas <= 0) {
            printf("\nUm território sem tropas não pode participar da batalha!\n");
            printf("Atacante: %s (tropas: %d)\n", atacante->nome, atacante->tropas);
            printf("Defensor: %s (tropas: %d)\n", defensor->nome, defensor->tropas);
            printf("Escolha novamente territórios válidos.\n\n");
            continue;
        }

        atacar(atacante, defensor);
        houveBatalha = 1;

        printf("\nPressione ENTER para continuar para o próximo turno...");
        getchar();

        if (verificarMissao(missao, territorios, MAX_QUANTIDADE)) {
            printf("\n====================================\n");
            printf("MISSAO CUMPRIDA!\n");
            printf("Objetivo: %s\n", missao);
            printf("====================================\n");
            break;
        }

        int vivos = 0;
        char ultimoNome[MAX_NOME] = "";
        char ultimaCor[MAX_COR] = "";

        for (int i = 0; i < MAX_QUANTIDADE; i++) {
            if (territorios[i].tropas > 0) {
                vivos++;
                strncpy(ultimoNome, territorios[i].nome, MAX_NOME - 1);
                ultimoNome[MAX_NOME - 1] = '\0';
                strncpy(ultimaCor, territorios[i].cor, MAX_COR - 1);
                ultimaCor[MAX_COR - 1] = '\0';
            }
        }

        if (vivos == 1) {
            printf("\n====================================\n");
            printf("FIM DE JOGO! O território %s de exército %s dominou todos os territórios!\n",
                ultimoNome, ultimaCor);
            printf("====================================\n");
            break;
        }
    }

    liberarMemoria(territorios, missao);
    return 0;
}

// Funções auxiliares

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void tirarQuebraLinha(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

void cadastrarTerritorios(Territorio* mapa, int n) {
    printf("\n====================================\n  WAR ESTRUTURADO - CADASTRO INICIAL\n====================================\n");

    for (int totalTerritorios = 0; totalTerritorios < n; totalTerritorios++) {
        printf("\n--- Cadastro de Território %d ---\n", totalTerritorios + 1);
        printf("Digite o nome do território: ");
        fgets(mapa[totalTerritorios].nome, MAX_NOME, stdin);
        tirarQuebraLinha(mapa[totalTerritorios].nome);

        printf("Digite a cor do exército: ");
        fgets(mapa[totalTerritorios].cor, MAX_COR, stdin);
        tirarQuebraLinha(mapa[totalTerritorios].cor);
        toLowerCase(mapa[totalTerritorios].cor);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[totalTerritorios].tropas);
        limparBufferEntrada();
    }
}

void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n===== MAPA DO MUNDO - ESTADO ATUAL =====\n");
    for (int totalTerritorios = 0; totalTerritorios < n; totalTerritorios++) {
        printf("%d. %s (Exército %s, Tropas: %d)\n",
            totalTerritorios + 1,
            mapa[totalTerritorios].nome,
            mapa[totalTerritorios].cor,
            mapa[totalTerritorios].tropas);
    }
    printf("\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O Atacante da %s com exército %s rolou um dado e tirou %d\n",
           atacante->nome, atacante->cor, dadoAtacante);
    printf("Defensor da %s com exército %s rolou um dado e tirou %d\n",
           defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        if (defensor->tropas > 0) defensor->tropas--;
        printf("ATACANTE VENCEU A RODADA! Defensor perdeu 1 tropa.\n");
    } else if (dadoDefensor > dadoAtacante) {
        if (atacante->tropas > 0) atacante->tropas--;
        printf("DEFENSOR VENCEU A RODADA! Atacante perdeu 1 tropa.\n");
    } else {
        printf("\nEMPATE! Nenhuma tropa perdida.\n");
    }

    if (defensor->tropas == 0) {
        strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
        defensor->cor[MAX_COR - 1] = '\0';
        printf("\nCONQUISTA! O território %s foi dominado pelo exército %s\n",
               defensor->nome, atacante->cor);
    } else if (atacante->tropas == 0) {
        strncpy(atacante->cor, defensor->cor, MAX_COR - 1);
        atacante->cor[MAX_COR - 1] = '\0';
        printf("\nCONQUISTA! O território %s foi dominado pelo exército %s\n",
               atacante->nome, defensor->cor);
    }
}

void exibirMissao(const char* missao) {
    printf("\n--- SUA MISSAO ---\n");
    printf("%s\n\n", missao);
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strstr(missao, "Dominar todos os territorios") != NULL) {
        char corDominante[MAX_COR] = "";
        int encontrouCor = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 0) {
                if (!encontrouCor) {
                    strncpy(corDominante, mapa[i].cor, MAX_COR - 1);
                    corDominante[MAX_COR - 1] = '\0';
                    encontrouCor = 1;
                } else if (strncmp(corDominante, mapa[i].cor, MAX_COR) != 0) {
                    return 0;
                }
            }
        }
        return encontrouCor ? 1 : 0;
    }

    if (strstr(missao, "cor Vermelho") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            char corTemp[MAX_COR];
            strncpy(corTemp, mapa[i].cor, MAX_COR - 1);
            corTemp[MAX_COR - 1] = '\0';
            toLowerCase(corTemp);

            if (strncmp(corTemp, "vermelho", MAX_COR) == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    }

    if (strstr(missao, "cor Azul") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            char corTemp[MAX_COR];
            strncpy(corTemp, mapa[i].cor, MAX_COR - 1);
            corTemp[MAX_COR - 1] = '\0';
            toLowerCase(corTemp);

            if (strncmp(corTemp, "azul", MAX_COR) == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    }

    if (strstr(missao, "pelo menos 3 territorios") != NULL) {
        int zerados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas == 0) zerados++;
        }
        return (zerados >= 3) ? 1 : 0;
    }

    if (strstr(missao, "0 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas == 0) return 1;
        }
        return 0;
    }

    return 0;
}

void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa) free(mapa);
    if (missao) free(missao);
}

int exibirMenuAcoes(void) {
    int escolha;
    printf("--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("Escolha sua ação: ");
    scanf("%d", &escolha);
    limparBufferEntrada();
    return escolha;
}

