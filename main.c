#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define MAX_CPF 12 // Ex.: 12345678900\0
#define MAX_STRING 100
#define MAX_PHONE 13 // Ex.: 16994317717\0
#define MAX_NROHOUSE 6
#define MAX_CEP 10

#define MAX_ISBN 14 // Ex: 9788533302273\0
#define MAX_AUTORES 10 // O livro com maior quantidade de autores possui 188 colaboradores. Uau.

#define MIN_CAPACITY 5

// ==================== STRUCTS ====================

struct Usuario {
    char CPF[MAX_CPF];
    char nome[MAX_STRING];
    char nome_rua[MAX_STRING];
    char numero_casa[MAX_NROHOUSE];
    char CEP[MAX_CEP];
    char numeros_telefone[2][MAX_PHONE];
    char contas_email[2][MAX_STRING];
    char profissao[MAX_STRING];
    int data_nascimento[3];
};

struct Livro {
    char ISBN[MAX_ISBN];
    char titulo[MAX_STRING];
    char genero[MAX_STRING];
    char autores[MAX_AUTORES][MAX_STRING];
    int numero_paginas;
};

struct Emprestimo {
    char CPF_Pessoa[MAX_CPF];
    char ISBN_Livro[MAX_ISBN];
    int data_retirada[3];
    int data_devolucao[3];
    float multa_diaria;
};

// ==================== INICALIZAÇÃO (protótipo das funções) ====================
// Declarar a função para ser inicializada, evitando possíveis erros.
// Além de ficar bem mais fácil a visualização do código enorme abaixo.
void menu_principal();

void submenu_usuarios();
struct Usuario *carregar_usuarios(int *qntd, int *capacidade);
int listar_todos_usuarios(struct Usuario *DB, int qntd);
int listar_especifico_usuario(struct Usuario *DB, int qntd);
int inserir_usuario(struct Usuario **DB, int *qntd, int *capacidade);
int alterar_usuario(struct Usuario *DB, int qntd);
int deletar_usuario(struct Usuario *DB, int *qntd);
void salvar_usuarios(struct Usuario *DB, int qntd);
int buscar_index_usuario(char key[], struct Usuario *DB, int qntd);

void submenu_livros();
struct Livro *carregar_livros(int *qntd, int *capacidade);
int listar_todos_livros(struct Livro *DB, int qntd);
int listar_especifico_livro(struct Livro *DB, int qntd);
int inserir_livro(struct Livro **DB, int *qntd, int *capacidade);
int alterar_livro(struct Livro *DB, int qntd);
int deletar_livro(struct Livro *DB, int *qntd);
void salvar_livros(struct Livro *DB, int qntd);
int buscar_index_livro(char key[], struct Livro *DB, int qntd);

void submenu_emprestimos();
struct Emprestimo *carregar_emprestimos(int *qntd, int *capacidade);
int listar_todos_emprestimos(struct Emprestimo *DB, int qntd);
int listar_especifico_emprestimo(struct Emprestimo *DB, int qntd);
int inserir_emprestimo(struct Emprestimo **DB, struct Usuario *DB_Usuarios, struct Livro *DB_Livros, int *qntd, int *capacidade, int qntd_u, int qntd_l);
int alterar_emprestimo(struct Emprestimo *DB, struct Usuario *DB_Usuarios, struct Livro *DB_Livros, int qntd, int qntd_u, int qntd_l);
int deletar_emprestimo(struct Emprestimo *DB, int *qntd);
void salvar_emprestimos(struct Emprestimo *DB, int qntd);
int buscar_index_emprestimo(char cpf[], char isbn[], int keys[3], struct Emprestimo *DB, int qntd);

void submenu_relatorios();
int relatorio_dados_usuarios_xidade(struct Usuario *DB, int qntd);
int relatorio_dados_livros_xautores(struct Livro *DB, int qntd);
int relatorio_dados_emprestimos_pdevolucao(struct Usuario *DB_Usuarios, struct Livro *DB_Livros, struct Emprestimo *DB_Emprestimos, int qntd_u, int qntd_l, int qntd_e);

int AUXILIAR_contarString(char str[]);
int AUXILIAR_confirmar();
void AUXILIAR_limparBuffer();
void AUXILIAR_lerData(int d[]);

int AUXILIAR_lerCPF(char *cpfBusca);
int AUXILIAR_lerISBN(char *isbnBusca);

int AUXILIAR_compararDatasEmprestimo(int devolucao[], int retirada[]);
void AUXILIAR_dataAtual(int arr[]);

// ==================== AUXILIARES ====================
int AUXILIAR_contarString(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}
int AUXILIAR_confirmar() {
    char str;
    printf("Tem certeza que deseja realizar esta operacao? (S/N)");
    scanf(" %c", &str);
    if (str == 's' || str == 'S') return 1;
    return 0;
}
void AUXILIAR_limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void AUXILIAR_lerStringRobusto(char *str) {
    // ROBUSTEZ DE CHECAGEM! Checa se o \n foi lido. Se não foi (strchr retorna NULL).
    // Significa que a entrada excedeu o buffer e o \n ainda está no stdin.

    // Se o '\n' tiver sido lido, houve espaço suficiente e não houve overflow. Se não for, o restante da linha ficou no buffer.
    if (strchr(str, '\n') == NULL) {
        AUXILIAR_limparBuffer();
    }
    
    // 1. Remove o '\n' (substituindo-o por '\0'). Se não houver um '\n' o strcspn substitui o '\0' por um '\0', logo, tudo certo.
    str[strcspn(str, "\n")] = '\0';
}

// Data a extrema utilização dessas leituras de "IDs", optei por modularizá-las.
void AUXILIAR_lerData(int d[]) {
    // Dia
    printf("Dia: ");
    scanf("%d", &d[0]);
    AUXILIAR_limparBuffer();
    while (d[0] < 1 || d[0] > 31) {
        printf("\nData invalida. Digite novamente o dia: ");
        scanf("%d", &d[0]);
    }

    // Mês
    printf("Insira Mes: ");
    scanf("%d", &d[1]);
    AUXILIAR_limparBuffer();
    while (d[1] < 1 || d[1] > 12) {
        printf("\nData invalida. Digite novamente o mes: ");
        scanf("%d", &d[1]);
    }

    // Ano
    printf("Insira Ano: ");
    scanf("%d", &d[2]);
    AUXILIAR_limparBuffer();
    while (d[2] < 0) {
        printf("\nData invalida. Digite novamente: ");
        scanf("%d", &d[2]);
    }
}
int AUXILIAR_lerCPF(char *cpfBusca) {    
    fgets(cpfBusca, MAX_CPF, stdin);
    AUXILIAR_lerStringRobusto(cpfBusca);

    if (AUXILIAR_contarString(cpfBusca) != (MAX_CPF - 1)) {
        printf("\n\033[32mAVISO:\033[0m Formatacao invalida de CPF.\n");
        return 0;
    }

    return 1;
}
int AUXILIAR_lerISBN(char *isbnBusca) {
    fgets(isbnBusca, MAX_ISBN, stdin);
    AUXILIAR_lerStringRobusto(isbnBusca);

    if (AUXILIAR_contarString(isbnBusca) != (MAX_ISBN - 1)) {
        printf("\n\033[32mAVISO:\033[0m Formatacao invalida de ISBN.\n ");
        return 0;
    }

    return 1;
}
int AUXILIAR_compararDatas(int dFinal[], int dInicial[]) {
    // Ano (Ano pode ser maior, então já retorna, mas se for igual segue. Essa é a lógica dos demais)
    if (dFinal[2] < dInicial[2]) return 0;
    if (dFinal[2] > dInicial[2]) return 1;

    // Mes ( anos iguais )
    if (dFinal[1] < dInicial[1]) return 0;
    if (dFinal[1] > dInicial[1]) return 1;

    // Dia ( anos e meses iguais )
    if (dFinal[0] < dInicial[0]) return 0;

    return 1; // Igual ou maior → válido
}
void AUXILIAR_dataAtual(int arr[]) {
    time_t agora = time(NULL);
    struct tm *dt = localtime(&agora);

    arr[0] = dt->tm_mday;
    arr[1] = dt->tm_mon + 1;
    arr[2] = dt->tm_year + 1900;
}

// ==================== PROGRAMA PRINCIPAL ====================
int main() {
    printf("★-------- SISTEMA DE BIBLIOTECA. --------★\nJuntos por um mundo mais literario.\nversao 1.0\n");
    menu_principal();
}

void menu_principal() {
    int opt;

    do {
        printf("#-------- MENU PRINCIPAL. --------#\n");
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
            case 2:
                submenu_livros();
                break;
            case 3:
                submenu_emprestimos();
                break;
            case 4:
                submenu_relatorios();
                break;
        }
    } while (opt != 5);
}

