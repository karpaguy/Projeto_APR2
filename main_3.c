#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
int inserir_livro(struct Livro **DB, int *qntd, int *capacidade);
void salvar_livros(struct Livro *DB, int qntd);
int buscar_index_livro(char key[], struct Livro *DB, int qntd);

int AUXILIAR_contarString(char str[]);
int AUXILIAR_confirmar();
void AUXILIAR_limparBuffer();

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
            case 2:
                submenu_livros();
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
                    printf("Nenhum usuário cadastrado. Retornando...\n");
                };
                break;
            case 2:
                if (listar_especifico_usuario(DB_Usuarios, qntd_elementos) == -1) {
                    printf("Usuario nao encontrado. Retornando...\n");
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
    fgets(cpfBusca, sizeof(cpfBusca), stdin);
    cpfBusca[strcspn(cpfBusca, "\n")] = '\0';

    while (AUXILIAR_contarString(cpfBusca) != (MAX_CPF - 1)) {
        printf("\n\033[32mAVISO:\033[0m Formatacao invalida de CPF.\n Insira novamente: ");
        fgets(cpfBusca, sizeof(cpfBusca), stdin);
        cpfBusca[strcspn(cpfBusca, "\n")] = '\0';
    }

    if ((i = buscar_index_usuario(cpfBusca, DB, qntd)) == -1) {
        return i;
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

    return i;
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
    fgets(novoU.CPF, sizeof(novoU.CPF), stdin);
    AUXILIAR_lerStringRobusto(novoU.CPF);

    if (AUXILIAR_contarString(novoU.CPF) != (MAX_CPF - 1)) {
        printf("\n\033[32mAVISO:\033[0m Formatacao invalida de CPF. ");
        return 0;
    }

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
                printf("Novo CPF: "); 
                fgets(tempCPF, sizeof(tempCPF), stdin);
                AUXILIAR_lerStringRobusto(tempCPF);

                if (AUXILIAR_contarString(tempCPF) != (MAX_CPF - 1)) {
                    printf("\n\033[32mAVISO:\033[0m Formatacao invalida de CPF. ");
                } else {
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
        printf("\n\033[32mAVISO:\033[0m CPF não encontrado.");
        return 0;
    }

    printf("Cliente a ser excluido...\nUsuario: %s - CPF: %s\n", DB[i].nome, DB[i].CPF);
    if (*qntd == 0) {
        (*qntd)--;
        // O usuário que fica não será lido e será sobrescrito numa próxima iteração se inserir.
    }

    if (AUXILIAR_confirmar()) {
        for (i; i < *qntd -1; i++) {
            DB[i] = DB[i+1];
        }

        (*qntd)--;
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
        printf("2. Listar Livro Especifico [WIP]\n");
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
    fgets(novoL.ISBN, sizeof(novoL.ISBN), stdin);
    AUXILIAR_lerStringRobusto(novoL.ISBN);

    if (AUXILIAR_contarString(novoL.ISBN) != (MAX_ISBN - 1)) {
        printf("\n\033[32mAVISO:\033[0m Formatacao invalida de ISBN. ");
        return 0;
    }

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
    fgets(novoL.titulo, MAX_STRING, stdin);
    AUXILIAR_lerStringRobusto(novoL.titulo);

    // #================ AUTORES
    printf("\nNOVO - Quantos autores deseja inserir? (1 a %d): ", MAX_AUTORES);
    scanf("%d", &qtnd_autores);
    AUXILIAR_limparBuffer();

    if (qtnd_autores < 1 || qtnd_autores > MAX_AUTORES) {
        printf("\n\033[32mAVISO:\033[0m Quantidade inválida.\n");
        return 0;
    }

    // limpa todos os autores do struct antes
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
    printf("\nNOVO - Insira o Total de Páginas: ");
    scanf("%d", &novoL.numero_paginas);

    (*DB)[*qntd] = novoL;
    (*qntd)++;

    return 1;
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
