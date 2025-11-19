#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// estrutura para os dados 
typedef struct {
    char nome[50];
    char posicao[20];
    int camisa;
    int idade;
} Jogador;


void limpaBuffer(void);
void ler_string(char *s, int tam);
int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void gerarArquivo(FILE *arq);


void limpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void ler_string(char *s, int tam) {
    fgets(s, tam, stdin);
    s[strcspn(s, "\n")] = '\0';
}


int tamanho(FILE *arq) {
    long pos = ftell(arq);
    fseek(arq, 0, SEEK_END);
    long fim = ftell(arq);
    fseek(arq, pos, SEEK_SET);
    return (int)(fim / sizeof(Jogador));
}


void cadastrar(FILE *arq) {
    Jogador j;
    char conf;

    printf("\n=== CADASTRAR JOGADOR ===\n");

    printf("Nome: ");
    ler_string(j.nome, sizeof(j.nome));

    printf("Posicao: ");
    ler_string(j.posicao, sizeof(j.posicao));

    printf("Número da camisa: ");
    scanf("%d", &j.camisa);
    limpaBuffer();

    printf("Idade: ");
    scanf("%d", &j.idade);
    limpaBuffer();

    printf("Confirmar cadastro (s/n)? ");
    scanf("%c", &conf);
    limpaBuffer();

    if (toupper(conf) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&j, sizeof(Jogador), 1, arq);
        fflush(arq);
        printf("Jogador cadastrado!\n");
    } else {
        printf("Cancelado.\n");
    }
}


void consultar(FILE *arq) {
    int pos;
    Jogador j;

    printf("\nInforme o índice do jogador: ");
    scanf("%d", &pos);
    limpaBuffer();

    int total = tamanho(arq);
    if (pos <= 0 || pos > total) {
        printf("Índice inválido.\n");
        return;
    }

    fseek(arq, (pos - 1) * sizeof(Jogador), SEEK_SET);
    fread(&j, sizeof(Jogador), 1, arq);

    printf("\n=== JOGADOR %d ===\n", pos);
    printf("Nome: %s\n", j.nome);
    printf("Posição: %s\n", j.posicao);
    printf("Camisa: %d\n", j.camisa);
    printf("Idade: %d anos\n", j.idade);
}


void gerarArquivo(FILE *arq) {
    FILE *txt = fopen("lista_jogadores.txt", "w");
    if (!txt) {
        printf("Erro ao criar arquivo de saída.\n");
        return;
    }

    int total = tamanho(arq);
    if (total == 0) {
        printf("Nenhum jogador cadastrado.\n");
        fclose(txt);
        return;
    }

    Jogador j;
    fseek(arq, 0, SEEK_SET);

    fprintf(txt, "==== LISTA DE JOGADORES ====\n\n");

    for (int i = 0; i < total; i++) {
        fread(&j, sizeof(Jogador), 1, arq);
        fprintf(txt, "Jogador %d:\n", i + 1);
        fprintf(txt, "Nome: %s\n", j.nome);
        fprintf(txt, "Posicao: %s\n", j.posicao);
        fprintf(txt, "Camisa: %d\n", j.camisa);
        fprintf(txt, "Idade: %d\n\n", j.idade);
    }

    fclose(txt);

    printf("\nArquivo 'lista_jogadores.txt' gerado com sucesso!\n");
}


int main(void) {
    FILE *arq = fopen("jogadores.dat", "r+b");
    if (!arq) arq = fopen("jogadores.dat", "w+b");
    if (!arq) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    int op;
    do {
        printf("\n========== CADASTRO DE ATLETAS ==========\n");
        printf("\n1 - Cadastrar Jogador\n");
        printf("2 - Consultar Jogador\n");
        printf("3 - Gerar arquivo de jogadores\n");
        printf("4 - Sair\n");
        printf("\n=========================================\n");
        printf("Total: %d jogadores cadastrados\n", tamanho(arq));
        printf("Opcao: ");

        scanf("%d", &op);
        limpaBuffer();

        switch (op) {
            case 1: cadastrar(arq); break;
            case 2: consultar(arq); break;
            case 3: gerarArquivo(arq); break;
            case 4: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while (op != 4);

    fclose(arq);
    return 0;
}
