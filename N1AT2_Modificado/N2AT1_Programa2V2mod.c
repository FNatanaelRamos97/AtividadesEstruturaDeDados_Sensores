#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    long timestamp;
    char sensor[10];
    char medicao[17];
}DadosSensor; 

typedef struct{
    int dia, mes, ano, hora, minuto, segundo;
}DataHora;

time_t decompor_timestamp(DadosSensor *dados, DataHora *dataHora, int i);
time_t capturar_timestamp_valido(char *argv[]);
int buscaBinaria(DadosSensor *dados, long *timestamp, int fim);
int Contador(char *argv[]);
int CarregarDados(DadosSensor *dados, int *tamanho, char *argv[]);
void Encontrado(int *enc, long *timestamp, DadosSensor *dados,  DataHora *dataHora, int *tamanho);

int main(int argc, char * argv[]){

    //Primeira etapa - Bloco de tratamento de erro para entrada de argumentos
    if(argc != 3){
        printf("Numero de argumentos inválidos!\nÉ necessário 3 argumentos!\nPrograma encerrado!\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Arquivo inválido ou inexistente!\nDigite o nome do arquivo e a extensão .txt!\nPrograma encerrado\n");
        return -1;
    }
    fclose(file);
    printf("Nome do arquivo: %s\n", argv[1]);

    if (strlen(argv[2]) != 19) {
        printf("Formato de data/hora incompleto.\nUse: dd/mm/yyyy-hh:mm:ss\nPrograma encerrado!\n");
        return -1;
    }
    printf("Data e hora: %s\n", argv[2]);

    //Segunda etapa - Contar as linhas do arquivo para fazer a alocação dinâmica nas estruturas
    int cont = Contador(argv);

    if(cont <= 1){
        printf("Erro com a contagem de linhas do arquivo!\nPrograma encerrado!\n");
        return -1;
    }

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
    DataHora *dataHora = malloc(cont * sizeof(DataHora));

    //Terceira etapa - Carregar os dados do arquivo .txt para as estruturas dados e dataHora
    if(CarregarDados(dados, &cont, argv) != 0){
        printf("Erro ao carregar os dados!\nPrograma encerrado!\n");
        return -1;
    }

    int i = 0;
    if(dados[i].timestamp < dados[i+1].timestamp){
        printf("Arquivo em ordem crescente!\nPrograma encerrado!\n");
        return -1;
    }

    //Quarta etapa - Decompõe os timestamp da estrutura dados e carrega na estrutura dataHora
    for(int i=0; i<cont; i++){
        if(decompor_timestamp(dados, dataHora, i) != 0){
            printf("Erro ao decompor os timestamp!\n");
            return -1;
        };
        printf("%d/%d/%d %d:%d:%d %s %s\n",dataHora[i].dia, dataHora[i].mes, dataHora[i].ano, dataHora[i].hora, dataHora[i].minuto, dataHora[i].segundo, dados[i].sensor, dados[i].medicao);
    }

    //Quinta etapa - Captura o data e hora informado na linha de comando e transforma em timestamp válido  
    long timestamp = capturar_timestamp_valido(argv);
    if(timestamp == -1){
        printf("Erro ao carregar Timestamp válido!\nPrograma encerrado!\n");
        return -1;
    }

    if(timestamp > dados[i].timestamp || timestamp < dados[cont-1].timestamp){
        printf("Data/Hora digitado está fora do intervalo!\nPrograma encerrado!\n");
        return -1;
    }
    
    //Sexta etapa - Busca o valor do timestamp através de uma busca binária 
    int enc = buscaBinaria(dados, &timestamp, cont); 
    if(enc == -1){
        printf("Programa encerrado!\n");
        return -1;
    }
    //Sétima etapa - Imprime o valor encontrado. Se não encontrar, vai buscar e imprimir o valor mais próximo  
    Encontrado(&enc, &timestamp, dados, dataHora, &cont);

    //Oitava etapa - Libera as estruturas dados e data hora
    free(dados);
    free(dataHora);

    return 0;
}

