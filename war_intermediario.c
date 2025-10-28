// DESAFIO: SISTEMA DE CADASTRO DE TERRITÓRIOS - JOGO WAR
// Nível: Aventureiro
// Objetivo desta etapa: adicionar ataque entre territórios usando ponteiros
// alocação dinâmica para o vetor de territórios e modularização


#include <stdio.h>
#include <stdlib.h> // Para alocação dinâmica
#include <string.h> // Para manupulação de strings
#include <time.h> // Para gerar números pseudo-aleatórios

// Constantes globais
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_QUANTIDADE 5

// Estrutura que representa território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Função para limpar o buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para limpar o enter do usuário / quebra de linha
void tirarQuebraLinha(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

// Função cadastrar territórios
// Cadastra n territórios no vetor dinâmico mapa
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

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[totalTerritorios].tropas);
        limparBufferEntrada();
    }

}

// Exibe todos os territórios (pós-ataque também)

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

// Função de ataque

void atacar(Territorio* atacante, Territorio* defensor) {
    // Simulação de dados com rand()
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
        // Empate: ninguém perde tropa
        printf("\nEMPATE! Nenhuma tropa perdida.\n");
    }

    // Conquista de território
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

// Liberação da memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Main - Função principal

int main() {
    // Aleatoriedade para os dados de ataque
    srand((unsigned int)time(NULL));

    Territorio* territorios = (Territorio*)calloc(MAX_QUANTIDADE, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Falha ao alocar memória.\n");
        return 1;
    }

    // Chamando a função cadastro
    cadastrarTerritorios(territorios, MAX_QUANTIDADE);

    int acabou = 0;
    while (!acabou) {
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


    // Validação de índices
    if (idxAtacante < 1 || idxAtacante > MAX_QUANTIDADE ||
        idxDefensor  < 1 || idxDefensor  > MAX_QUANTIDADE) {
        printf("Índice(s) inválido(s). Encerrando.\n");
        break;
    }

    // Ponteiros atualizados a cada rodada
    Territorio* atacante = &territorios[idxAtacante - 1];
    Territorio* defensor  = &territorios[idxDefensor  - 1];

    // Validação de cor
    if (strncmp(atacante->cor, defensor->cor, MAX_COR) == 0) {
        printf("\nOperação inválida: não é permitido atacar um território da mesma cor (mesmo exército).\n");
        continue;
    }
    
    // Verifica se o território atacante tem tropas suficientes
    if (atacante->tropas <= 0 || defensor->tropas <= 0) {
        printf("\nUm território sem tropas não pode participar da batalha!\n");
        printf("Atacante: %s (tropas: %d)\n", atacante->nome, atacante->tropas);
        printf("Defensor: %s (tropas: %d)\n", defensor->nome, defensor->tropas);
        printf("Escolha novamente territórios válidos.\n\n");
        continue; // volta para o início do while
    }

    // Executa a batalha da rodada
    atacar(atacante, defensor);

    printf("\nPressione ENTER para continuar para o próximo turno...");
    getchar();

    // Verifica se apenas um território ainda possui tropas
    int vivos = 0;
    char ultimoNome[MAX_NOME] = "";
    char ultimaCor[MAX_COR] = "";

    for (int i = 0; i < MAX_QUANTIDADE; i++) {
        if (territorios[i].tropas > 0) {
            vivos++;
            strncpy(ultimoNome, territorios[i].nome, MAX_NOME - 1);
            strncpy(ultimaCor, territorios[i].cor, MAX_COR - 1);
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

    // Libera memória
    liberarMemoria(territorios);

    return 0;
}

