#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// Importante: Alocação dinâmica.

//------------------ Structs → Sera que structs vão precisar de alocação dinâmica para os vetores de vetores? Afinal, são só estruturas.
struct Usuario {
    char CPF[12], nome[20], telefones[2][12], emails[2][50], profissao[30], nasc[3][5];
    char numeroCasa[5], CEP[10], rua[40]; 

 // Escolhas dentro dessa Struct:
 // Max 2 Emails, @aluno.ifsp.edu.br → Modelo de email (Min 17 caracteres).
 // Max 2 Telefones, Telefone Molde → 12345-6789
 // Nascimento → 01/01/2000
};
struct Livro {
    char ISBN[13], titulo[40], genero[15], autores[5][20], numPaginas[3];
 // Max 5 Autores
};
struct Emprestismo {
    char CPF[11], ISBN[13], dataRetirada[3][4], dataDevolucao[3][4];
    float multaDiaria;
};

//------------------ GERAL FUNCTIONS 
long tamanho_Dados(FILE *fp) {
    long tamanho;
    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);
    rewind(fp);
    fclose(fp);

    return tamanho;
}

//------------------ MENU FUNCTIONS
void printaMenuPrincipal() {
    printf("1. Submenu de Usuarios\n");
    printf("2. Submenu de Livros\n");
    printf("3. Submenu de Emprestimos\n");
    printf("4. Submenu Relatorios\n");
    printf("5. Sair\n");
}

void printaSubMenu_Usuarios() {
    printf("1. Listar todos os Usuarios\n");
    printf("2. Listar um Usuario\n"); // Busca.
    printf("3. Adicionar um Usuario\n");
    printf("4. Alterar um Usuario\n");
    printf("5. Excluir um Usuario\n"); // Ordenação
    printf("6. Retornar\n");
}

void printaSubMenu_Livros() {
    printf("1. Listar todos os Livros\n");
    printf("2. Listar um Livro\n"); // Busca.
    printf("3. Alterar um Livro\n");
    printf("4. Excluir um Livro\n"); // Ordenação
    printf("5. Retornar\n");
}

void printaSubMenu_Emprestimos() {
    printf("1. Listar todos os Emprestimos\n");
    printf("2. Listar um Emprestimo\n"); // Busca.
    printf("3. Alterar um Emprestimo\n");
    printf("4. Excluir um Emprestimo\n"); // Ordenação
    printf("5. Retornar\n");
}

void printaSubMenu_Relatorios() {
    printf("1. Mostrar todos os dados de todos os usuário com mais de X anos de idade.\n");
    printf("2. Mostrar todos os dados de todos os livros com mais de X autores.\n");
    printf("3. Mostrar o CPF da pessoa, o nome da pessoa, o ISBN do livro, o título do livro e todos os demais atributos dos empréstimos que possuem data de devolução entre as datas X e Y (inclusive).\n");
    printf("4. Retornar\n");
}

// --------------- USUARIO FUNCTIONS ---------------
// O vetor_UsuariosAlocacao poderia ser feito com ponteiro de ponteiro e remover o return.
struct Usuario *vetor_UsuariosAlocacao(FILE *fp, int qtd) {
    // Aloca espaço com base em sua quantidade. Em caso de 0s, simplesmente aloca um único espaço.
    // VERIFICAÇÃO SE A PESSOA ABRIR, O ARQUIVO INICIAL GERAR E FECHAR O ARQUIVO. Não gravar um objeto vazio.
    struct Usuario *v;
    if (qtd == 0) {
        v = malloc(sizeof *v);
        // printf("%d", sizeof *v);
    } else {
        v = malloc(qtd * sizeof *v);
        fread(v, sizeof *v, qtd, fp);
    } // Não tenho certeza se é isso, rever código.

    return v;
}

void escreverArquivo_Usuarios(FILE *fp, int qtd, struct Usuario v[]) {
    int i;

    printf("\nGravando\n");
    for (i= 0; i<MAX; i++) {
        if (fwrite(&v[i], sizeof(struct Usuario), 1, fp) != 1) {
            puts("Erro na escrita.");
        }
    }

    rewind(pa); // Será necessário mover o ponteiro? E se a pessoa abrir novamente o Usuarios? Ou for usar relatório?
} 

void listandoTodos_Usuarios(int pos, struct Usuario v[]) {
    int i;
    for (i = 0; i < pos; i++) {
        printf("POSICAO %d:\n", i);
        printf("CPF: %s\n", v[i].CPF);
        printf("Nome: %s\n", v[i].nome);
        printf("Telefone 1: %s\n", v[i].telefones[0]);
        printf("Telefone 2: %s\n", v[i].telefones[1]);
        printf("Email 1: %s\n", v[i].emails[0]);
        printf("Email 2: %s\n", v[i].emails[1]);
        printf("Profissão: %s\n", v[i].profissao);
        printf("Nascimento: %s/%s/%s\n", v[i].nasc[0], v[i].nasc[1], v[i].nasc[2]);
        printf("Numero casa: %s\n", v[i].numeroCasa);
        printf("CEP: %s\n", v[i].CEP);
        printf("Rua: %s\n\n", v[i].rua);
    }
}

