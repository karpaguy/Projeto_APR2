#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_CPF 12 // Ex.: 12345678900\0
#define MAX_STRING 100
#define MAX_PHONE 12 // Ex.: 1699431-7717\0
#define MAX_NROHOUSE 5
#define MAX_CEP 10


// ==================== STRUCTS ====================
typedef struct {
    int dia, mes, ano;
} Data;

struct Usuario {
    char CPF[MAX_CPF];
    char nome[MAX_STRING];
    char nome_rua[MAX_STRING];
    char numero_casa[5];
    char CEP[MAX_CEP];
    char numeros_telefone[2][MAX_PHONE];
    char contas_email[2][MAX_STRING];
    char profissao[MAX_STRING];
    Data data_nascimento;
};

// ==================== PROTOTIPOS ====================
// Menus
void menu_principal();
void submenu_usuarios();


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
        }

    } while (opt != 5);
}

// ==================== SUBPROGRAMA USUÁRIOS ====================
void submenu_usuarios() {
    int opt;

    printf("#-------- MENU DE USUARIOS. --------#\n");
    do {
        printf("5. Sair\n");
        printf("Escolha sua opcao: ");
        scanf("%d", &opt);
    } while (opt != 5);
}