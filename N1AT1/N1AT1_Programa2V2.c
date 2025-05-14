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

time_t decompor_timestamp(DadosSensor *dados, DataHora *dataHora, int i);
time_t capturar_timestamp_valido();
int buscaBinaria(DadosSensor *dados, long *timestamp, int fim);
int Contador(char *sensor);
int CarregarDados(DadosSensor *dados, int *tamanho, char *sensor);
void Encontrado(int *enc, long *timestamp, DadosSensor *dados,  DataHora *dataHora, int *tamanho);
char *BuscaSensor();

int main(){
    char *sensor = BuscaSensor();
    if(sensor == NULL){
        printf("Erro ao buscar o sensor!\nPrograma encerrado!");
        return -1;
    }
    
    int cont = Contador(sensor);
    if(cont <= 1){
        printf("Erro com a contagem de linhas do arquivo!\nPrograma encerrado!");
        return -1;
    }

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
    DataHora *dataHora = malloc(cont * sizeof(DataHora));

    if(CarregarDados(dados, &cont, sensor) != 0){
        printf("Erro ao carregar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    for(int i=0; i<cont; i++){
        decompor_timestamp(dados, dataHora, i);
        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[i].dia, dataHora[i].mes, dataHora[i].ano, dataHora[i].hora, dataHora[i].minuto, dataHora[i].segundo, dados[i].sensor, dados[i].medicao);
    }

    long timestamp = capturar_timestamp_valido();
    
    int enc = buscaBinaria(dados, &timestamp, cont); //enc vai representar o valor encontra qu vai ser a posição do valor

    Encontrado(&enc, &timestamp, dados, dataHora, &cont);

    free(dados);
    free(dataHora);
    //printf("%ld\n", timestamp);

    return 0;
}

time_t decompor_timestamp(DadosSensor *dados, DataHora *dataHora, int i) {
    time_t temp = dados[i].timestamp;
    struct tm *info_tempo = localtime(&temp);

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

int buscaBinaria(DadosSensor *dados, long *timestamp, int fim){
    int inicio = 0; //naoEncontrado = fim+1;

    while(inicio <= fim){
        int meio = (inicio + fim)/2;

        if(dados[meio].timestamp == *timestamp) return meio;
        if(dados[meio].timestamp < *timestamp) inicio=meio+1;
        else fim = meio-1;
    }

    return -1;
}

int Contador(char *sensor){

    if(sensor == NULL){
        printf("Nome do sensor inválido!\n");
        return -1;
    }

    FILE *file = fopen(sensor, "r");
    if(file == NULL) return -1;

    int cont = 0;
    char frase[100];
    while(fgets(frase, sizeof(frase), file) != NULL) ++cont;

    return cont; 
}

int CarregarDados(DadosSensor *dados, int *tamanho, char *sensor){

    if(dados == NULL) return -1;
    if(sensor == NULL) return -1;

    FILE *file = fopen(sensor, "r");
    if(file == NULL) return -1;

    char frase[100];
    for(int i = 0; i<*tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *timestamp = strtok(frase, " ");
        char *sensor = strtok(NULL, " ");
        char *medicao = strtok(NULL, "\n");

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        dados[i].medicao = strtof(medicao, NULL);
    }

    fclose(file);

    return 0;
}

void Encontrado(int *enc, long *timestamp, DadosSensor *dados, DataHora *dataHora,int *tamanho){
    if(*enc == -1){
        int j = 0; //j vai representar o indice_mais_proximo
        long minimaDiferenca = labs(*timestamp - dados[0].timestamp);

        printf("Dados pesquisados não encontrados!\nValor do sensor mais próximo: ");
    
        for(int i = 1; i < *tamanho; i++) {
        long diferencaAtual = labs(*timestamp - dados[i].timestamp); //função labs - converte o numero negativo em positivo
        
            if(diferencaAtual < minimaDiferenca) {
                minimaDiferenca = diferencaAtual;
                j = i;
            }
        }    

        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[j].dia, dataHora[j].mes, dataHora[j].ano, dataHora[j].hora, dataHora[j].minuto, dataHora[j].segundo, dados[j].sensor, dados[j].medicao);
    
    }else{
        printf("Dados do sensor encontrado!\n");
        printf("%d/%d/%d %d:%d:%d %s %.2f\n",dataHora[*enc].dia, dataHora[*enc].mes, dataHora[*enc].ano, dataHora[*enc].hora, dataHora[*enc].minuto, dataHora[*enc].segundo, dados[*enc].sensor, dados[*enc].medicao);
    } 
}

char *BuscaSensor(){
    static char sensor[15];
    char extensao[] = ".txt";
    char *listaSensor[] = {"FLUX", "TEMP", "PRES", "UMID", "VIBR"};
    int totalSensores = sizeof(listaSensor)/sizeof(listaSensor[0]);
    FILE *file;

    while(1){
        printf("Lista de sensores disponiveis.\n");

        for(int i = 0; i<totalSensores; i++) printf("%d - %s\n",i+1, listaSensor[i]);

        printf("Informe o nome sensor a ser pesquisado: ");
        fgets(sensor, sizeof(sensor), stdin);
        sensor[strcspn(sensor, "\n")] = '\0';
        //scanf("%s", sensor);
        printf("\n");

        strcat(sensor, extensao);

        file = fopen(sensor,"r");

        if(file == NULL){
            printf("Arquivo inválido!\n");
            continue;
        }
        else{
            fclose(file);
            return sensor;
        }  
    }

    return NULL;
}