#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_CPF 12 // Ex.: 12345678900\0
#define MAX_STRING 100
#define MAX_PHONE 12 // Ex.: 1699431-7717\0
#define MAX_NROHOUSE 5
#define MAX_CEP 10

#define MIN_CAPACITY 5

// ==================== STRUCTS ====================

struct Usuario {
    char CPF[MAX_CPF];
    char nome[MAX_STRING];
    char nome_rua[MAX_STRING];
    char numero_casa[5];
    char CEP[MAX_CEP];
    char numeros_telefone[2][MAX_PHONE];
    char contas_email[2][MAX_STRING];
    char profissao[MAX_STRING];
    int data_nascimento[3];
};

// ==================== INICALIZAÇÃO (protótipo das funções) ====================
// Declarar a função para ser inicializada, evitando possíveis erros.
// Além de ficar bem mais fácil a visualização do código enorme abaixo.
void menu_principal();
void submenu_usuarios();

struct Usuario *carregar_usuarios(int *qntd, int *capacidade);
void listar_todos_usuarios(struct Usuario *DB, int qntd);
void listar_especifico_usuario(struct Usuario *DB, int qntd);
void inserir_usuario(struct Usuario **DB, int *qntd, int *capacidade);
void salvar_usuarios(struct Usuario *DB, int qntd);
int buscar_index_usuario(char key[], struct Usuario *DB, int qntd);

int AUXILIAR_contarString(char str[]);

// ==================== AUXILIARES ====================
int AUXILIAR_contarString(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

// ==================== PROGRAMA PRINCIPAL ====================
int main() {
    printf("★-------- SISTEMA DE BIBLIOTECA. --------★\nJuntos por um mundo mais literario.\nversao 0.1\n");

    menu_principal();
}

void menu_principal() {
    int opt;

    do {
        printf("1. Submenu de Usuarios\n");
        printf("2. Submenu de Livros\n");
        printf("3. Submenu de Emprestimos\n");
        printf("4. Submenu Relatorios\n");
        printf("5. Sair\n");
        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                submenu_usuarios();
                break;
        }
    } while (opt != 5);
}

// ==================== SUBPROGRAMA USUÁRIOS ====================
void submenu_usuarios() {
    int opt;
    int CPF[MAX_STRING];
    int FLAG_clientInserted = 0;

    struct Usuario *DB_Usuarios = NULL;
    int qntd_elementos = 0, capacidade_total = 0;

    // Optei por não utilizar ponteiro de ponteiro por ser mais intuitivo para mim. E por questões didáticas.
    DB_Usuarios = carregar_usuarios(&qntd_elementos, &capacidade_total);

    printf("#-------- MENU DE USUARIOS. --------#\n");
    do {
        printf("1. Listar Todos os Usuarios\n");
        printf("2. Listar Usuario Especifico\n");
        printf("3. Inserir Usuario\n");
        printf("4. Alterar Usuario [NAO IMPLEMENTADO]\n");
        printf("5. Excluir Usuario [NAO IMPLEMENTADO]\n");
        printf("6. Sair\n");
        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            // Criar retornos para verificação por IF!
            // Lidar com EXITS para return na maioria das vezes.
            case 1 :
                listar_todos_usuarios(DB_Usuarios, qntd_elementos);
                break;
            case 2:
                listar_especifico_usuario(DB_Usuarios, qntd_elementos);
                break;
            case 3:
                inserir_usuario(&DB_Usuarios, &qntd_elementos, &capacidade_total);  
                break;
        }
    } while (opt != 6);

    salvar_usuarios(DB_Usuarios, qntd_elementos);

    if(DB_Usuarios != NULL) free(DB_Usuarios);
    printf("Retornando...\n\n");
}

// ---------→→→→→ Por ordem de função chamada dentro do Subprograma!
struct Usuario *carregar_usuarios(int *qntd, int *capacidade) {
    FILE *fUser = fopen("dados_usuarios.dat", "rb");
    struct Usuario *DB = NULL;

    if (fUser  == NULL ) {
        printf("Arquivo de Usuarios nao encontrado!\nGerando arquivos iniciais de Usuario...\n\n");
        *capacidade = MIN_CAPACITY;
        *qntd = 0;

        DB = (struct Usuario*)malloc((*capacidade) * sizeof(struct Usuario));
        if (DB == NULL) {
            printf("Erro: falha na alocacao de memoria.");
            exit(1);
        }

        // Princípio de evitar excesso de tabulação.
        if ((fUser = fopen("dados_usuarios.dat", "wb")) == NULL) {
            printf("Erro: falha na criacao do arquivo.");
            exit(1);
        }

        fclose(fUser);
        return DB;
    }

    fseek(fUser, 0, SEEK_END);
    long tamanho = ftell(fUser);
    if (tamanho == -1) {
        printf("Erro: falha ao determinar tamanho do arquivo.");
        exit(0);
    }

