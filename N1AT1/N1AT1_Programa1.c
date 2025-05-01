#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    long timestamp;
    char sensor[10];
    float medicao;
}DadosSensor;

void AlgoritmoOrdenacao(DadosSensor separar[], int quantidade){
    long timestamp;
    char sensor[10];
    float medicao;

    for(int i=0; i<quantidade; i++){
        for(int j = i+1; j<quantidade ;j++){
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
        //printf("%ld %s %.2f \n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
    }

    //printf("%s\n", sensor);

    char nomeArquivo[10];
    char extensao[] = ".txt";
    FILE *fl;

    strcpy(nomeArquivo, sensor);

    //printf("%s\n", nomeArquivo);

    strcat(nomeArquivo, extensao);

    //printf("%s\n", nomeArquivo);

    fl = fopen(nomeArquivo, "w");

    for(int i = 0; i<quantidade; i++){
        if(strcmp(separar[i].sensor, "")!=0) {
            fprintf(fl,"%ld %s %.2f \n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
            //printf("%ld %s %.2f \n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
        }
    }

    fclose(fl);


}



int main(){

    FILE *file = fopen("dadosAleatorios.txt", "r");

    if(file == NULL) return 1;

    int count = 0;
    char ch;
    while(!feof(file)) {
        ch = fgetc(file);
        if(ch == '\n') count++;
    }
    rewind(file); //Equivalente a fseek(file, 0, SEEK_SET)

    //printf("%d\n", count);


    //fseek(file, 0, SEEK_END);    // vai para o fim do arquivo
    //long tamanho = ftell(file);  // pega a posição atual (tamanho do arquivo)*/
    //fseek(file, 0, SEEK_SET);    // volta para o início do arquivo

    //size_t n = (tamanho / sizeof(DadosSensor)); // Tamanho do arquivo / pelo tamanho normal da struct == 24 bytes

    //printf("%d %d\n", n, tamanho); 

    DadosSensor *dados = malloc(count * sizeof(DadosSensor));
    DadosSensor *separar = malloc(count * sizeof(DadosSensor));

    int i = 0;

    while(fscanf(file, "%ld %s %f", &dados[i].timestamp, &dados[i].sensor, &dados[i].medicao) != EOF){

        //printf("%ld %s %.2f \n", dados[i].timestamp, dados[i].sensor, dados[i].medicao);

        ++i;
    }

    fclose(file);


    char nomeSensor[10];

    //strcpy(nomeSensor, dados[0].sensor);

    int quantidade = i;

    int aindaTemSensor = 1;

    while(aindaTemSensor){
        aindaTemSensor = 0;

        for(int i=0; i<quantidade; i++){
            if(strcmp(dados[i].sensor, "")!=0){
                strcpy(nomeSensor, dados[i].sensor);
                aindaTemSensor = 1;
                break;
            }

        }

        if(!aindaTemSensor) break;
        

        for(int i=0; i < quantidade; i++){
            if(strcmp(nomeSensor, dados[i].sensor)==0){
                separar[i].timestamp = dados[i].timestamp;
                strcpy(separar[i].sensor, dados[i].sensor);
                separar[i].medicao = dados[i].medicao;

                dados[i].timestamp = 0;
                strcpy(dados[i].sensor, "");
                dados[i].medicao = 0;

                //printf("%ld %s %.2f \n", separar[i].timestamp, separar[i].sensor, separar[i].medicao);
            }
        }

        AlgoritmoOrdenacao(separar, quantidade);

        for(int i=0; i < quantidade; i++){
                separar[i].timestamp = 0;
                strcpy(separar[i].sensor, "");
                separar[i].medicao = 0;
            }
        
        
    }    
    

    free(dados);
    free(separar);

    return 0;
}