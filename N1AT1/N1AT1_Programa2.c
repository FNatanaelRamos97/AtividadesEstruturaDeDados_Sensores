#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    long timestamp;
    char sensor[10];
    float medicao;
}DadosSensor; 

typedef struct{
    int dia, mes, ano, hora, minuto, segundo;
}DataHora;

time_t decompor_timestamp(DadosSensor dados[], DataHora dataHora[], int i/*time_t timestamp, int *dia, int *mes, int *ano, int *hora, int *minuto, int *segundo*/) {

    struct tm *info_tempo = localtime(&dados[i].timestamp);

    dataHora[i].ano = 1900 + info_tempo->tm_year;
    dataHora[i].mes = 1 + info_tempo->tm_mon;
    dataHora[i].dia = info_tempo->tm_mday;
    dataHora[i].hora = info_tempo->tm_hour;
    dataHora[i].minuto = info_tempo->tm_min;
    dataHora[i].segundo = info_tempo->tm_sec;
}

time_t capturar_timestamp_valido() {
    int dia, mes, ano, hora, min, seg;
    struct tm t;

    while (1) {
        printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
        if (scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &min, &seg) != 6) {
            while (getchar() != '\n');
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || 
            hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) {
            printf("Valores fora dos limites aceitáveis.\n");
            continue;
        }
        

        t.tm_year = ano - 1900;
        t.tm_mon = mes - 1;
        t.tm_mday = dia;
        t.tm_hour = hora;
        t.tm_min = min;
        t.tm_sec = seg;
        t.tm_isdst = -1;

        time_t timestamp = mktime(&t);
        if (timestamp == -1) {
            printf("Data inválida. Tente novamente.\n");
        } else {
            return timestamp;
        }
    }
}

int buscaBinaria(DadosSensor dados[], long timestamp, int fim){
    int inicio = 0; //naoEncontrado = fim+1;

    while(inicio <= fim){
        int meio = (inicio + fim)/2;

        if(dados[meio].timestamp == timestamp) return meio;
        if(dados[meio].timestamp < timestamp) inicio=meio+1;
        else fim = meio-1;
    }

    return -1;
}

int main(){
    long timestamp;
    char sensor[15];
    char extensao[] = ".txt";
    char *listaSensor[] = {"FLUX", "TEMP", "PRES", "UMID", "VIBR"};
    int totalSensores = sizeof(listaSensor)/sizeof(listaSensor[0]);
    FILE *file;

    while(1){
        printf("Lista de sensores disponiveis.\n");

        for(int i = 0; i<totalSensores; i++) printf("%d - %s\n",i+1, listaSensor[i]);

        printf("Informe o nome sensor a ser pesquisado: ");
        scanf("%s", sensor);
        printf("\n");

        strcat(sensor, extensao);

        file = fopen(sensor,"r");

        if(file == NULL){
            printf("Arquivo inválido!\n");
            continue;
        }
        else break; 
    }

    int cont = 0;
    char ch;
    while(!feof(file)){
        ch = fgetc(file);
        if(ch == '\n') cont++;
    }
    rewind(file);

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
    DataHora *dataHora = malloc(cont * sizeof(DataHora));

    int i = 0;
    while(fscanf(file ,"%ld %s %f", &dados[i].timestamp, dados[i].sensor, &dados[i].medicao)!=EOF){
        //printf("%ld %s %.2f\n", dados[i].timestamp, dados[i].sensor, dados[i].medicao);
        i++;
    }

    int quantidade = i;

    for(int i=0; i<quantidade; i++){
        decompor_timestamp(dados, dataHora, i);
        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[i].dia, dataHora[i].mes, dataHora[i].ano, dataHora[i].hora, dataHora[i].minuto, dataHora[i].segundo, dados[i].sensor, dados[i].medicao);
    }

    timestamp = capturar_timestamp_valido();
    
    int enc = buscaBinaria(dados, timestamp, quantidade); //enc vai representar o valor encontra qu vai ser a posição do valor

    i=0;

    if(enc == -1){
        int j = 0; //j vai representar o indice_mais_proximo
        long minimaDiferenca = labs(timestamp - dados[0].timestamp);

        printf("Dados pesquisados não encontrados!\nValor do sensor mais próximo: ");
    
        for(int i = 1; i < quantidade; i++) {
        long diferencaAtual = labs(timestamp - dados[i].timestamp); //função labs - converte o numero negativo em positivo
        
            if(diferencaAtual < minimaDiferenca) {
                minimaDiferenca = diferencaAtual;
                j = i;
            }
        }    

        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[j].dia, dataHora[j].mes, dataHora[j].ano, dataHora[j].hora, dataHora[j].minuto, dataHora[j].segundo, dados[j].sensor, dados[j].medicao);
    
    }else{
        printf("Dados do sensor encontrado!\n");
        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[enc].dia, dataHora[enc].mes, dataHora[enc].ano, dataHora[enc].hora, dataHora[enc].minuto, dataHora[enc].segundo, dados[enc].sensor, dados[enc].medicao);
    } 


    free(dados);
    free(dataHora);
    //printf("%ld\n", timestamp);


    return 0;
}