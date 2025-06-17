#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
    long timestamp;
    char sensor[10];
    char medicao[17];
}DadosSensor;

int Contador(char *argv[]);
int CarregarDados(DadosSensor *dados, int *tamanho, char *argv[]);
int SepararDados(DadosSensor *dados, int *tamanho);
int AlgoritmoOrdenacao(DadosSensor *separar, int *tamanho);
int GravarDados(DadosSensor *separar, char *sensor, int *tamanho);

int main(int argc, char * argv[]){

    //Primeira etapa - Bloco de tratamento de erro para entrada de argumento
    if(argc != 2){
        printf("Numero de argumentos inválidos!\n");
        return -1;
    }

    FILE *file = fopen(argv[1],"r");
    if(file == NULL){
        printf("Arquivo inválido ou inexistente!\nDigite o nome do arquivo e a extensão .txt!\nPrograma encerrado\n");
        return -1;
    }
    fclose(file);

    printf("Nome do arquivo: %s\n", argv[1]);

    //Segunda etapa - Contar as linhas do arquivo para fazer a alocação dinâmica nas estruturas 
    int cont = Contador(argv);
    if(cont == -1){
        printf("Erro ao contar as linhas do arquivo!\nPrograma encerrado!\n");
        return -1;
    }

    printf("Quantidade de linhas: %d\n", cont);

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));

    //Terceira etapa - Carrega os dados do arquivo na estrutura 
    if(CarregarDados(dados, &cont, argv) != 0){
        printf("Erro ao carregar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    //Quarta etapa - Separa os dados por nome do sensor, ordena por timestamp e guarda em um arquivo
    if(SepararDados(dados, &cont) !=0){
        printf("Erro ao separar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    //Libera a alocação dinâmica
    free(dados);

    return 0;
}

int SepararDados(DadosSensor *dados, int *tamanho){
    if(dados == NULL){
        printf("Erro com a estrutura DadosSensor!\n");
        return -1;
    }    
    
    char nomeSensor[10];
    bool aindaTemSensor = true;
    
    //O objetivo desse laço é identificar os sensores e escrever no arquivo de forma ordenada por timestamp 
    while(aindaTemSensor == true){
        aindaTemSensor = false;
        int cont = 0;

        //Etapa 4.1 - Identfica se exite sensores na estrutura dados(Se o campo sensor está vazio, o laço ignora). Vai capturar o nome do primeiro sensor encontrado.
        for(int i=0; i<*tamanho; i++){
            if(strcmp(dados[i].sensor, "")!=0){
                strcpy(nomeSensor, dados[i].sensor);
                aindaTemSensor = true;
                break;
            }
        }

        //Se não existir sensores, o laço é interrompido
        if(aindaTemSensor == false) break;

        //Etapa 4.2 - Vai contar quantos nomes iguais existem dentro da estrutura dados. A ideia é fazer a alocação dinâmica na estrutura separar.
        for(int i = 0; i<*tamanho; i++){
            if(strcmp(dados[i].sensor, nomeSensor)==0) ++cont;
        }

        printf("Sensor %s com %d linhas!\n",nomeSensor, cont);

        DadosSensor *separar = malloc(cont * sizeof(DadosSensor));

        if(separar == NULL){
            printf("Erro na alocação de memória!\n");
            return -1;
        }

        int j = 0;
        
        //Etapa 4.3 - Esse laço vai copiar da estrutura dados para estrutura separar. Vão ser copiados aqueles que são iguais ao nome do sensor capturado.
        //Ao final, a posição copiada é zerada para que não haja o risco de de ocorrer o mesmo processo na etapa anterior  
        for(int i=0; i <*tamanho; i++){
            if(strcmp(nomeSensor, dados[i].sensor)==0){
                separar[j].timestamp = dados[i].timestamp;
                strcpy(separar[j].sensor, dados[i].sensor);
                strcpy(separar[j].medicao, dados[i].medicao);

                dados[i].timestamp = 0;
                strcpy(dados[i].sensor, "");
                strcpy(dados[i].medicao, "");

                ++j;
            }
        }

        //Etapa 4.4 - Vai ordenar a estrutura separar por ordem crescente 
        if(AlgoritmoOrdenacao(separar, &cont) != 0){
            printf("Erro ao fazer a ordenação da estrutura!\n");
            return -1;
        }

        //Etapa 4.5 - Vai gravar os dados da estrutura separar em um arquivo .txt com o nome do sensor
        if(GravarDados(separar, nomeSensor, &cont) != 0){
            printf("Erro ao gravar os dados!\n");
            return -1;
        }

        //Etapa 4.6 - Ordena os sensores da estrutura dados, onde os campos vazios vão para o final. Assim, os próximos laços for serão menores.
        for(int i=0; i <*tamanho; i++){
            unsigned timestampTemp = 0;
            char sensorTemp[10];
            char medicaoTemp[16]; 

            for(int j = i; j<*tamanho; j++){
                if(dados[i].timestamp == 0 && (strcmp(dados[i].sensor, "")==0)){
                    timestampTemp = dados[i].timestamp;
                    strcpy(sensorTemp, dados[i].sensor);
                    strcpy(medicaoTemp, dados[i].medicao);

                    dados[i].timestamp = dados[j].timestamp;
                    strcpy(dados[i].sensor, dados[j].sensor);
                    strcpy(dados[i].medicao, dados[j].medicao);

                    dados[j].timestamp = timestampTemp;
                    strcpy(dados[j].sensor, sensorTemp);
                    strcpy(dados[j].medicao, dados[i].medicao);
                }
            }
        }

        //Etapa 4.7 - Será subtraida da quantidade de linhas total  
        *tamanho-=cont;

        free(separar);
                
    } 

    return 0;
}

int CarregarDados(DadosSensor *dados, int *tamanho, char *argv[]){
    if(argv[1] == NULL){
        printf("Argumento inválido\n");
        return -1;
    }
    
    if(dados == NULL){
        printf("Erro na estrutura DadosSensor!\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Arquivo não encontrado!\n");
        return -1;
    }

    char frase[100];
    for(int i = 0; i<*tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *timestamp = strtok(frase, " ");
        char *sensor = strtok(NULL, " ");
        char *medicao = strtok(NULL, "\n");

        if(timestamp == NULL || sensor == NULL || medicao == NULL){
            printf("Linha %d ignorada!\n",i);
            continue;
        }

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        strncpy(dados[i].medicao, medicao, sizeof(dados->medicao));
        //dados[i].medicao = strtof(medicao, NULL);
    }

    fclose(file);

    return 0;
}

int Contador(char *argv[]){
    if(argv[1] == NULL){
        printf("Argumento inválido\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Arquivo não encontrado!\n");
        return -1;
    }

    char frase[100];
    int cont = 0;
    while(fgets(frase, sizeof(frase), file) != NULL) {
        ++cont;
    }
    fclose(file);

    if(cont == 0) return -1;

    return cont;
}

int AlgoritmoOrdenacao(DadosSensor *separar, int *tamanho){
    if(separar == NULL){
        printf("Erro na estrutura DadosSensor!\n");
        return -1;
    }

    long timestamp;
    char sensor[10];
    char medicao[17];

    for(int i=0; i<*tamanho; i++){
        for(int j = i+1; j<*tamanho ;j++){
            if(separar[i].timestamp<separar[j].timestamp){//Aqui ocorre a troca de ordem

                timestamp = separar[j].timestamp;
                strcpy(sensor, separar[j].sensor);
                strcpy(medicao, separar[j].medicao);

                separar[j].timestamp = separar[i].timestamp;
                strcpy(separar[j].sensor, separar[i].sensor);
                strcpy(separar[j].medicao, separar[i].medicao);

                separar[i].timestamp = timestamp;
                strcpy(separar[i].sensor, sensor);
                strcpy(separar[i].medicao, medicao);
            }
        }
    }

    return 0;
}

int GravarDados(DadosSensor *separar, char *sensor, int *tamanho){
    if(separar == NULL || sensor == NULL || *tamanho == 0){
        printf("Erro ao repassar os dados para a função GravarDados()!\n");
        return -1;
    }

    char nomeArquivo[20];
    char extensao[] = ".txt";

    strcpy(nomeArquivo, sensor);
    strcat(nomeArquivo, extensao);

    FILE *file = fopen(nomeArquivo, "r");
    if(file != NULL){
        printf("Arquivo %s já possui dados gravados!\n", nomeArquivo);
        return -1;
    }

    file = fopen(nomeArquivo, "w");
    for(int i = 0; i<*tamanho; i++){
        if(strcmp(separar[i].sensor, "") != 0) {
            fprintf(file,"%ld %s %s\n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
        }
    }

    fclose(file);

    printf("Arquivo %s criado com sucesso!\n", nomeArquivo);

    return 0;
}