// ==================== SUBPROGRAMA USUÁRIOS ====================
void submenu_usuarios() {
    int opt;
    struct Usuario *DB_Usuarios = NULL;
    int qntd_elementos = 0, capacidade_total = 0;

    // Optei por não utilizar ponteiro de ponteiro por ser mais intuitivo para mim. E por questões didáticas.
    DB_Usuarios = carregar_usuarios(&qntd_elementos, &capacidade_total);

    do {
        printf("#-------- MENU DE USUARIOS. --------#\n");
        printf("1. Listar Todos os Usuarios\n");
        printf("2. Listar Usuario Especifico\n");
        printf("3. Inserir Usuario\n");
        printf("4. Alterar Usuario\n");
        printf("5. Excluir Usuario\n");
        printf("6. Sair\n");

        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1 :
                if (listar_todos_usuarios(DB_Usuarios, qntd_elementos) == 0) {
                    printf("Nenhum usuario cadastrado. Retornando...\n");
                };
                break;
            case 2:
                if (listar_especifico_usuario(DB_Usuarios, qntd_elementos) == 0) {
                    printf("Nao foi possivel encontrar o usuario. Retornando...\n");
                };
                break;
            case 3:
                if (inserir_usuario(&DB_Usuarios, &qntd_elementos, &capacidade_total)) {
                    printf("Usuario inserido com sucesso. Retornando...\n");
                } else {
                    printf("Erro ao inserir usuario. Retornando...\n");
                }
                break;
            case 4:
                if (alterar_usuario(DB_Usuarios, qntd_elementos)) {
                    printf("Usuario alterado com sucesso. Retornando...\n");
                } else {
                    printf("Usuario nao pode ser alterado. Retornando...\n");
                };
                break;
            case 5:
                if (deletar_usuario(DB_Usuarios, &qntd_elementos)) {
                    printf("Usuario deletado com sucesso. Retornando...\n");
                } else {
                    printf("Usuario nao pode ser deletado. Retornando...\n");
                };
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
int listar_todos_usuarios(struct Usuario *DB, int qntd) {
    int i;

    for (i = 0; i < qntd; i++) {
        printf(
            "\n\033[1;33m===== REGISTRO %d =====\033[0m\n"
            "\033[36mCPF:            \033[32m%s\033[0m\n"
            "\033[36mNome:           \033[32m%s\033[0m\n"
            "\033[36mTelefone 1:     \033[32m%s\033[0m \033[36m| Telefone 2: \033[32m%s\033[0m\n"
            "\033[36mEmail 1:        \033[32m%s\033[0m \033[36m| Email 2: \033[32m%s\033[0m\n"
            "\033[36mProfissao:      \033[32m%s\033[0m\n"
            "\033[36mNascimento:     \033[32m%02d/%02d/%04d\033[0m\n"
            "\033[36mCEP:            \033[32m%s\033[0m \033[36m| Rua: \033[32m%s\033[0m \033[36m| Numero casa: \033[32m%s\033[0m\n",
            i,
            DB[i].CPF,
            DB[i].nome,
            DB[i].numeros_telefone[0],
            DB[i].numeros_telefone[1],
            DB[i].contas_email[0],
            DB[i].contas_email[1],
            DB[i].profissao,
            DB[i].data_nascimento[0], DB[i].data_nascimento[1], DB[i].data_nascimento[2],
            DB[i].CEP,
            DB[i].nome_rua,
            DB[i].numero_casa
        );
    }

    return i;
}
int listar_especifico_usuario(struct Usuario *DB, int qntd) {
    char cpfBusca[MAX_CPF];
    int i;

    AUXILIAR_limparBuffer(); // Limpeza inicial.
    printf("Insira o CPF de busca: ");
    if ( !(AUXILIAR_lerCPF(cpfBusca)) ) return 0;
    // fgets(cpfBusca, sizeof(cpfBusca) , stdin);
    // AUXILIAR_lerStringRobusto(cpfBusca);

    // if (AUXILIAR_contarString(cpfBusca) != (MAX_CPF - 1)) {
    //     printf("\n\033[32mAVISO:\033[0m Formatacao invalida de CPF.\n");
    //     return -1;
    // }

    if ((i = buscar_index_usuario(cpfBusca, DB, qntd)) == -1) {
        return 0;
    }

    printf("Usuario encontrado.\n");

    printf(
        "\n\033[1;33m===== REGISTRO %d =====\033[0m\n"
        "\033[36mCPF:            \033[32m%s\033[0m\n"
        "\033[36mNome:           \033[32m%s\033[0m\n"
        "\033[36mTelefone 1:     \033[32m%s\033[0m \033[36m| Telefone 2: \033[32m%s\033[0m\n"
        "\033[36mEmail 1:        \033[32m%s\033[0m \033[36m| Email 2: \033[32m%s\033[0m\n"
        "\033[36mProfissao:      \033[32m%s\033[0m\n"
        "\033[36mNascimento:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mCEP:            \033[32m%s\033[0m \033[36m| Rua: \033[32m%s\033[0m \033[36m| Numero casa: \033[32m%s\033[0m\n",
        i,
        DB[i].CPF,
        DB[i].nome,
        DB[i].numeros_telefone[0],
        DB[i].numeros_telefone[1],
        DB[i].contas_email[0],
        DB[i].contas_email[1],
        DB[i].profissao,
        DB[i].data_nascimento[0], DB[i].data_nascimento[1], DB[i].data_nascimento[2],
        DB[i].CEP,
        DB[i].nome_rua,
        DB[i].numero_casa
    );

    return 1;
}
int inserir_usuario(struct Usuario **DB, int *qntd, int *capacidade) {
    
    if ((*qntd) == (*capacidade)) {
        (*capacidade)++;
        
        // Fazendo com um tempDB para não dar realloc direto no DB, caso aconteça NULL, vazamos a memória inteira.
        struct Usuario *tempDB = realloc(*DB, (*capacidade) * sizeof(struct Usuario));
        if (tempDB == NULL) {
            return 0;
        }
        
        *DB = tempDB;
    }

    struct Usuario novoU;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // #================ CPF
    printf("NOVO - Insira o CPF: ");
    if ( !(AUXILIAR_lerCPF(novoU.CPF)) ) return 0;

    if (buscar_index_usuario(novoU.CPF, *DB, *qntd) != -1) {
        printf("\n\033[32mAVISO:\033[0m CPF digitado ja existe.\n");
        return 0;
    }
    
    // #================ NOME
    printf("CPF valido.\nNOVO - Insira o Nome: ");
    fgets(novoU.nome, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoU.nome);
    
    // #================ CEP
    printf("\nNOVO - Insira o CEP: ");
    fgets(novoU.CEP, MAX_CEP, stdin);
    AUXILIAR_lerStringRobusto(novoU.CEP);

    // #================ RUA NOME
    printf("\nNOVO - Insira a Rua: ");
    fgets(novoU.nome_rua, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoU.nome_rua);


    // #================ CASA NUMERO
    printf("\nNOVO - Insira o Numero da Casa: ");
    fgets(novoU.numero_casa, MAX_NROHOUSE, stdin);
    AUXILIAR_lerStringRobusto(novoU.numero_casa);
    
    // #================ NUMERO CELULAR 1
    printf("\nNOVO - Insira o Primeiro Telefone: ");
    fgets(novoU.numeros_telefone[0], MAX_PHONE, stdin);
    AUXILIAR_lerStringRobusto(novoU.numeros_telefone[0]);

    
    // #================ NUMERO CELULAR 2
    printf("\nNOVO - Insira o Segundo Telefone: ");
    fgets(novoU.numeros_telefone[1], MAX_PHONE, stdin);
    AUXILIAR_lerStringRobusto(novoU.numeros_telefone[1]);


    // #================ EMAIL 1
    printf("\nNOVO - Insira o Primeiro Email: ");
    fgets(novoU.contas_email[0], MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoU.contas_email[0]);

    // #================ EMAIL 2
    printf("\nNOVO - Insira o Segundo Email: ");
    fgets(novoU.contas_email[1], MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoU.contas_email[1]);

    // #================ PROFISSAO
    printf("\nNOVO - Insira a Profissao: ");
    fgets(novoU.profissao, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoU.profissao);


    // #================ NASCIMENTO
    printf("\nNOVO - (Data de Nascimento)\nInsira Dia: ");
    scanf("%d", &novoU.data_nascimento[0]);

    while (novoU.data_nascimento[0] < 1 || novoU.data_nascimento[0] > 31) {
        printf("\nData invalida. Digite novamente o dia: ");
        scanf("%d", &novoU.data_nascimento[0]);
    }
                
    printf("Insira Mes: ");
    scanf("%d", &novoU.data_nascimento[1]);
    
    while (novoU.data_nascimento[1] < 1 || novoU.data_nascimento[1] > 12) {
        printf("\nData invalida. Digite novamente o mes: ");
        scanf("%d",  &novoU.data_nascimento[1]);
    }

    printf("Insira Ano: ");
    scanf("%d", &novoU.data_nascimento[2]);

    while (novoU.data_nascimento[2] < 0) {
        printf("\nData invalida. Digite novamente: ");
        scanf("%d", &novoU.data_nascimento[2]);
    }

    (*DB)[*qntd] = novoU;
    (*qntd)++;

    return 1;
}
int alterar_usuario(struct Usuario *DB, int qntd) {
    char CPF_busca[MAX_CPF];
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    printf("Digite o CPF do cliente que deseja alterar: ");
    scanf("%s", CPF_busca); // Pode ser scanf, caso não digite no tamanho certo ele não vai localizar e não segue.

    int i = buscar_index_usuario(CPF_busca, DB, qntd);
    if (i == -1) {
        printf("\n\033[32mAVISO:\033[0m CPF nao encontrado.");
        return 0;
    }

    printf("Usuario encontrado.");
    printf(
        "\n\033[1;33m===== REGISTRO %d =====\033[0m\n"
        "\033[36mCPF:            \033[32m%s\033[0m\n"
        "\033[36mNome:           \033[32m%s\033[0m\n"
        "\033[36mTelefone 1:     \033[32m%s\033[0m \033[36m| Telefone 2: \033[32m%s\033[0m\n"
        "\033[36mEmail 1:        \033[32m%s\033[0m \033[36m| Email 2: \033[32m%s\033[0m\n"
        "\033[36mProfissao:      \033[32m%s\033[0m\n"
        "\033[36mNascimento:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mCEP:            \033[32m%s\033[0m \033[36m| Rua: \033[32m%s\033[0m \033[36m| Numero casa: \033[32m%s\033[0m\n",
        i,
        DB[i].CPF,
        DB[i].nome,
        DB[i].numeros_telefone[0],
        DB[i].numeros_telefone[1],
        DB[i].contas_email[0],
        DB[i].contas_email[1],
        DB[i].profissao,
        DB[i].data_nascimento[0], DB[i].data_nascimento[1], DB[i].data_nascimento[2],
        DB[i].CEP,
        DB[i].nome_rua,
        DB[i].numero_casa
    );

    // Clone independente do usuário em questão a ser editado. Variáveis para edição.
    struct Usuario temp; char tempCPF[MAX_CPF]; int opt;
    temp = DB[i];

    do{
       printf("Qual informacao deseja alterar?\n1. CPF\n2. Nome\n3. Telefones\n4. Emails\n5. Profissao\n6. Data Nascimento\n7. Endereco\n8. Sair\n>__ ");
       scanf("%d", &opt);
       AUXILIAR_limparBuffer();

        switch (opt) {
            case 1: 
            // Verificação com outra variável, para evitar sobrescrever e depois verificar que não pode.
                printf("Novo CPF: "); 
                if ( !(AUXILIAR_lerCPF(tempCPF)) ) break;
                else {
                    if (buscar_index_usuario(tempCPF, DB, qntd) != -1) { 
                        printf("\n\033[32mAVISO:\033[0m CPF já existe."); 
                    } 
                    else { 
                        strcpy(temp.CPF, tempCPF);
                    }
                }

                break;
            case 2:
                printf("Novo nome: ");
                fgets(temp.nome, MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.nome);
                break;
            case 3:
                printf("Novo telefone 1: ");
                fgets(temp.numeros_telefone[0], MAX_PHONE, stdin);
                AUXILIAR_lerStringRobusto(temp.numeros_telefone[0]);

                printf("Novo telefone 2: ");
                fgets(temp.numeros_telefone[1], MAX_PHONE, stdin);
                AUXILIAR_lerStringRobusto(temp.numeros_telefone[1]);
                break;
            case 4:
                printf("Novo email 1: ");
                fgets(temp.contas_email[0], MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.contas_email[0]);

                printf("Novo email 2: ");
                fgets(temp.contas_email[1], MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.contas_email[1]);
                break;
            case 5:
                printf("Nova profissao: ");
                fgets(temp.profissao, MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.profissao);
                break;
            case 6:
                
                printf("\nNovo dia: ");
                scanf("%d", &temp.data_nascimento[0]);
                while (temp.data_nascimento[0] < 1 || temp.data_nascimento[0] > 31) {
                    printf("\nData invalida. Digite novamente o dia: ");
                    scanf("%d", &temp.data_nascimento[0]);
                }
                
                printf("Novo mes: ");
                scanf("%d", &temp.data_nascimento[1]);
                while (temp.data_nascimento[1] < 1 || temp.data_nascimento[1] > 12) {
                    printf("\nData invalida. Digite novamente o mes: ");
                    scanf("%d", &temp.data_nascimento[1]);
                }

                printf("Novo ano: ");
                scanf("%d", &temp.data_nascimento[2]);
                while (temp.data_nascimento[2] < 0) {
                    printf("\nData invalida. Digite novamente o ano: ");
                    scanf("%d", &temp.data_nascimento[2]);
                }

                AUXILIAR_limparBuffer();
                break;

            case 7:
                printf("Nova rua: ");
                fgets(temp.nome_rua, MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.nome_rua);

                printf("Novo numero da casa: ");
                fgets(temp.numero_casa, MAX_NROHOUSE, stdin);
                AUXILIAR_lerStringRobusto(temp.numero_casa);

                printf("Novo CEP: ");
                fgets(temp.CEP, MAX_CEP, stdin);
                AUXILIAR_lerStringRobusto(temp.CEP);
                break;
        }
    } while (opt != 8);

    printf(
        "\n\033[1;33m===== USUARIO ALTERADO =====\033[0m\n"
        "\033[36mCPF:            \033[32m%s\033[0m\n"
        "\033[36mNome:           \033[32m%s\033[0m\n"
        "\033[36mTelefone 1:     \033[32m%s\033[0m \033[36m| Telefone 2: \033[32m%s\033[0m\n"
        "\033[36mEmail 1:        \033[32m%s\033[0m \033[36m| Email 2: \033[32m%s\033[0m\n"
        "\033[36mProfissao:      \033[32m%s\033[0m\n"
        "\033[36mNascimento:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mCEP:            \033[32m%s\033[0m \033[36m| Rua: \033[32m%s\033[0m \033[36m| Numero casa: \033[32m%s\033[0m\n",
        temp.CPF,
        temp.nome,
        temp.numeros_telefone[0],
        temp.numeros_telefone[1],
        temp.contas_email[0],
        temp.contas_email[1],
        temp.profissao,
        temp.data_nascimento[0], temp.data_nascimento[1], temp.data_nascimento[2],
        temp.CEP,
        temp.nome_rua,
        temp.numero_casa
    );

    printf("Alterar o usuario?\n");
    if (AUXILIAR_confirmar()) {
        DB[i] = temp;
        return 1;
    }
    return 0;
}
int deletar_usuario(struct Usuario *DB, int *qntd) {
    char CPF_busca[MAX_CPF];
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    printf("Digite o CPF do cliente que deseja excluir: ");
    scanf("%s", CPF_busca); // Pode ser scanf, caso não digite no tamanho certo ele não vai localizar e não segue.

    int i = buscar_index_usuario(CPF_busca, DB, *qntd);
    if (i == -1) {
        printf("\n\033[32mAVISO:\033[0m CPF nao encontrado.");
        return 0;
    }

    printf("Cliente a ser excluido...\nUsuario: %s - CPF: %s\n", DB[i].nome, DB[i].CPF);
    if (AUXILIAR_confirmar()) {
        if (*qntd == 0) {
            (*qntd)--;
            // O usuário que fica não será lido e será sobrescrito numa próxima iteração se inserir.
        } else {
            for (i; i < *qntd -1; i++) {
                DB[i] = DB[i+1];
            }

            (*qntd)--;
        }
        return 1;
    }

    return 0;
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

    // Busca linear.
    while (!FLAG_indexLocalized && i < qntd) {
        if(strcmp(DB[i].CPF, key) == 0) {
            FLAG_indexLocalized = 1;
        } else {
            i++;
        }
    }

    return (FLAG_indexLocalized) ? i : -1;
}

// ==================== SUBPROGRAMA USUÁRIOS ====================
void submenu_livros() {
    int opt;
    struct Livro *DB_Livros = NULL;
    int qntd_elementos = 0, capacidade_total = 0;

    // Optei por não utilizar ponteiro de ponteiro por ser mais intuitivo para mim. E por questões didáticas.
    DB_Livros = carregar_livros(&qntd_elementos, &capacidade_total);

    do {
        printf("#-------- MENU DE LIVROS. --------#\n");
        printf("1. Listar Todos os Livros\n");
        printf("2. Listar Livro Especifico\n");
        printf("3. Inserir Livro\n");
        printf("4. Alterar Livro\n");
        printf("5. Excluir Livro\n");
        printf("6. Sair\n");

        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1 :
                if (listar_todos_livros(DB_Livros, qntd_elementos) == 0) {
                    printf("Nenhum livro cadastrado. Retornando...\n");
                };
                break;
            case 2:
                if (listar_especifico_livro(DB_Livros, qntd_elementos) == 0) {
                    printf("Nao foi possivel encontrar o livro. Retornando...\n");
                };
                break;
            case 3:
                if (inserir_livro(&DB_Livros, &qntd_elementos, &capacidade_total)) {
                    printf("Livro inserido com sucesso. Retornando...\n");
                } else {
                    printf("Erro ao inserir livro. Retornando...\n");
                }
                break;
            case 4:
                if (alterar_livro(DB_Livros, qntd_elementos)) {
                    printf("Livro alterado com sucesso. Retornando...\n");
                } else {
                    printf("Livro nao pode ser alterado. Retornando...\n");
                };
                break;
            case 5:
                if (deletar_livro(DB_Livros, &qntd_elementos)) {
                    printf("Livro deletado com sucesso. Retornando...\n");
                } else {
                    printf("Livro nao pode ser deletado. Retornando...\n");
                };
                break;
        }
    } while (opt != 6);

    salvar_livros(DB_Livros, qntd_elementos);

    if(DB_Livros != NULL) free(DB_Livros);
    printf("Retornando...\n\n");
}

// ---------→→→→→ Por ordem de função chamada dentro do Subprograma!
struct Livro *carregar_livros(int *qntd, int *capacidade) {
    FILE *fBook = fopen("dados_livros.dat", "rb");
    struct Livro *DB = NULL;

    if (fBook  == NULL ) {
        printf("Arquivo de Livros nao encontrado!\nGerando arquivos iniciais de Livro...\n\n");
        *capacidade = MIN_CAPACITY;
        *qntd = 0;

        DB = (struct Livro*)malloc((*capacidade) * sizeof(struct Livro));
        if (DB == NULL) {
            printf("Erro: falha na alocacao de memoria.");
            exit(1);
        }

        // Princípio de evitar excesso de tabulação.
        if ((fBook = fopen("dados_livros.dat", "wb")) == NULL) {
            printf("Erro: falha na criacao do arquivo.");
            exit(1);
        }

        fclose(fBook);
        return DB;
    }

    fseek(fBook, 0, SEEK_END);
    long tamanho = ftell(fBook);
    if (tamanho == -1) {
        printf("Erro: falha ao determinar tamanho do arquivo.");
        exit(0);
    }

    *qntd = tamanho / sizeof(struct Livro);
    rewind(fBook);

    *capacidade = (*qntd > MIN_CAPACITY) ? *qntd : MIN_CAPACITY;

    DB = (struct Livro*)malloc((*capacidade) * sizeof(struct Livro));
    if (DB == NULL) {
        printf("Erro: falha na alocacao de memoria.");
        exit(1);
    }

    if (*qntd > 0) {
        fread(DB, sizeof(struct Livro), *qntd, fBook);
        printf("Dados carregados.\n");
    }

    fclose(fBook);
    return DB;
}
int listar_todos_livros(struct Livro *DB, int qntd) {
    int i;
    int a;

    for (i = 0; i < qntd; i++) {
        printf(
            "\n\033[1;33m===== LIVRO %d =====\033[0m\n"
            "\033[36mISBN:           \033[32m%s\033[0m\n"
            "\033[36mTitulo:         \033[32m%s\033[0m\n"
            "\033[36mGenero:         \033[32m%s\033[0m\n"
            "\033[36mNumero Paginas: \033[32m%d\033[0m\n"
            "\033[36mAutores:\033[0m\n",
            i,
            DB[i].ISBN,
            DB[i].titulo,
            DB[i].genero,
            DB[i].numero_paginas
        );

        // imprime lista de autores
        for (a = 0; a < MAX_AUTORES; a++) {
            // imprime apenas autores não vazios
            if (DB[i].autores[a][0] != '\0') {
                printf("    \033[36m- \033[32m%s\033[0m\n", DB[i].autores[a]);
            }
        }
    }

    return i;
}
int listar_especifico_livro(struct Livro *DB, int qntd) {
    char isbnBusca[MAX_ISBN];
    int i; int a;

    AUXILIAR_limparBuffer(); // Limpeza inicial.
    printf("Insira o ISBN de busca: ");
    if (!(AUXILIAR_lerISBN(isbnBusca))) return 0;

    if ((i = buscar_index_livro(isbnBusca, DB, qntd)) == -1) {
        return 0;
    }

    printf("ISBN encontrado.\n");
    printf(
        "\n\033[1;33m===== LIVRO %d =====\033[0m\n"
        "\033[36mISBN:           \033[32m%s\033[0m\n"
        "\033[36mTitulo:         \033[32m%s\033[0m\n"
        "\033[36mGenero:         \033[32m%s\033[0m\n"
        "\033[36mNumero Paginas: \033[32m%d\033[0m\n"
        "\033[36mAutores:\033[0m\n",
        i,
        DB[i].ISBN,
        DB[i].titulo,
        DB[i].genero,
        DB[i].numero_paginas
    );

    // imprime lista de autores
    for (a = 0; a < MAX_AUTORES; a++) {
        // imprime apenas autores não vazios
        if (DB[i].autores[a][0] != '\0') {
            printf("\033[36m- \033[32m%s\033[0m\n", DB[i].autores[a]);
        }
    }

    return 1;
}
int alterar_livro(struct Livro *DB, int qntd) {
    char isbnBusca[MAX_ISBN];
    int a; int aut_max;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    printf("Digite o ISBN do livro que deseja alterar: ");
    scanf("%s", isbnBusca); // Pode ser scanf, caso não digite no tamanho certo ele não vai localizar e não segue.

    int i = buscar_index_livro(isbnBusca, DB, qntd);
    if (i == -1) {
        printf("\n\033[32mAVISO:\033[0m ISBN nao encontrado.");
        return 0;
    }

    printf("ISBN encontrado.\n");
    printf(
        "\n\033[1;33m===== LIVRO %d =====\033[0m\n"
        "\033[36mISBN:           \033[32m%s\033[0m\n"
        "\033[36mTitulo:         \033[32m%s\033[0m\n"
        "\033[36mGenero:         \033[32m%s\033[0m\n"
        "\033[36mNumero Paginas: \033[32m%d\033[0m\n"
        "\033[36mAutores:\033[0m\n",
        i,
        DB[i].ISBN,
        DB[i].titulo,
        DB[i].genero,
        DB[i].numero_paginas
    );

    // imprime lista de autores
    for (a = 0; a < MAX_AUTORES; a++) {
        if (DB[i].autores[a][0] != '\0') {
            printf("\033[36m- \033[32m%s\033[0m\n", DB[i].autores[a]);
        }
    }

    // Clone independente do livro em questão a ser editado. Variáveis para edição.
    struct Livro temp; char tempISBN[MAX_ISBN]; int opt;
    temp = DB[i];

    do{
       printf("Qual informacao deseja alterar?\n1. ISBN\n2. Titulo\n3. Genero\n4. Numero de Paginas\n5. Autores\n6. Sair\n>__ ");
       scanf("%d", &opt);
       AUXILIAR_limparBuffer();

        switch (opt) {
            case 1: 
                printf("Novo ISBN: "); 
                if (!(AUXILIAR_lerISBN(tempISBN))) {
                    break;
                } else {
                    if (buscar_index_livro(tempISBN, DB, qntd) != -1) { 
                        printf("\n\033[32mAVISO:\033[0m ISBN já existe."); 
                    } 
                    else { 
                        strcpy(temp.ISBN, tempISBN);
                    }
                }
                break;
            case 2:
                printf("Novo titulo: ");
                fgets(temp.titulo, MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.titulo);
                break;
            case 3:
                printf("Novo genero: ");
                fgets(temp.genero, MAX_STRING, stdin);
                AUXILIAR_lerStringRobusto(temp.genero);
                break;
            case 4:
                printf("Novo total de paginas: ");
                scanf("%d", &temp.numero_paginas);
                break;
            case 5:
            // O que aocntece se eu deixar vazio a passagem de um... será que ele preenche como "\0"?
                printf("Quantos autores serao editados?\n\033[32mOBS:\033[0m Deixe um numero menor para remover autores, e um maior para adicionar.\n>__");
                scanf("%d", &aut_max);
                AUXILIAR_limparBuffer();

                if (aut_max < 1 || aut_max > 10) {
                    printf("Quantia invalida de autores.\n");
                    break;
                }

                for (a = 0; a < aut_max; a++) {
                    printf("%do Autor: ", a+1);
                    fgets(temp.autores[a], MAX_STRING, stdin);
                    AUXILIAR_lerStringRobusto(temp.autores[a]);

                    // Impede de tentar zerar os autores.
                    while(AUXILIAR_contarString(temp.autores[a]) == 0) {
                        printf("\033[32mAVISO:\033[0m Nome do autor nao pode ser vazio.\nInsira novamente: ");
                        fgets(temp.autores[a], MAX_STRING, stdin);
                        AUXILIAR_lerStringRobusto(temp.autores[a]);
                    }
                }

                // Apaga autores restantes.
                for (a = aut_max; a < MAX_AUTORES; a++) {
                    temp.autores[a][0] = '\0';
                }
                break;
        }
    } while (opt != 6);

    printf(
        "\n\033[1;33m===== LIVRO %d =====\033[0m\n"
        "\033[36mISBN:           \033[32m%s\033[0m\n"
        "\033[36mTitulo:         \033[32m%s\033[0m\n"
        "\033[36mGenero:         \033[32m%s\033[0m\n"
        "\033[36mNumero Paginas: \033[32m%d\033[0m\n"
        "\033[36mAutores:\033[0m\n",
        i,
        temp.ISBN,
        temp.titulo,
        temp.genero,
        temp.numero_paginas
    );

    // imprime lista de autores
    for (a = 0; a < MAX_AUTORES; a++) {
        if (temp.autores[a][0] != '\0') {
            printf("\033[36m- \033[32m%s\033[0m\n", temp.autores[a]);
        }
    }
    printf("Alterar o livro?\n");
    if (AUXILIAR_confirmar()) {
        DB[i] = temp;
        return 1;
    }
    return 0;
}
int inserir_livro(struct Livro **DB, int *qntd, int *capacidade) {
    if ((*qntd) == (*capacidade)) {
        (*capacidade)++;
        
        // Fazendo com um tempDB para não dar realloc direto no DB, caso aconteça NULL, vazamos a memória inteira.
        struct Livro *tempDB = realloc(*DB, (*capacidade) * sizeof(struct Livro));
        if (tempDB == NULL) {
            return 0;
        }
        
        *DB = tempDB;
    }

    struct Livro novoL; int qtnd_autores = 0;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // #================ ISBN
    printf("NOVO - Insira o ISBN: ");
    if (!(AUXILIAR_lerISBN(novoL.ISBN))) return 0;

    if (buscar_index_livro(novoL.ISBN, *DB, *qntd) != -1) {
        printf("\n\033[32mAVISO:\033[0m ISBN digitado ja existe.\n");
        return 0;
    }
    
    // #================ TITULO
    printf("ISBN valido.\nNOVO - Insira o Titulo: ");
    fgets(novoL.titulo, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoL.titulo);

    // #================ GENERO
    printf("\nNOVO - Insira o Genero: ");
    fgets(novoL.genero, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoL.genero);

    // #================ AUTORES
    printf("\nNOVO - Quantos autores deseja inserir? (1 a %d): ", MAX_AUTORES);
    scanf("%d", &qtnd_autores);
    AUXILIAR_limparBuffer();

    if (qtnd_autores < 1 || qtnd_autores > MAX_AUTORES) {
        printf("\n\033[32mAVISO:\033[0m Quantidade invalida.\n");
        return 0;
    }

    // Todos os autores ficam "zerados", útil para print/verificações posteriomente. String vazia!
    for (int i = 0; i < MAX_AUTORES; i++) {
        novoL.autores[i][0] = '\0';
    }

    printf("\nInsira os autores:\n");
    for (int i = 0; i < qtnd_autores; i++) {
        printf("Autor %d: ", i + 1);
        fgets(novoL.autores[i], MAX_STRING, stdin);
        AUXILIAR_lerStringRobusto(novoL.autores[i]);
    }

    // #================ PAGINAS
    printf("\nNOVO - Insira o Total de Paginas: ");
    scanf("%d", &novoL.numero_paginas);

    (*DB)[*qntd] = novoL;
    (*qntd)++;

    return 1;
}
int deletar_livro(struct Livro *DB, int *qntd) {
    char isbnBusca[MAX_ISBN];
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    printf("Digite o ISBN do livro que deseja excluir: ");
    scanf("%s", isbnBusca); // Pode ser scanf, caso não digite no tamanho certo ele não vai localizar e não segue.

    int i = buscar_index_livro(isbnBusca, DB, *qntd);
    if (i == -1) {
        printf("\n\033[32mAVISO:\033[0m ISBN nao encontrado.");
        return 0;
    }

    printf("Livro a ser excluido...\nISBN: %s - Titulo: %s\n", DB[i].ISBN, DB[i].titulo);
    if (AUXILIAR_confirmar()) {
        if (*qntd == 0) {
            (*qntd)--;
            // O livro que fica não será lido e será sobrescrito numa próxima iteração se inserir.
        } else {
            for (i; i < *qntd -1; i++) {
                DB[i] = DB[i+1];
            }

            (*qntd)--;
        }
        return 1;
    }

    return 0;
}
void salvar_livros(struct Livro *DB, int qntd) {
    FILE *fBook;

    if ((fBook = fopen("dados_livros.dat", "wb")) == NULL) {
        printf("Erro: falha ao salvar livros.");
        exit(1);
    }

    if (qntd > 0) {
        fwrite(DB, sizeof(struct Livro), qntd, fBook);
        printf("Livros salvos com sucesso.\n");
    }
    fclose(fBook);
}
int buscar_index_livro(char key[], struct Livro *DB, int qntd) {
    int i = 0;
    int FLAG_indexLocalized = 0;

    // Busca linear.
    while (!FLAG_indexLocalized && i < qntd) {
        if(strcmp(DB[i].ISBN, key) == 0) {
            FLAG_indexLocalized = 1;
        } else {
            i++;
        }
    }

    return (FLAG_indexLocalized) ? i : -1;
}

// ==================== SUBPROGRAMA EMPRESTIMOS ====================
void submenu_emprestimos() {
    int opt;
    struct Emprestimo *DB_Emprestimos = NULL;
    int qntd_elementos = 0, capacidade_total = 0;

    // Optei por não utilizar ponteiro de ponteiro por ser mais intuitivo para mim. E por questões didáticas.
    DB_Emprestimos = carregar_emprestimos(&qntd_elementos, &capacidade_total);

    struct Usuario *DB_Usuarios = NULL;
    int qntd_elementos_u = 0, capacidade_total_u = 0;

    DB_Usuarios = carregar_usuarios(&qntd_elementos_u, &capacidade_total_u);

    struct Livro *DB_Livros = NULL;
    int qntd_elementos_l = 0, capacidade_total_l = 0;

    DB_Livros = carregar_livros(&qntd_elementos_l, &capacidade_total_l);

    do {
        printf("#-------- MENU DE EMPRESTIMOS. --------#\n");
        printf("1. Listar Todos os Emprestimos\n");
        printf("2. Listar Emprestimo Especifico\n");
        printf("3. Inserir Emprestimo\n");
        printf("4. Alterar Emprestimo\n");
        printf("5. Excluir Emprestimo\n");
        printf("6. Sair\n");

        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            // Case 1 e 2 funciona somente com o DB próprio. Dado que é uma busca interna. Como não há um cadastro, não preciso validar existência da informação fora dele mesmo.
            case 1 :
                if (listar_todos_emprestimos(DB_Emprestimos, qntd_elementos) == 0) {
                    printf("Nenhum emprestimo cadastrado. Retornando...\n");
                };
                break;
            case 2:
                if (listar_especifico_emprestimo(DB_Emprestimos, qntd_elementos) == 0) {
                    printf("Nao foi possivel encontrar o emprestimo. Retornando...\n");
                };
                break;

            case 3:
                if (inserir_emprestimo(&DB_Emprestimos, DB_Usuarios, DB_Livros, &qntd_elementos, &capacidade_total, qntd_elementos_u, qntd_elementos_l)) {
                    printf("Emprestimo inserido com sucesso. Retornando...\n");
                } else {
                    printf("Erro ao inserir emprestimo. Retornando...\n");
                }
                break;
            case 4:
                if (alterar_emprestimo(DB_Emprestimos, DB_Usuarios, DB_Livros, qntd_elementos, qntd_elementos_u, qntd_elementos_l)) {
                    printf("Emprestimo alterado com sucesso. Retornando...\n");
                } else {
                    printf("Emprestimo nao pode ser alterado. Retornando...\n");
                };
                break;
            case 5:
                if (deletar_emprestimo(DB_Emprestimos, &qntd_elementos)) {
                    printf("Emprestimo deletado com sucesso. Retornando...\n");
                } else {
                    printf("Emprestimo nao pode ser deletado. Retornando...\n");
                };
                break;
        }
    } while (opt != 6);

    salvar_emprestimos(DB_Emprestimos, qntd_elementos);

    if(DB_Emprestimos != NULL) free(DB_Emprestimos);
    if(DB_Usuarios != NULL) free(DB_Usuarios);
    if(DB_Livros != NULL) free(DB_Livros);
    printf("Retornando...\n\n");
}
struct Emprestimo *carregar_emprestimos(int *qntd, int *capacidade) {
    FILE *fEmpre = fopen("dados_emprestimos.dat", "rb");
    struct Emprestimo *DB = NULL;

