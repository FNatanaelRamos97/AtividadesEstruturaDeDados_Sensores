/*
    O programa deve receber da linha de comando o número de matrícula e 
    apresentar na interface de saída padrão o nome registrada para matrícula informada, 
    ou informar quando a matrícula não for encontrada.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct  
{
    char nome[128];
    unsigned mat;
}cadastro_t;

char *pesquisa(int mat, cadastro_t * cad, int tamanhoDados);
int CarregarDados(cadastro_t *dados, int tamanho);
int Contador();

int main(int argc, char * argv[])
{
    setlocale(LC_ALL, "Portuguese");

    if (argc < 2)
    {
        printf("Numero de matricula nao fornecido na linha de comando \r\n");
        return -1;
    }

    int mat = strtoul(argv[1], NULL, 10);
    if(mat == 0){
        printf("Matricula incorreta!\nPrograma encerrado\n");
        return -1;
    }

    int cont = Contador();

    if(cont == -1 || cont == 0){
        printf("Contagem não ocorreu como esperado!\nPrograma encerrado!\n");
        return -1;
    }

    cadastro_t *dados = malloc(cont * sizeof(cadastro_t));

    if(CarregarDados(dados, cont) != 0){
        printf("Dados do arquivo .txt não carregados!\nPrograma encerrado!\n");
        return -1;
    }

    char *nome = pesquisa(mat, dados, cont);

    if (nome == NULL)
    {
        printf("\r\n Cadastro nao encontrado");
    }
    else
    {
        puts(nome);
    }

    return 0;
}

int Contador(){
    FILE *file = fopen("cad.txt", "r");
    if(file == NULL){
        return -1;
    }

    int cont = 0;
    char frase[150];
    while(fgets(frase, sizeof(frase), file) != NULL){
        ++cont;
    }

    if(cont == 0) return 0;

    return cont;
}

int CarregarDados(cadastro_t *dados, int tamanho){
    if(dados == NULL) return -1;

    FILE *file = fopen("cad.txt", "r");
    if(file == NULL) return -1;

    char frase[150];
    for(int i=0; i<tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *matricula = strtok(frase, " ");
        char *nome = strtok(NULL, "\n");

        dados[i].mat = strtoul(matricula, NULL, 10);
        strncpy(dados[i].nome, nome, sizeof(dados->nome));
    }

    fclose(file);

    return 0;
}

char* pesquisa(int mat, cadastro_t *dados, int tamanhoDados)
{
    for(int i = 0; i<tamanhoDados; i++){
        if(mat == dados[i].mat){
            return dados[i].nome;
            //strncpy(nome, dados[i].nome, sizeof(nome));
            //break;
        }
    }
    return NULL;
}