time_t decompor_timestamp(DadosSensor *dados, DataHora *dataHora, int i) {
    if(dados == NULL){
        printf("Erro com a estrutura DadosSensor!\n");
        return -1;
    }

    if(dataHora == NULL){
        printf("Erro com a estrutura DataHora!\n");
        return -1;
    }

    time_t temp = dados[i].timestamp;
    struct tm *info_tempo = localtime(&temp);

    dataHora[i].ano = 1900 + info_tempo->tm_year;
    dataHora[i].mes = 1 + info_tempo->tm_mon;
    dataHora[i].dia = info_tempo->tm_mday;
    dataHora[i].hora = info_tempo->tm_hour;
    dataHora[i].minuto = info_tempo->tm_min;
    dataHora[i].segundo = info_tempo->tm_sec;

    return 0;
}

time_t capturar_timestamp_valido(char *argv[]){

    if(argv[2] == NULL){
        printf("Argumento inválido!\n");
        return -1;
    }

    char data_hora[30];
    strncpy(data_hora, argv[2], sizeof(data_hora));
    data_hora[sizeof(data_hora) - 1] = '\0';

    int dia = strtol(strtok(data_hora,"/-:"), NULL, 10);
    int mes = strtol(strtok(NULL,"/-:"), NULL, 10);
    int ano = strtol(strtok(NULL,"/-:"), NULL, 10);
    int hora = strtol(strtok(NULL,"/-:"), NULL, 10);
    int min = strtol(strtok(NULL,"/-:"), NULL, 10);
    int seg = strtol(strtok(NULL,"/-:"), NULL, 10);
    printf("%d/%d/%d-%d:%d:%d\n", dia, mes, ano, hora, min, seg);

    struct tm t;

    int maxDias = -1;
    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDias = 31;
            break;
        case 4: case 6: case 9: case 11:
            maxDias = 30;
            break;
        case 2:
            if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
                maxDias = 29;
            else
                maxDias = 28;
            break;
        default:
            maxDias = -1; // Mês inválido
    }

    if (dia < 1 || dia > maxDias || mes < 0 || mes > 12 || ano < 1900 ||  hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) {
        printf("Valores fora do intervalo permitido na data/hora: %d/%d/%d-%d:%d:%d\n", dia, mes, ano, hora, min, seg);
        return -1;
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
    }else return timestamp;
}

int buscaBinaria(DadosSensor *dados, long *timestamp, int fim){
    if(dados == NULL){
        printf("Erro com a estrutura DadosSensor!\n");
        return -1;
    }

    int inicio = 0; //naoEncontrado = fim+1;

    while(inicio <= fim){
        int meio = (inicio + fim)/2;

        if(dados[meio].timestamp == *timestamp) return meio;
        if(dados[meio].timestamp < *timestamp) inicio=meio+1;
        else fim = meio-1;
    }

    return -2;
}

int Contador(char *argv[]){
    if(argv[1] == NULL){
        printf("Argumento inválido!\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Arquivo não encontrado!\n");
        return -1;
    }

    int cont = 0;
    char frase[100];
    while(fgets(frase, sizeof(frase), file) != NULL) ++cont;

    return cont; 
}

int CarregarDados(DadosSensor *dados, int *tamanho, char *argv[]){

    if(dados == NULL){
        printf("Erro com a estrutura DadosSensor!\n");
        return -1;
    }

    if(argv[1] == NULL){
        printf("Argumento inválido!\n");
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

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        strncpy(dados[i].medicao, medicao,sizeof(dados->medicao));
    }

    fclose(file);

    return 0;
}

void Encontrado(int *enc, long *timestamp, DadosSensor *dados, DataHora *dataHora, int *tamanho){
    if(*enc == -2){
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

        printf("%d/%d/%d %d:%d:%d %s %s\n",dataHora[j].dia, dataHora[j].mes, dataHora[j].ano, dataHora[j].hora, dataHora[j].minuto, dataHora[j].segundo, dados[j].sensor, dados[j].medicao);
    
    }else{
        printf("Dados do sensor encontrado!\n");
        printf("%d/%d/%d %d:%d:%d %s %s\n",dataHora[*enc].dia, dataHora[*enc].mes, dataHora[*enc].ano, dataHora[*enc].hora, dataHora[*enc].minuto, dataHora[*enc].segundo, dados[*enc].sensor, dados[*enc].medicao);
    } 
}