    if (fEmpre  == NULL ) {
        printf("Arquivo de Emprestimos nao encontrado!\nGerando arquivos iniciais de Emprestimo...\n\n");
        *capacidade = MIN_CAPACITY;
        *qntd = 0;

        DB = (struct Emprestimo*)malloc((*capacidade) * sizeof(struct Emprestimo));
        if (DB == NULL) {
            printf("Erro: falha na alocacao de memoria.");
            exit(1);
        }

        // Princípio de evitar excesso de tabulação.
        if ((fEmpre = fopen("dados_emprestimo.dat", "wb")) == NULL) {
            printf("Erro: falha na criacao do arquivo.");
            exit(1);
        }

        fclose(fEmpre);
        return DB;
    }

    fseek(fEmpre, 0, SEEK_END);
    long tamanho = ftell(fEmpre);
    if (tamanho == -1) {
        printf("Erro: falha ao determinar tamanho do arquivo.");
        exit(0);
    }

    *qntd = tamanho / sizeof(struct Emprestimo);
    rewind(fEmpre);

    *capacidade = (*qntd > MIN_CAPACITY) ? *qntd : MIN_CAPACITY;

    DB = (struct Emprestimo*)malloc((*capacidade) * sizeof(struct Emprestimo));
    if (DB == NULL) {
        printf("Erro: falha na alocacao de memoria.");
        exit(1);
    }

