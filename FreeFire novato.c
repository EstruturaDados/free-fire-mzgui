#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_MOCHILA 10

// Enum CriterioOrdenacao: Define os critérios para a lógica do Insertion Sort
typedef enum { NOME, TIPO, PRIORIDADE } CriterioOrdenacao;

// Struct Item: Definição do componente de loot
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;
} Item;

// Estado Global (Escopo de Sistema)
Item mochila[MAX_MOCHILA];
int numItens = 0;
int comparacoesUltimaOrdenacao = 0;
bool ordenadaPorNome = false;

// Protótipos das Funções
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();

int main() {
    int opcao;
    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa buffer de entrada falha
            continue;
        }
        
        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: menuDeOrdenacao(); break;
            case 5: buscaBinariaPorNome(); break;
            case 0: printf("\nSaindo do sistema de sobrevivencia...\n"); break;
            default: printf("\nErro: Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementações ---

void limparTela() {
    for (int i = 0; i < 30; i++) printf("\n");
}

void exibirMenu() {
    printf("\n========================================");
    printf("\n   SISTEMA DE INVENTARIO - MESTRE");
    printf("\n   Status: %d/%d Itens | Ordenado Nome: %s", 
           numItens, MAX_MOCHILA, ordenadaPorNome ? "SIM" : "NAO");
    printf("\n========================================");
    printf("\n1. Adicionar Item");
    printf("\n2. Remover Item");
    printf("\n3. Listar Itens");
    printf("\n4. Ordenar Mochila (Insertion Sort)");
    printf("\n5. Busca Binaria (Por Nome)");
    printf("\n0. Sair");
    printf("\nEscolha uma acao: ");
}

void inserirItem() {
    if (numItens >= MAX_MOCHILA) {
        printf("\nErro: Mochila cheia! Gerencie seus recursos.\n");
        return;
    }

    Item novo;
    printf("\nNome do item: ");
    scanf("%29s", novo.nome);
    printf("Tipo (Arma/Municao/Cura): ");
    scanf("%19s", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Prioridade (1-5): ");
    scanf("%d", &novo.prioridade);

    mochila[numItens] = novo;
    numItens++;
    ordenadaPorNome = false; // Inserção quebra a ordem prévia
    printf("\nItem adicionado com sucesso!\n");
}

void removerItem() {
    char busca[30];
    printf("\nNome do item a remover: ");
    scanf("%29s", busca);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, busca) == 0) {
            // Shift para manter a lista sequencial (Evita buracos no vetor)
            for (int j = i; j < numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            numItens--;
            printf("\nItem '%s' removido.\n", busca);
            return;
        }
    }
    printf("\nErro: Item nao encontrado.\n");
}

void listarItens() {
    if (numItens == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    printf("\n%-15s | %-10s | %-5s | %-5s", "NOME", "TIPO", "QTD", "PRIO");
    printf("\n--------------------------------------------");
    for (int i = 0; i < numItens; i++) {
        printf("\n%-15s | %-10s | %-5d | %-5d", 
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("\n");
}

void menuDeOrdenacao() {
    int escolha;
    printf("\nEscolha o criterio de ordenacao:");
    printf("\n1. Nome (A-Z)\n2. Tipo (A-Z)\n3. Prioridade (5-1)\nOpcao: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1: insertionSort(NOME); break;
        case 2: insertionSort(TIPO); break;
        case 3: insertionSort(PRIORIDADE); break;
        default: printf("\nCriterio invalido.\n"); return;
    }
    printf("\nOrdenacao concluida. Comparações realizadas: %d\n", comparacoesUltimaOrdenacao);
}

void insertionSort(CriterioOrdenacao criterio) {
    comparacoesUltimaOrdenacao = 0;
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        bool condicao = false;

        while (j >= 0) {
            comparacoesUltimaOrdenacao++;
            if (criterio == NOME) condicao = (strcmp(mochila[j].nome, chave.nome) > 0);
            else if (criterio == TIPO) condicao = (strcmp(mochila[j].tipo, chave.tipo) > 0);
            else if (criterio == PRIORIDADE) condicao = (mochila[j].prioridade < chave.prioridade); // Maior primeiro

            if (condicao) {
                mochila[j + 1] = mochila[j];
                j--;
            } else break;
        }
        mochila[j + 1] = chave;
    }
    ordenadaPorNome = (criterio == NOME);
}

void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("\nErro: A mochila precisa estar ordenada por NOME para busca binaria.\n");
        return;
    }

    char alvo[30];
    printf("\nNome do item para busca binaria: ");
    scanf("%29s", alvo);

    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int res = strcmp(mochila[meio].nome, alvo);

        if (res == 0) {
            printf("\nItem Encontrado!");
            printf("\nNome: %s | Tipo: %s | Qtd: %d | Prio: %d\n", 
                   mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade, mochila[meio].prioridade);
            return;
        }
        if (res < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    printf("\nItem '%s' nao encontrado no inventario.\n", alvo);
}