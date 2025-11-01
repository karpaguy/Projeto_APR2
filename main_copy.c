#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//------------------ Structs → Sera que structs vão precisar de alocação dinâmica para os vetores de vetores? Afinal, são só estruturas.
struct Usuario {
    char CPF[12], nome[20], telefones[2][12], emails[2][50], profissao[30], nasc[3][5];
    char numeroCasa[5], CEP[10], rua[40]; 

 // Escolhas dentro dessa Struct:
 // Max 2 Emails, @aluno.ifsp.edu.br → Modelo de email (Min 17 caracteres).
 // Max 2 Telefones, Telefone Molde → 12345-6789
 // Nascimento → 01/01/2000
};

//------------------ GERAL FUNCTIONS 
long pegartamanho_Dados(FILE *fp) {
    long tamanho;
    fseek(fp, 0, SEEK_END);
    tamanho = ftell(fp);
    tamanho = tamanho / sizeof(struct Usuario);
    printf("PORRRA: %d\n\n", tamanho);
    rewind(fp);

    if (tamanho == -1) {
        tamanho = 0;
    }
    fclose(fp);
    return tamanho;
}

//------------------ MENU FUNCTIONS
void printaMenuPrincipal() {
    printf("1. Submenu de Usuarios\n");
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

// --------------- USUARIO FUNCTIONS ---------------
struct Usuario *vetor_UsuariosAlocacao(int qtd, struct Usuario *u) {
    // Aloca espaço com base em sua quantidade. Em caso de 0s, simplesmente aloca um único espaço.
    struct Usuario *v = NULL;

    if (qtd == 0) {
        v = realloc(v, sizeof(struct Usuario));
    } else {
        v = realloc(u, qtd * sizeof(struct Usuario));
    }

    if (v == NULL) {
        return u;
    }
    printf("Tamanho vetor: %d\n", sizeof(v));
    return v;   
}

void adicionarDado_Usuarios(long *pos, struct Usuario v[]) {
    (*pos)++;
    v = vetor_UsuariosAlocacao(*pos, v);

    // Insere num outro canto as informações, se der erro, o código pode explodir e sem querer ter incrementado algo no arquivo por conta das mudanças acima. // conferir.
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
}

void listandoTodos_Usuarios(long pos, struct Usuario v[]) {
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

void UsuariosMain() {
    struct Usuario *vetor_usuarios = NULL;
    long usuario_qtd;
    int opt, i;
    FILE *fUser;

    //#--- INICIAIS
    printf("Carregando Usuarios...\n");
    if ((fUser = fopen("dados_usuarios.dat", "rb")) == NULL) {
        printf("Gerando arquivos iniciais de Usuario...\n\n");
        fUser = fopen("dados_usuarios.dat", "wb");
    }
    usuario_qtd = pegartamanho_Dados(fUser);
    vetor_usuarios = vetor_UsuariosAlocacao(usuario_qtd, vetor_usuarios);
    fread(vetor_usuarios, sizeof(Usuario), usuario_qtd, fUser);
    fclose(fUser);

    do {
        printaSubMenu_Usuarios();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("\n+------ Listando Todos ------+\n");
                listandoTodos_Usuarios(usuario_qtd, vetor_usuarios);
                // EXEMPLO TESTE RÁPIDO!
                break;
            case 2: 
                printf("\n+------ Listando Fulano: %d ------+\n");
                break;
            case 3:
                printf("\n+------ Adicionando Fulano ------+\n");
                adicionarDado_Usuarios(&usuario_qtd, vetor_usuarios);
                break;
            case 4:
                printf("\n+------ Alterando Fulano ------+\n");
                break;
            case 5:
                printf("\n+------ Excluindo Fulano ------+\n");
                break;
        }
    } while(opt != 6);
    printf("Valor - %d\n", usuario_qtd);

    if ((fUser = fopen("dados_usuarios.dat", "wb")) == NULL) {
        printf("Erro na gravacao.");
        exit(1);
    }
    for (i = 0; i < usuario_qtd; i++) {
        if ((fwrite(&vetor_usuarios[i], sizeof(struct Usuario), 1, fUser)) != 1) {
            printf("Erro na gravacao na posição: %d.\n", i);
        }

    }
    fclose(fUser);
    // escreverArquivo_Usuarios();
    // Escrever
    // Fechar arquivo
    // Limpar memória
}

int main() {    
    int opt;
    do
    {
        printaMenuPrincipal();
        printf("\nEscolha uma opcao: >___ ");
        scanf("%d", &opt);
        switch(opt) {
            case 1:
                UsuariosMain(); // Precisa de fato ser enviado como ponteiro? Necessita de testes.
                break;
        }
    } while(opt != 5);
}


// if(fUser == NULL) {
//     puts("Erro ao abrir ou criar o arquivo.");
//     exit(1);