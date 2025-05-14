/*
O programa deve receber da linha de comando o nome e matrícula de uma pessoa e registrar
os dados no arquivo cad.txt.

Se a matrícula informada já estiver registrada no arquivo, o nome deve ser sobrescrito. 

Se a matrícula informada não estiver registrada no arquivo, o novo cadastro 
deve ser anotado em uma nova linha no final do arquivo.

Utilize o seguinte esqueleto:
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

typedef struct {
    char nome[128];
    unsigned mat;
}cadastro_t;

int interpreta(int argc, char * argv[], cadastro_t * cad);
int registra(cadastro_t * cad, cadastro_t * dados, int tamanhoDados);
int ReceberDados(cadastro_t * cad, int tamanho);
int Contador();

int main(int argc, char * argv[])
{
    setlocale(LC_ALL, "Portuguese");

    cadastro_t cad;

    int cont = Contador();

    if(cont == -1 || cont == 0){
        printf("Programa encerrado!\n");
        return -1;
    }

    //printf("Tudo certo até aqui!\n");
    //printf("%d\n", cont);

    cadastro_t *dados = malloc(cont * sizeof(cadastro_t));

    if(ReceberDados(dados, cont) != 0){
        printf("Erro na estrutura de dados ou ao carregar os arquivos\n!");
        return -1;
    }

    //printf("Tudo certo até aqui!\n");
    
    if (interpreta(argc, argv, &cad) != 0)
    {
        printf("Erro na linha de comando \r\n");
        return -1;
    }
    
    if (registra(&cad, dados, cont) != 0)
    {
        printf("Erro ao registrar no arquivo \r\n");
        return -1;
    }

    free(dados);

    return 0;
}

int ReceberDados(cadastro_t * dados, int tamanho){

    if(dados == NULL) return -1;

    FILE *file = fopen("cad.txt", "r");
    if(file == NULL) return -1;

    //int cont = 0;
    char frase[150]; 
    for(int i = 0; i<tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *matricula = strtok(frase, " ");
        char *nome = strtok(NULL, "\n");
        dados[i].mat = strtoul(matricula, NULL, 10);
        strncpy(dados[i].nome, nome, sizeof(dados->nome));

        //printf("%d %s\n", dados[i].mat, dados[i].nome);
    }
    /*while((fgets(frase, sizeof(frase), file)) != NULL){
        char *matricula = strtok(frase, " ");
        char *nome = strtok(NULL, "\n");

        if (matricula == NULL || nome == NULL) {
            printf("Linha mal formatada, ignorando.\n");
            continue;
        }
        
        dados[cont].mat = strtoul(matricula, NULL, 10);
        strncpy(dados[cont].nome, nome, sizeof(dados->nome));

        printf("%d %s\n", dados[cont].mat, dados[cont].nome);

        if(feof(file)) break;
        
        ++cont;

        if(cont == tamanho) break;
    }*/

    fclose(file);

    //printf("Tudo certo até aqui!\n");

    return 0;
}

int Contador(){
     FILE *file = fopen("cad.txt", "r");

    if(file == NULL)
    {
        printf("Arquivo .txt não encontrado!\nPrograma encerrado!\n");
        return -1;
    }

    char frase[150];
    int cont = 0;

    while((fgets(frase, sizeof(frase), file)) != NULL)
    {
        ++cont;
        //letra = fgetc(file);
        //if(letra == '\n') ++cont;
        //if(feof(file)) ++cont;
    }

    if(cont == 0){
        fclose(file);
        printf("Contagem não ocorreu como esperado!\n");
        return -1;
    }

    fclose(file);

    return cont;
}

int interpreta(int argc, char * argv[], cadastro_t * cad)
{
    if(argc < 3) return -1;

    if(cad == NULL) return -1;

    cad->mat = strtoul(argv[1], NULL, 10);
    if(cad->mat == 0) return -1;

    strncpy(cad->nome, argv[2], sizeof(cad->nome));
    //cad[0].nome[sizeof(cad->nome)-1] = '\n';

    //printf("Interpreta %d %s\n",cad->mat, cad->nome);

    return 0;
}

int registra(cadastro_t * cad, cadastro_t *dados, int tamanhoDados)
{
    if(dados == NULL) return -1;

    bool registra = false;

    //printf("%d\n", tamanhoDados);

    for(int i=0; i<tamanhoDados; i++){
        if(cad->mat == dados[i].mat){
            printf("Matricula %d achado na posição %d com o nome de %s.\nO nome será substituido por %s\n", cad->mat, i, dados[i].nome, cad->nome);
            strncpy(dados[i].nome, cad->nome, sizeof(dados->nome));
            registra = true;
            break;
        }
    }

    if(registra == true){
        FILE *file = fopen("temp.txt", "r");
        if(file != NULL){
            printf("Arquivo temp.txt possui dados!\n");
            return -1;
        }
        fclose(file);
        file = fopen("temp.txt", "w");
        for(int i=0; i<tamanhoDados; i++){
            fprintf(file,"%d %s\n", dados[i].mat, dados[i].nome);
        }

        fclose(file);
        remove("cad.txt");
        rename("temp.txt", "cad.txt");
        printf("Alteração feita com sucesso!\n");
    }

    if(registra == false){
        FILE *file = fopen("cad.txt", "a");
        fprintf(file,"%d %s\n", cad->mat, cad->nome);
        printf("%d %s\n",cad->mat, cad->nome);
        printf("Matricula e nome adicionados com sucesso!\n");
        fclose(file);
    }

    return 0;
}