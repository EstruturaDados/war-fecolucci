// DESAFIO: SISTEMA DE CADASTRO DE TERRITÓRIOS - JOGO WAR
// Nível: Novato
//
// Objetivo: Criar um sistema simples em C para cadastrar e exibir
// informações de 5 territórios usando structs e vetores.

#include <stdio.h>
#include <string.h> // Para manupulação de strings

// Constantes globais
#define MAX_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10

// Estrutura que representa território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Função para limpar o buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função principal
int main() {
    struct Territorio territorios[MAX_TERRITORIOS];


    // Laço for para cadastro dos terrotórios

    printf("\nCadastrando os Terrotórios\n");

    for (int totalTerritorios = 0; totalTerritorios < 5; totalTerritorios++) {

        printf("\n--- Cadastro de Território %d ---\n", totalTerritorios + 1);
        printf("Digite o nome do terrotório: ");
        fgets(territorios[totalTerritorios].nome, MAX_NOME, stdin);

        printf("Digite a cor do exército: ");
        fgets(territorios[totalTerritorios].cor, MAX_COR, stdin);

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[totalTerritorios].tropas);
        limparBufferEntrada();

        printf("------------------------------------\n");

    }

    printf("\n===== LISTA DE TERRITÓRIOS CADASTRADOS =====\n");
    for (int totalTerritorios = 0; totalTerritorios < 5; totalTerritorios++) {
        printf("\nTERRITÓRIO %d\n", totalTerritorios + 1);
        printf("    - Nome: %s", territorios[totalTerritorios].nome);
        printf("    - Dominado por exército: %s", territorios[totalTerritorios].cor);
        printf("    - Quantidade de tropas: %d\n", territorios[totalTerritorios].tropas);
    }

    printf("\nCadastro concluído com sucesso!\n");
    return 0;
}