void adicionarDado_Usuarios(int *pos, struct Usuario v[]) {
    // EXEMPLO DE TESTE!
    strcpy(v[*pos].CPF, "1234567890");
    strcpy(v[*pos].nome, "Carlos Silva");
    strcpy(v[*pos].telefones[0], "1199999999");
    strcpy(v[*pos].telefones[1], "1133334444");
    strcpy(v[*pos].emails[0], "carlos@example.com");
    strcpy(v[*pos].emails[1], "c.silva@trab.com");
    strcpy(v[*pos].profissao, "Engenheiro");
    strcpy(v[*pos].nasc[0], "01");
    strcpy(v[*pos].nasc[1], "01");
    strcpy(v[*pos].nasc[2], "1990");
    strcpy(v[*pos].numeroCasa, "123");
    strcpy(v[*pos].CEP, "12345000");
    strcpy(v[*pos].rua, "Rua das Flores, 100");
    (*pos)++;
}

void UsuariosMain(int pos) {
    int opt;
    FILE *fUser;

    fUser = fopen

    do {
        printaSubMenu_Usuarios();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("\n+------ Listando Todos ------+\n");
                listandoTodos_Usuarios(pos);
                // EXEMPLO TESTE RÁPIDO!
                break;
            case 2: 
                printf("\n+------ Listando Fulano: %d ------+\n", r);
                break;
            case 3:
                printf("\n+------ Adicionando Fulano ------+\n");
                adicionarDado_Usuarios(&pos, v);
                break;
            case 4:
                printf("\n+------ Alterando Fulano ------+\n");
                break;
            case 5:
                printf("\n+------ Excluindo Fulano ------+\n");
                break;
        }
    } while(opt != 6);

    escreverArquivo_Usuarios();
    // Escrever
    // Fechar arquivo
    // Limpar memória
}


// --------------- LIVROS FUNCTIONS ---------------
void LivrosMain() {
    int opt;

    do {
        printaSubMenu_Livros();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("\n+------ Listando Livros ------+\n");
                break;
            case 2: 
                printf("\n+------ Listando LivroBom ------+\n");
                break;
            case 3:
                printf("\n+------ Alterando LivroBom ------+\n");
                break;
            case 4:
                printf("\n+------ Excluindo LivroBom ------+\n");
                break;
        }
    } while(opt != 5);
}

// --------------- EMPRESTIMOS FUNCTIONS ---------------
void EmprestimosMain() {
    int opt;

    do {
        printaSubMenu_Emprestimos();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("\n+------ Listando Emprestimos ------+\n");
                break;
            case 2: 
                printf("\n+------ Listando Emprestimo ------+\n");
                break;
            case 3:
                printf("\n+------ Alterando Emprestimo ------+\n");
                break;
            case 4:
                printf("\n+------ Excluindo Emprestimo ------+\n");
                break;
        }
    } while(opt != 5);
}

// --------------- RELATORIOS FUNCTIONS ---------------
void RelatoriosMain() {
    int opt;

    do {
        printaSubMenu_Relatorios();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("\n+------ Relatorio 1 ------+\n");
                break;
            case 2:
                printf("\n+------ Relatorio 2 ------+\n");
                break;
            case 3:
                printf("\n+------ Relatorio 3 ------+\n");
                break;
        }
    } while(opt != 4);
}

int main() {

    //**-----  Variáveis
    FILE *fp;
    struct Usuario *vetor_usuarios;
    struct Livro *vetor_livros;
    struct Emprestismo *vetor_emprestimos;

    long usuario_qtd, livro_qtd, emprestimo_qtd; // Modo Header
    // int usuario_qtd=-1, livro_qtd=-1, emprestimo_qtd=-1; // Ideia anterior de fazer um "sentinel" e ver tamanho apenas ao abrir arquivos, mas aumentaria complexidade em certos casos.

    //**----- GERAÇÃO INICIAL.
    if ((fp = fopen("dados_usuarios.dat", "rb")) == NULL) {
        printf("Gerando arquivos iniciais de Usuário...\n...\n");
        fp = fopen("dados_usuarios.dat", "wb");
        fclose(fp);
    }
    if ((fp = fopen("dados_livros.dat", "rb")) == NULL) {
        printf("Gerando arquivos iniciais de Livros...\n...\n");
        fp = fopen("dados_livros.dat", "wb");
        fclose(fp);
    }
    if ((fp = fopen("dados_emprestimos.dat", "rb")) == NULL) {
        printf("Gerando arquivos iniciais de Emprestimos...\n...\n");
        fp = fopen("dados_emprestimos.dat", "wb");
        fclose(fp);
    }

    //**----- Carregamento inicial de posições. Método Header.
    // usuario_qtd = totalElementoDados(fUser); // Versão anterior, reutilizável.
    printf("Carregando...");
    usuario_qtd = tamanho_Dados((fp = fopen("dados.usuarios.dat", "rb")));
    livro_qtd = tamanho_Dados((fp = fopen("dados.usuarios.dat", "rb")));
    emprestimo_qtd = tamanho_Dados((fp = fopen("dados.usuarios.dat", "rb")));
    
    int opt;
    do
    {
        printaMenuPrincipal();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);
        switch(opt) {
            case 1:
                UsuariosMain(usuario_qtd);
                break;
            case 2:
                LivrosMain();
                break;
            case 3:
                EmprestimosMain();
                break;
            case 4:
                RelatoriosMain();
                break;
        }
    } while(opt != 5);
}


// if(fUser == NULL) {
//     puts("Erro ao abrir ou criar o arquivo.");
//     exit(1);