    *qntd = tamanho / sizeof(struct Usuario);
    rewind(fUser);

    *capacidade = (*qntd > MIN_CAPACITY) ? *qntd : MIN_CAPACITY;

    DB = (struct Usuario*)malloc((*capacidade) * sizeof(struct Usuario));
    if (DB == NULL) {
        printf("Erro: falha na alocacao de memoria.");
        exit(1);
    }

    if (*qntd > 0) {
        fread(DB, sizeof(struct Usuario), *qntd, fUser);
        printf("Dados carregados.\n");
    }

    fclose(fUser);
    return DB;
}

void listar_todos_usuarios(struct Usuario *DB, int qntd) {
    int i;

    for (i = 0; i < qntd; i++) {
        printf("\nPOSICAO %d:\n", i);
        printf("CPF: %s\n", DB[i].CPF);
        printf("Nome: %s\n", DB[i].nome);
        printf("Telefone 1: %s\n", DB[i].numeros_telefone[0]);
        printf("Telefone 2: %s\n", DB[i].numeros_telefone[1]);
        printf("Email 1: %s\n", DB[i].contas_email[0]);
        printf("Email 2: %s\n", DB[i].contas_email[1]);
        printf("Profissao: %s\n", DB[i].profissao);
        printf("Nascimento: %d/%d/%d\n", DB[i].data_nascimento[0], DB[i].data_nascimento[1], DB[i].data_nascimento[2]);
        printf("CEP: %s\n", DB[i].CEP);
        printf("Rua: %s\n", DB[i].nome_rua);
        printf("Numero casa: %s\n", DB[i].numero_casa);
    }
}