    if (*qntd > 0) {
        fread(DB, sizeof(struct Emprestimo), *qntd, fEmpre);
        printf("Dados carregados.\n");
    }

    fclose(fEmpre);
    return DB;
}
int listar_todos_emprestimos(struct Emprestimo *DB, int qntd) {
    int i;

    for (i = 0; i < qntd; i++) {
        printf(
            "\n\033[1;33m===== EMPRESTIMO %d =====\033[0m\n"
            "\033[36mCPF da Pessoa:      \033[32m%s\033[0m\n"
            "\033[36mISBN do Livro:      \033[32m%s\033[0m\n"
            "\033[36mData Retirada:      \033[32m%02d/%02d/%04d\033[0m\n"
            "\033[36mData Devolucao:     \033[32m%02d/%02d/%04d\033[0m\n"
            "\033[36mMulta Diaria:       \033[32m%.2f\033[0m\n",
            i,
            DB[i].CPF_Pessoa,
            DB[i].ISBN_Livro,
            DB[i].data_retirada[0], DB[i].data_retirada[1], DB[i].data_retirada[2],
            DB[i].data_devolucao[0], DB[i].data_devolucao[1], DB[i].data_devolucao[2],
            DB[i].multa_diaria
        );
    }

    return i;
}
int listar_especifico_emprestimo(struct Emprestimo *DB, int qntd) {
    char isbnBusca[MAX_ISBN]; char cpfBusca[MAX_CPF]; int data_retiradaBusca[3];
    int i;

    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // ===== CPF
    printf("Insira o CPF (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerCPF(cpfBusca)) ) return 0;

    // ===== ISBN
    printf("Insira o ISBN (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerISBN(isbnBusca)) ) return 0;

    // ===== RETIRADA
    printf("Insira a Data de Retirada (Emprestimo) de busca.\n");
    AUXILIAR_lerData(data_retiradaBusca);


    if ((i = buscar_index_emprestimo(cpfBusca, isbnBusca, data_retiradaBusca, DB, qntd)) == -1 ) {
        return i;
    }

    printf(
        "\n\033[1;33m===== EMPRÉSTIMO %d =====\033[0m\n"
        "\033[36mCPF da Pessoa:      \033[32m%s\033[0m\n"
        "\033[36mISBN do Livro:      \033[32m%s\033[0m\n"
        "\033[36mData Retirada:      \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mData Devolucao:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mMulta Diaria:       \033[32m%.2f\033[0m\n",
        i,
        DB[i].CPF_Pessoa,
        DB[i].ISBN_Livro,
        DB[i].data_retirada[0], DB[i].data_retirada[1], DB[i].data_retirada[2],
        DB[i].data_devolucao[0], DB[i].data_devolucao[1], DB[i].data_devolucao[2],
        DB[i].multa_diaria
    );

    return i;
}
int inserir_emprestimo(struct Emprestimo **DB, struct Usuario *DB_Usuarios, struct Livro *DB_Livros, int *qntd, int *capacidade, int qntd_u, int qntd_l) {
    if ((*qntd) == (*capacidade)) {
        (*capacidade)++;
        
        // Fazendo com um tempDB para não dar realloc direto no DB, caso aconteça NULL, vazamos a memória inteira.
        struct Emprestimo *tempDB = realloc(*DB, (*capacidade) * sizeof(struct Emprestimo));
        if (tempDB == NULL) {
            return 0;
        }
        
        *DB = tempDB;
    }

    struct Emprestimo novoE;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // ===== CPF
    printf("Insira o CPF: ");
    if ( !(AUXILIAR_lerCPF(novoE.CPF_Pessoa)) ) return 0;
    if (buscar_index_usuario(novoE.CPF_Pessoa, DB_Usuarios, qntd_u) == -1) {
        printf("\n\033[32mAVISO:\033[0m CPF não existe..\n");
        return 0;
    }

    // ===== ISBN
    printf("Insira o ISBN: ");
    if ( !(AUXILIAR_lerISBN(novoE.ISBN_Livro)) ) return 0;
    if (buscar_index_livro(novoE.ISBN_Livro, DB_Livros, qntd_l) == -1) {
        printf("\n\033[32mAVISO:\033[0m ISBN não existe..\n");
        return 0;
    }

    // ===== RETIRADA
    printf("NOVO - Insira a Data de Retirada.\n");
    AUXILIAR_lerData(novoE.data_retirada);
    
    // Se todos passaram pela verificação se existência em outros DB (CPF e LIVROS), seguir verificar se já não está no DB.

    if (buscar_index_emprestimo(novoE.CPF_Pessoa, novoE.ISBN_Livro, novoE.data_retirada, *DB, *qntd) != -1) {
        printf("\n\033[32mAVISO:\033[0m Cadastro de emprestimo ja existe.\n");
        return 0;
    }

    // Passado tudo isso, finalmente...
    // #================ DATA DEVOLUÇÃO
    printf("\nNOVO - Insira a Data para Devolucao.\n");
    AUXILIAR_lerData(novoE.data_devolucao);

    if (AUXILIAR_compararDatas(novoE.data_devolucao, novoE.data_retirada) == 0) {
        printf("\n\033[32mAVISO:\033[0m Data de devolucao nao pode ser antes da retirada.\n");
        return 0;
    }

    // #================ MULTA DIARIA
    printf("\nNOVO - Insira valor da Multa Diaria: ");
    scanf("%f", &novoE.multa_diaria);

    (*DB)[*qntd] = novoE;
    (*qntd)++;

    return 1;
}
int alterar_emprestimo(struct Emprestimo *DB, struct Usuario *DB_Usuarios, struct Livro *DB_Livros, int qntd, int qntd_u, int qntd_l) {
    char isbnBusca[MAX_ISBN]; char cpfBusca[MAX_CPF]; int data_retiradaBusca[3];
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // ===== CPF
    printf("Insira o CPF (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerCPF(cpfBusca)) ) return 0;

    // ===== ISBN
    printf("Insira o ISBN (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerISBN(isbnBusca)) ) return 0;

    // ===== RETIRADA
    printf("Insira a Data de Retirada (Emprestimo) de busca.\n");
    AUXILIAR_lerData(data_retiradaBusca);

    int i = buscar_index_emprestimo(cpfBusca, isbnBusca, data_retiradaBusca, DB, qntd);
    if ( i == -1) {
        printf("\n\033[32mAVISO:\033[0m Cadastro de emprestimo nao encontrado.\n");
        return 0;
    }

    printf("Emprestimo encontrado.\n");
    printf(
        "\n\033[1;33m===== EMPRÉSTIMO %d =====\033[0m\n"
        "\033[36mCPF da Pessoa:      \033[32m%s\033[0m\n"
        "\033[36mISBN do Livro:      \033[32m%s\033[0m\n"
        "\033[36mData Retirada:      \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mData Devolucao:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mMulta Diaria:       \033[32m%.2f\033[0m\n",
        i,
        DB[i].CPF_Pessoa,
        DB[i].ISBN_Livro,
        DB[i].data_retirada[0], DB[i].data_retirada[1], DB[i].data_retirada[2],
        DB[i].data_devolucao[0], DB[i].data_devolucao[1], DB[i].data_devolucao[2],
        DB[i].multa_diaria
    );

    // Clone independente do emprestimo em questão a ser editado. Variáveis para edição.
    struct Emprestimo temp; char tempISBN[MAX_ISBN]; char tempCPF[MAX_CPF]; int tempData[3]; int opt; int j;
    temp = DB[i];

    do {
        printf("Qual informacao deseja alterar?\n1. CPF do Emprestimo\n2. ISBN do Emprestimo\n3. Data Retirada\n4. Data Devolucao\n5. Multa Diara\n6. Sair\n>__");
        scanf("%d", &opt);
        AUXILIAR_limparBuffer();

        switch(opt) {
            case 1:
            // Verificação com outra variável, para evitar sobrescrever e depois verificar que não pode.
                printf("Novo CPF que realizou o emprestimo: "); 
                if ( !(AUXILIAR_lerCPF(tempCPF)) ) break;
                else {
                    if (buscar_index_usuario(tempCPF, DB_Usuarios, qntd_u) == -1) { 
                        printf("\n\033[32mAVISO:\033[0m CPF inexistente.");
                    } else {
                        if (buscar_index_emprestimo(tempCPF, temp.ISBN_Livro, temp.data_retirada, DB, qntd) != -1) {
                            printf("\n\033[32mAVISO:\033[0m Já existe um cadastro com esse CPF, ISBN e na Data Informada.");
                        }
                        else {
                            strcpy(temp.CPF_Pessoa, tempCPF);
                            printf("\n\033[32mOK!\033[0m\n");
                        }
                    }
                }

                break;
            case 2:
            // Verificação com outra variável, para evitar sobrescrever e depois verificar que não pode.
                printf("Novo ISBN do livro emprestado: "); 
                if ( !(AUXILIAR_lerISBN(tempISBN)) ) break;
                else {
                    if (buscar_index_livro(tempISBN, DB_Livros, qntd_l) == -1) { 
                        printf("\n\033[32mAVISO:\033[0m ISBN inexistente.");
                    } else {
                        if (buscar_index_emprestimo(temp.CPF_Pessoa, tempISBN, temp.data_retirada, DB, qntd) != -1) {
                            printf("\n\033[32mAVISO:\033[0m Já existe um cadastro com esse CPF, ISBN e na Data Informada.");
                        }
                        else {
                            strcpy(temp.ISBN_Livro, tempISBN);
                            printf("\n\033[32mOK!\033[0m\n");
                        }
                    }
                }

                break;
            case 3:
                printf("Nova data de emprestimo do livro: ");
                AUXILIAR_lerData(tempData);

                if (AUXILIAR_compararDatas(temp.data_devolucao, tempData) == 0) {
                    printf("\n\033[32mAVISO:\033[0m Data de devolucao nao pode ser antes da retirada.\n");
                } else{
                    if (buscar_index_emprestimo(temp.CPF_Pessoa, temp.ISBN_Livro, tempData, DB, qntd) != -1) {
                        printf("\n\033[32mAVISO:\033[0m Já existe um cadastro com esse CPF, ISBN e na Data Informada.");
                    }
                    else {
                        for (j = 0; j < 3; j++) 
                        {
                            // Atribuição elemento por elemento
                            temp.data_retirada[j] = tempData[j];
                        }
                        printf("\n\033[32mOK!\033[0m\n");
                    }
                }
        
                break;
            case 4:
                printf("Nova data de devolucao do livro: ");
                AUXILIAR_lerData(tempData);

                if (AUXILIAR_compararDatas(tempData, temp.data_retirada) == 0) {
                    printf("\n\033[32mAVISO:\033[0m Data de devolucao nao pode ser antes da retirada.\n");
                } else{
                    for (j = 0; j < 3; j++) 
                    {
                        // Atribuição elemento por elemento
                        temp.data_devolucao[j] = tempData[j];
                    }
                    printf("\n\033[32mOK!\033[0m\n");
                }

                break;
            case 5:
                printf("Novo valor da multa por atraso: ");
                scanf("%f", &temp.multa_diaria);
                break;
        }
    } while (opt != 6);

    printf(
        "\n\033[1;33m===== EMPRÉSTIMO %d =====\033[0m\n"
        "\033[36mCPF da Pessoa:      \033[32m%s\033[0m\n"
        "\033[36mISBN do Livro:      \033[32m%s\033[0m\n"
        "\033[36mData Retirada:      \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mData Devolucao:     \033[32m%02d/%02d/%04d\033[0m\n"
        "\033[36mMulta Diaria:       \033[32m%.2f\033[0m\n",
        i,
        temp.CPF_Pessoa,
        temp.ISBN_Livro,
        temp.data_retirada[0], temp.data_retirada[1], temp.data_retirada[2],
        temp.data_devolucao[0], temp.data_devolucao[1], temp.data_devolucao[2],
        temp.multa_diaria

    );

    printf("Alterar o emprestimo?\n");
    if (AUXILIAR_confirmar()) {
        DB[i] = temp;
        return 1;
    }
    return 0;
}
int deletar_emprestimo(struct Emprestimo *DB, int *qntd) {
    char isbnBusca[MAX_ISBN]; char cpfBusca[MAX_CPF]; int data_retiradaBusca[3];
    int i;

    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // ===== CPF
    printf("Insira o CPF (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerCPF(cpfBusca)) ) return 0;


    // ===== ISBN
    printf("Insira o ISBN (Emprestimo) de busca: ");
    if ( !(AUXILIAR_lerISBN(isbnBusca)) ) return 0;

    // ===== RETIRADA
    printf("Insira a Data de Retirada (Emprestimo) de busca.\n");
    AUXILIAR_lerData(data_retiradaBusca);

    if ((i = buscar_index_emprestimo(cpfBusca, isbnBusca, data_retiradaBusca, DB, *qntd)) == -1 ) {
        return 0;
    }

    i = listar_especifico_emprestimo(DB, *qntd); // Editar, ver TODO.
    if (i == -1) {
        return 0;
    }

    printf("Emprestimo a ser excluido...\nCPF: %s - ISBN: %s\n - Data de Retirada: %d/%d/%d", DB[i].CPF_Pessoa, DB[i].ISBN_Livro, DB[i].data_retirada[0], DB[i].data_retirada[1], DB[i].data_retirada[2]);
    if (AUXILIAR_confirmar()) {
        if (*qntd == 0) {
            (*qntd)--;
            // O emprestimo que fica não será lido e será sobrescrito numa próxima iteração se inserir.
        } else {
            for (i; i < *qntd -1; i++) {
                DB[i] = DB[i+1];
            }

            (*qntd)--;
        }
        return 1;
    }
    return 0;
}
void salvar_emprestimos(struct Emprestimo *DB, int qntd) {
    FILE *fEmpre;

    if ((fEmpre = fopen("dados_emprestimos.dat", "wb")) == NULL) {
        printf("Erro: falha ao salvar emprestimos.");
        exit(1);
    }

    if (qntd > 0) {
        fwrite(DB, sizeof(struct Emprestimo), qntd, fEmpre);
        printf("Emprestimos salvos com sucesso.\n");
    }
    fclose(fEmpre);
}

int buscar_index_emprestimo(char cpf[], char isbn[], int keys[3], struct Emprestimo *DB, int qntd) {
    int i = 0;
    int FLAG_indexLocalized = 0;

    // Busca linear.
    while (!FLAG_indexLocalized && i < qntd) {
        if ( (strcmp(DB[i].CPF_Pessoa, cpf) == 0) && (strcmp(DB[i].ISBN_Livro, isbn) == 0) && (keys[0] == DB[i].data_retirada[0] && keys[1] == DB[i].data_retirada[1] && keys[2] == DB[i].data_retirada[2]) ) {
            FLAG_indexLocalized = 1;
        }
        else {
            i++;
        }
    }

    return (FLAG_indexLocalized) ? i : -1; 
}
// O exercico faz com que oprograma assuma ter N livros existentes, e não conta se um livro foi devolvido ou não para os emprestismos... Poderia ser uma biblioteca virtual?

// ==================== SUBPROGRAMA RELATORIOS ====================
void submenu_relatorios() {
    int opt;

    struct Emprestimo *DB_Emprestimos = NULL;
    int qntd_elementos_e = 0, capacidade_total_e = 0;
    DB_Emprestimos = carregar_emprestimos(&qntd_elementos_e, &capacidade_total_e);

    struct Usuario *DB_Usuarios = NULL;
    int qntd_elementos_u = 0, capacidade_total_u = 0;
    DB_Usuarios = carregar_usuarios(&qntd_elementos_u, &capacidade_total_u);

    struct Livro *DB_Livros = NULL;
    int qntd_elementos_l = 0, capacidade_total_l = 0;
    DB_Livros = carregar_livros(&qntd_elementos_l, &capacidade_total_l);

    do {
        printf("#-------- MENU DE RELATORIOS. --------#\n");
        printf("1. RELATORIO: Todos os dados de todos os usuarios com X de idade.\n");
        printf("2. RELATORIO: Todos os dados de todos os livros que tenham mais do que X autores.\n");
        printf("3. RELATORIO: Todos os dados dos emprestimos com data de devolucao entre X e Y (inclusive).\n");
        printf("4. Sair\n");

        printf("Escolha sua opcao: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                if (relatorio_dados_usuarios_xidade(DB_Usuarios, qntd_elementos_u)) {
                    printf("Relatorio gerado com sucesso.\n");
                } else {
                    printf("Algo deu errado.\n");
                }
                break;
            case 2:
                if (relatorio_dados_livros_xautores(DB_Livros, qntd_elementos_l)) {
                    printf("Relatorio gerado com sucesso.\n");
                } else {
                    printf("Algo deu errado.\n");
                }
                break;
            case 3:
                if (relatorio_dados_emprestimos_pdevolucao(DB_Usuarios, DB_Livros, DB_Emprestimos, qntd_elementos_u, qntd_elementos_l, qntd_elementos_e)) {
                    printf("Relatorio gerado com sucesso.\n");
                } else {
                    printf("Algo deu errado.\n");
                }
                break;
        }

    } while (opt != 4);

    if(DB_Emprestimos != NULL) free(DB_Emprestimos);
    if(DB_Usuarios != NULL) free(DB_Usuarios);
    if(DB_Livros != NULL) free(DB_Livros);
    printf("Retornando...\n\n");
}

int relatorio_dados_usuarios_xidade(struct Usuario *DB, int qntd) {
    int idadeBusca; int i; int dataAtual[3]; int FLAG_queryLocalized = 0;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // Condição necessária.
    if (qntd == 0) {
        printf("\n\033[32mAVISO:\033[0m Não há usuários cadastrados. Retornando...\n");
        return 0;
    }

    printf("Digite a idade para filtrar: ");
    scanf("%d", &idadeBusca);
    FILE *relatorio = fopen("relatorio_dados_usuarios_xidade.txt", "w");
    if (relatorio == NULL) {
        return 0;
    }

    AUXILIAR_dataAtual(dataAtual);
    for (i = 0; i < qntd; i++) {

        int idade = dataAtual[2] - DB[i].data_nascimento[2];
        // ainda não fez aniversário este ano
        if (dataAtual[1] < DB[i].data_nascimento[1] || (dataAtual[1] == DB[i].data_nascimento[1] && dataAtual[0] < DB[i].data_nascimento[0])) idade--;
        
        if (idade >= idadeBusca) {
            fprintf(relatorio, "Nome: %s\nCPF: %s\n Rua: %s\nNúmero da casa: %s\nCEP: %s\nTelefone 1: %s\nTelefone 2: %s\nEmail 1: %s\nEmail 2: %s\nProfissão: %s\nData de nascimento: %02d/%02d/%04d\nIdade: %d\n\n", 
                DB[i].nome,
                DB[i].CPF,
                DB[i].nome_rua,
                DB[i].numero_casa,
                DB[i].CEP,
                DB[i].numeros_telefone[0],
                DB[i].numeros_telefone[1],
                DB[i].contas_email[0],
                DB[i].contas_email[1],
                DB[i].profissao,
                DB[i].data_nascimento[0],
                DB[i].data_nascimento[1],
                DB[i].data_nascimento[2],
                idade
            );

            FLAG_queryLocalized = 1;
        }
    }

    if (!FLAG_queryLocalized) {
        fprintf(relatorio, "Nenhum usuario encontrado com este filtro.\n");
    }

    fclose(relatorio);
    return 1;
}
int relatorio_dados_livros_xautores(struct Livro *DB, int qntd) {
    int quantidadeBusca; int i; int a; int FLAG_queryLocalized = 0;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // Condição necessária.
    if (qntd == 0) {
        printf("\n\033[32mAVISO:\033[0m Não há livros cadastrados. Retornando...\n");
        return 0;
    }

    printf("Digite a quantidade de autores para filtrar: ");
    scanf("%d", &quantidadeBusca);
    if (quantidadeBusca < 0 || quantidadeBusca > 10) {
        printf("\n\033[32mAVISO:\033[0m Quantidade invalida de autores. Retornando...\n");
        return 0;
    }

    FILE *relatorio = fopen("relatorio_dados_livros_xautores.txt", "w");
    if (relatorio == NULL) {
        return 0;
    }

    for (i = 0; i < qntd; i++) {
        a = 0;
        while (a < MAX_AUTORES && DB[i].autores[a][0] != '\0') a++;

        if (a > quantidadeBusca) {
            fprintf(relatorio,"Título: %s\nISBN: %s\nGênero: %s\nNúmero de páginas: %d\n",
                DB[i].titulo,
                DB[i].ISBN,
                DB[i].genero,
                DB[i].numero_paginas
            );

            // Listar autores
            fprintf(relatorio, "Autores:\n");
            for (a = 0; a < MAX_AUTORES && DB[i].autores[a][0] != '\0'; a++) {
                fprintf(relatorio, "  %d. %s\n", a + 1, DB[i].autores[a]);
            }
            fprintf(relatorio, "\n");  // Linha em branco depois do registro

            FLAG_queryLocalized = 1;
        } 
    }

    if (!FLAG_queryLocalized) {
        fprintf(relatorio, "Nenhum livro encontrado com este filtro.\n");
    }
    fclose(relatorio);
    return 1;
}
int relatorio_dados_emprestimos_pdevolucao(struct Usuario *DB_Usuarios, struct Livro *DB_Livros, struct Emprestimo *DB_Emprestimos, int qntd_u, int qntd_l, int qntd_e) {
    int dataInicial[3]; int dataFinal[3]; int FLAG_queryLocalized = 0;
    int i; int j;
    AUXILIAR_limparBuffer(); // Limpeza inicial.

    // Condições necessárias.
    if (qntd_u == 0) {
        printf("\n\033[32mAVISO:\033[0m Não há usuarios cadastrados. Retornando...\n");
        return 0;
    }
    if (qntd_l == 0) {
        printf("\n\033[32mAVISO:\033[0m Não há livros cadastrados. Retornando...\n");
        return 0;
    }
    if (qntd_e == 0) {
        printf("\n\033[32mAVISO:\033[0m Não há emprestimos cadastrados. Retornando...\n");
        return 0;
    }

    printf("Insira a DATA INICIAL de busca.\n");
    AUXILIAR_lerData(dataInicial);

    printf("Insira a DATA FINAL de busca.\n");
    AUXILIAR_lerData(dataFinal);

    if (AUXILIAR_compararDatas(dataFinal, dataInicial) == 0) {
        printf("\n\033[32mAVISO:\033[0m Data de busca final nao pode ser antes da data de busca inicial.\n");
        return 0;
    }

    FILE *relatorio = fopen("relatorio_dados_emprestimos_pdevolucao.txt", "w");
    if (relatorio == NULL) {
        return 0;
    }

    for (i = 0; i < qntd_e; i ++) {
        // Comparação para verificar se enquadra dentro dos parâmetros estabelecidos de período. Inicio > Devolução < Final 
        if (AUXILIAR_compararDatas(DB_Emprestimos[i].data_devolucao, dataInicial) && AUXILIAR_compararDatas(dataFinal, DB_Emprestimos[i].data_devolucao)) {
            fprintf(relatorio,"CPF: %s\n", DB_Emprestimos[i].CPF_Pessoa);
            j = buscar_index_usuario(DB_Emprestimos[i].CPF_Pessoa, DB_Usuarios, qntd_u);
            fprintf(relatorio,"Nome: %s\n", DB_Usuarios[j].nome);

            fprintf(relatorio,"ISBN: %s\n", DB_Emprestimos[i].ISBN_Livro);
            j = buscar_index_livro(DB_Emprestimos[i].ISBN_Livro, DB_Livros, qntd_l);
            fprintf(relatorio,"Título: %s\n", DB_Livros[j].titulo);

            fprintf(relatorio,"Data Retirada: %d/%d/%d\nData Devolução: %d/%d/%d\nMulta Diária: %.2f", 
                DB_Emprestimos[i].data_retirada[0],
                DB_Emprestimos[i].data_retirada[1],
                DB_Emprestimos[i].data_retirada[2],
                DB_Emprestimos[i].data_devolucao[0],
                DB_Emprestimos[i].data_devolucao[1],
                DB_Emprestimos[i].data_devolucao[2],
                DB_Emprestimos[i].multa_diaria
            );
            fprintf(relatorio, "\n\n");  // Linha em branco depois do registro

            FLAG_queryLocalized = 1;
        }
    }

    if (!FLAG_queryLocalized) {
        fprintf(relatorio, "Nenhuma informação encontrada com este filtro.\n");
    }
    fclose(relatorio);
    return 1;
}