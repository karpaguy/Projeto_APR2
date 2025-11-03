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
void listar_usuarios(struct Usuario *DB, int qntd);
void listar_usuario(struct Usuario *DB, int qntd);
void inserir_usuario(struct Usuario **DB, int *qntd, int *capacidade);

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

    struct Usuario *DB_Usuarios = NULL;
    int qntd_elementos = 0, capacidade_total = 0;

    // Optei por não utilizar ponteiro de ponteiro por ser mais intuitivo para mim. E por questões didáticas.
    DB_Usuarios = carregar_usuarios(&qntd_elementos, &capacidade_total);

    printf("#-------- MENU DE USUARIOS. --------#\n");
    do {
        printf("1. Listar Todos os Usuarios\n");
        printf("2. Listar Usuario Especifico\n");
        printf("3. Inserir Usuario\n");
        printf("5. Sair\n");
        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            // Criar retornos para verificação por IF!
            case 1 :
                listar_usuarios(DB_Usuarios, qntd_elementos);
                break;
            case 2:
                listar_usuario(DB_Usuarios, qntd_elementos);
                break;
            case 3:
                inserir_usuario(&DB_Usuarios, &qntd_elementos, &capacidade_total);
                break;
        }
    } while (opt != 5);
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
    if ( (*qntd = tamanho / sizeof(struct Usuario)) == -1 ) {
        *qntd = 0;
    };
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

void listar_usuarios(struct Usuario *DB, int qntd) {
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

void listar_usuario(struct Usuario *DB, int qntd) {
    char cpfBusca[MAX_CPF];

    getchar();
    printf("Insira o CPF de busca: ");
    fgets(cpfBusca, sizeof(cpfBusca), stdin);
    cpfBusca[strcspn(cpfBusca, "\n")] = '\0';

    while (AUXILIAR_contarString(cpfBusca) != (MAX_CPF - 1)) {
        printf("\nstring size: %d\n", AUXILIAR_contarString(cpfBusca));
        printf("\nFormatacao invalida de CPF.\n Insira novamente: ");
        fgets(cpfBusca, sizeof(cpfBusca), stdin);
        cpfBusca[strcspn(cpfBusca, "\n")] = '\0';
    }

    // buscar_index_usuario();
}

// int buscar_index_usuario(char cpf[], struct Usuario *DB, int qntd) {


//     int i;
//     for (i = 0; i < pos; i++) {
//         if (strcmp(cpf, v[i].cpf) == 0) {
//             return i;
//         }
//     }

//     return -1;
// }

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
    
    // Lembrar de tratar bem o strcpy depois.
    strcpy((*DB)[*qntd].CPF, "1234567890");
    strcpy((*DB)[*qntd].nome, "Carlos Silva");
    strcpy((*DB)[*qntd].nome_rua, "Rua das Flores, 100");
    strcpy((*DB)[*qntd].numero_casa, "123");
    strcpy((*DB)[*qntd].numeros_telefone[0], "1199999999");
    strcpy((*DB)[*qntd].numeros_telefone[1], "1133334444");
    strcpy((*DB)[*qntd].contas_email[0], "carlos@example.com");
    strcpy((*DB)[*qntd].contas_email[1], "c.silva@trab.com");
    strcpy((*DB)[*qntd].profissao, "Engenheiro");
    (*DB)[*qntd].data_nascimento[0] = 1; // Talvez voltem a virar string.
    (*DB)[*qntd].data_nascimento[1] = 1;
    (*DB)[*qntd].data_nascimento[2] = 2001;
    strcpy((*DB)[*qntd].CEP, "12345000");

    (*qntd)++;
}

