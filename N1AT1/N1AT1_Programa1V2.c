#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
    long timestamp;
    char sensor[10];
    float medicao;
}DadosSensor;

int Contador();
int CarregarDados(DadosSensor *dados, int *tamanho);
int SepararDados(DadosSensor *dados, DadosSensor *separar, int *tamanho);
int AlgoritmoOrdenacao(DadosSensor *separar, int *tamanho);
int GravarDados(DadosSensor *separar, char *sensor, int *tamanho);

int main(){

    int cont = Contador();
    if(cont == -1){
        printf("Erro ao contar as linhas do arquivo!\nPrograma encerrado!\n");
        return -1;
    }

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
    DadosSensor *separar = malloc(cont * sizeof(DadosSensor));

    if(CarregarDados(dados, &cont) != 0){
        printf("Erro ao carregar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    if(SepararDados(dados, separar, &cont) !=0){
        printf("Erro ao separar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    free(dados);
    free(separar);

    return 0;
}

int SepararDados(DadosSensor *dados, DadosSensor *separar, int *tamanho){
    if(dados == NULL || separar == NULL) return -1;
    
    char nomeSensor[10];
    bool aindaTemSensor = true;

    while(aindaTemSensor == true){
        aindaTemSensor = false;

        for(int i=0; i<*tamanho; i++){
            if(strcmp(dados[i].sensor, "")!=0){
                strcpy(nomeSensor, dados[i].sensor);
                aindaTemSensor = true;
                break;
            }
        }

        if(aindaTemSensor == false) break;
        
        for(int i=0; i <*tamanho; i++){
            if(strcmp(nomeSensor, dados[i].sensor)==0){
                separar[i].timestamp = dados[i].timestamp;
                strcpy(separar[i].sensor, dados[i].sensor);
                separar[i].medicao = dados[i].medicao;

                dados[i].timestamp = 0;
                strcpy(dados[i].sensor, "");
                dados[i].medicao = 0;

            }
        }

        if(AlgoritmoOrdenacao(separar, tamanho) != 0){
            printf("Erro ao fazer a ordenação da estrutura!\n");
            return -1;
        }

        for(int i=0; i <*tamanho; i++){
                separar[i].timestamp = 0;
                strcpy(separar[i].sensor, "");
                separar[i].medicao = 0;
            }
                
    } 

    //printf("4 - Passei por aqui!\n");
    return 0;
}

int CarregarDados(DadosSensor *dados, int *tamanho){
    printf("%d %d\n", tamanho, *tamanho);

    if(dados == NULL) return -1;

    FILE *file = fopen("dadosAleatorios.txt", "r");
    if(file == NULL) return -1;

    char frase[100];
    for(int i = 0; i<*tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *timestamp = strtok(frase, " ");
        char *sensor = strtok(NULL, " ");
        char *medicao = strtok(NULL, "\n");

        //printf("%s\n", frase);

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        dados[i].medicao = strtof(medicao, NULL);

        //printf("%d - %d %s %.2f\n", i, dados[i].timestamp, dados[i].sensor, dados[i].medicao);
    }

    //printf("1 - Passei por aqui!\n");

    return 0;
}

int Contador(){
    FILE *file = fopen("dadosAleatorios.txt", "r");

    if(file == NULL) return 1;

    char frase[100];
    int cont = 0;
    while(fgets(frase, sizeof(frase), file) != NULL) {
        ++cont;
    }
    fclose(file);

    if(cont == 0) return -1;

    printf("%d\n", cont);

    return cont;
}

int AlgoritmoOrdenacao(DadosSensor *separar, int *tamanho){
    if(separar == NULL || tamanho == 0) return -1;

    long timestamp;
    char sensor[10];
    float medicao;

    for(int i=0; i<*tamanho; i++){
        for(int j = i+1; j<*tamanho ;j++){
            if(separar[i].timestamp>separar[j].timestamp){

                timestamp = separar[j].timestamp;
                strcpy(sensor, separar[j].sensor);
                medicao = separar[j].medicao;

                separar[j].timestamp = separar[i].timestamp;
                strcpy(separar[j].sensor, separar[i].sensor);
                separar[j].medicao = separar[i].medicao;

                separar[i].timestamp = timestamp;
                strcpy(separar[i].sensor, sensor);
                separar[i].medicao = medicao;

            }
        }
    }

    if(GravarDados(separar, sensor, tamanho) != 0) return -1;

    //printf("3 - Passei por aqui!\n");

    return 0;
}

int GravarDados(DadosSensor *separar, char *sensor, int *tamanho){
    if(separar == NULL || sensor == NULL || tamanho == 0){
        printf("Erro ao repassar os dados para a função GravarDados()!\n");
        return -1;
    }

    char nomeArquivo[10];
    char extensao[] = ".txt";

    strcpy(nomeArquivo, sensor);
    strcat(nomeArquivo, extensao);

    FILE *file = fopen(nomeArquivo, "r");
    if(file != NULL){
        printf("Arquivo %s já possui dados gravados!\n", nomeArquivo);
        return -1;
    }
    fclose(file);

    file = fopen(nomeArquivo, "w");
    for(int i = 0; i<*tamanho; i++){
        if(strcmp(separar[i].sensor, "") != 0) {
            fprintf(file,"%ld %s %.2f \n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
        }
    }

    fclose(file);

    //printf("2 - Passei por aqui!\n");

    return 0;
}