void listar_especifico_usuario(struct Usuario *DB, int qntd) {
    char cpfBusca[MAX_CPF];
    int i;

    getchar();
    printf("Insira o CPF de busca: ");
    fgets(cpfBusca, sizeof(cpfBusca), stdin);
    cpfBusca[strcspn(cpfBusca, "\n")] = '\0';

    while (AUXILIAR_contarString(cpfBusca) != (MAX_CPF - 1)) {
        printf("\nstring size: %d\n", AUXILIAR_contarString(cpfBusca));
        printf("\nAVISO: Formatacao invalida de CPF.\n Insira novamente: ");
        fgets(cpfBusca, sizeof(cpfBusca), stdin);
        cpfBusca[strcspn(cpfBusca, "\n")] = '\0';
    }

    if ((i = buscar_index_usuario(cpfBusca, DB, qntd)) == -1) {
        printf("Usuario nao encontrado. Retornando...\n");
        return;
    }

    // printf("VALOR DE I: %d", i);
    printf("Usuario encontrado.\n");

    printf("\nPOSICAO %d:\n", i);
    printf("CPF: %s\n", DB[i].CPF);
    printf("Nome: %s\n", DB[i].nome);
    printf("Telefone 1: %s\n", DB[i].numeros_telefone[0]);
    printf("Telefone 2: %s\n", DB[i].numeros_telefone[1]);
    printf("Email 1: %s\n", DB[i].contas_email[0]);
    printf("Email 2: %s\n", DB[i].contas_email[1]);
    printf("Profissao: %s\n", DB[i].profissao);
    printf("Nascimento: %d/%d/%d\n", DB[i].data_nascimento[0], DB[i].data_nascimento[1], DB[i].data_nascimento[2]);
    printf("CEP: %s\n", DB[i].CEP);
    printf("Rua: %s\n", DB[i].nome_rua);
    printf("Numero casa: %s\n", DB[i].numero_casa);
}
void inserir_usuario(struct Usuario **DB, int *qntd, int *capacidade) {
    if ((*qntd) == (*capacidade)) {
        (*capacidade)++;
        
        // Fazendo com um tempDB para não dar realloc direto no DB, caso aconteça NULL, vazamos a memória inteira.
        struct Usuario *tempDB = realloc(*DB, (*capacidade) * sizeof(struct Usuario));
        if (tempDB == NULL) {
            printf("Erro: falha na realocacao de memoria.");
            exit(1);
        }

        *DB = tempDB;
    }

    struct Usuario novoU;

    // limpar buffer. Pode virar AUXILIAR
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // strcpy((*DB)[*qntd].CPF, "12345678900");
    printf("NOVO - Insira o CPF: "); // Precisa validador com trim.
    fgets(novoU.CPF, MAX_CPF, stdin);
    novoU.CPF[strcspn(novoU.CPF, "\n")] = '\0';

    while(AUXILIAR_contarString(novoU.CPF) != MAX_CPF - 1) {
        printf("\nAVISO: Tamanho inválido para CPF. Insira novamente:");
        fgets(novoU.CPF, MAX_CPF, stdin);
        novoU.CPF[strcspn(novoU.CPF, "\n")] = '\0';
    }

    while (buscar_index_usuario(novoU.CPF, *DB, *qntd) != -1) {
        printf("\nAVISO: CPF digitado já existe. Insira novamente:");
        fgets(novoU.CPF, MAX_CPF, stdin);
        novoU.CPF[strcspn(novoU.CPF, "\n")] = '\0';
    }

    while ((c = getchar()) != '\n' && c != EOF);
    // strcpy((*DB)[*qntd].nome, "Carlos Silva");
    printf("CPF valido.\nNOVO - Insira o Nome: ");
    fgets(novoU.nome, MAX_STRING, stdin);
    novoU.nome[strcspn(novoU.nome, "\n")] = '\0';
    
    // strcpy((*DB)[*qntd].CEP, "12345000");
    printf("\nNOVO - Insira o CEP: ");
    fgets(novoU.CEP, MAX_CEP, stdin);
    novoU.CEP[strcspn(novoU.CEP, "\n")] = '\0';

    // strcpy((*DB)[*qntd].nome_rua, "Rua das Flores, 100");
    printf("\nNOVO - Insira a Rua: ");
    fgets(novoU.nome_rua, MAX_STRING, stdin);
    novoU.nome_rua[strcspn(novoU.nome_rua, "\n")] = '\0';

    // strcpy((*DB)[*qntd].numero_casa, "123");
    printf("\nNOVO - Insira o Numero da Rua: ");
    fgets(novoU.numero_casa, MAX_NROHOUSE, stdin);
    novoU.numero_casa[strcspn(novoU.numero_casa, "\n")] = '\0';
    
    // strcpy((*DB)[*qntd].numeros_telefone[0], "1199999999");
    printf("\nNOVO - Insira o Primeiro Telefone: ");
    fgets(novoU.numeros_telefone[0], MAX_PHONE, stdin);
    novoU.numeros_telefone[0][strcspn(novoU.numeros_telefone[0], "\n")] = '\0';
    
    // strcpy((*DB)[*qntd].numeros_telefone[1], "1133334444");
    printf("\nNOVO - Insira o Segundo Telefone: ");
    fgets(novoU.numeros_telefone[1], MAX_PHONE, stdin);
    novoU.numeros_telefone[1][strcspn(novoU.numeros_telefone[1], "\n")] = '\0';

    // strcpy((*DB)[*qntd].contas_email[0], "carlos@example.com");
    printf("\nNOVO - Insira o Primeiro Email: ");
    fgets(novoU.contas_email[0], MAX_STRING, stdin);
    novoU.contas_email[0][strcspn(novoU.contas_email[0], "\n")] = '\0';

    // strcpy((*DB)[*qntd].contas_email[1], "c.silva@trab.com");
    printf("\nNOVO - Insira o Segundo Email: ");
    fgets(novoU.contas_email[1], MAX_STRING, stdin);
    novoU.contas_email[1][strcspn(novoU.contas_email[1], "\n")] = '\0';

    // strcpy((*DB)[*qntd].profissao, "Engenheiro");
    printf("\nNOVO - Insira a Profissao: ");
    fgets(novoU.profissao, MAX_STRING, stdin);
    novoU.profissao[strcspn(novoU.profissao, "\n")] = '\0';

    printf("\nNOVO - (Data de Nascimento)\nInsira Dia: ");
    scanf("%d", novoU.data_nascimento[0]);

    printf("\nInsira Mês: ");
    scanf("%d", novoU.data_nascimento[1]);

    printf("\nInsira Ano: ");
    scanf("%d", novoU.data_nascimento[2]);

    (*DB)[*qntd] = novoU;
    (*qntd)++;

    printf("Usuario inserido com sucesso.\n");
}

void salvar_usuarios(struct Usuario *DB, int qntd) {
    FILE *fUser;

    if ((fUser = fopen("dados_usuarios.dat", "wb")) == NULL) {
        printf("Erro: falha ao salvar usuarios.");
        exit(1);
    }

    if (qntd > 0) {
        fwrite(DB, sizeof(struct Usuario), qntd, fUser);
        printf("Usuarios salvos com sucesso.\n");
    }
    fclose(fUser);
}

int buscar_index_usuario(char key[], struct Usuario *DB, int qntd) {
    int i = 0;
    int FLAG_indexLocalized = 0;

    // Busca linear para casos simples. Ou talvez tenha que ir lá para fora, e verificar se já foi ordenado.
    if (qntd <= MIN_CAPACITY) {
        while (!FLAG_indexLocalized && i < qntd) {
            if(strcmp(DB[i].CPF, key) == 0) {
                FLAG_indexLocalized = 1;
            } else {
                i++;
            }
        }
    }

    return (FLAG_indexLocalized) ? i : -1;
}