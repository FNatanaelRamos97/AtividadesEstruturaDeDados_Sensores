#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(){
    char *sensores[] = {"FLUX", "TEMP", "PRES", "UMID", "VIBR"};
    char randomString[10]; // = malloc(sizeof(sensores)/sizeof(sensores[0])); 
    int size = sizeof(sensores)/sizeof(sensores[0]);
    float min = 0.0;
    float max = 100.0;
    time_t agora = time(NULL);
    time_t fim = agora - (5*365*24*60*60);//definido para cinco anos atrás

    float randomFloat;
    int randomIndex;
    time_t randomTimestamp;

    char nomeArquivo[50];
    char extensao[] = ".txt";

    while(1){
        printf("Digite o nome para o arquivo onde será guardado os valores aleatórios: ");
        fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
        
        nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';

        /*for(int i = 0; i < (sizeof(nomeArquivo)/sizeof(nomeArquivo[0])); i++){
            if(nomeArquivo[i] == '\n'){
                nomeArquivo[i] = '\0';
                break;
            }
        }*/

        strcat(nomeArquivo, extensao);

        FILE *file = fopen(nomeArquivo, "r");
        if(file != NULL){
            printf("Arquivo já possui dados gravados!\n");
            continue;
        }
        else{
            break;
        }
    }  
    
    FILE *file = fopen(nomeArquivo, "w");

    srand(time(NULL)); //incializar o gerador de números aleatórios

    for(int i = 0; i < 1000; i++){
        randomFloat = (float)rand() / RAND_MAX*(max - min)+min;
        randomIndex = rand() % size;
        strcpy(randomString,sensores[randomIndex]);
        randomTimestamp = agora + (time_t)((double)rand() / RAND_MAX * (fim - agora));

        //printf("%ld %s %.2f\n",randomTimestamp, randomString, randomFloat);
        fprintf(file,"%ld %s %.2f\n",randomTimestamp, randomString, randomFloat);
    }

    

    return 0;
}