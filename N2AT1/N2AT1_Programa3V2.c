#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t ConverteDataHora(char *dataHora);
void MensagemErro();

int main(int argc, char *argv[]){
   if(argc != 6){
        printf("Numero de argumentos inválidos!\n");
        MensagemErro();
        return -1;
    }

    if(strlen(argv[1]) >= 20){
        printf("O nome do seu arquivo é igual ou maior a 20 caracteres!\n");
        printf("Programa encerrado!\n");
        return -1;
    }
    char nomeArquivo[20];
    char extencao[] = ".txt";
    strncpy(nomeArquivo, argv[1], sizeof(nomeArquivo));
    strcat(nomeArquivo, extencao);  

    FILE *file = fopen(nomeArquivo, "r");
    if(file != NULL){
        printf("Arquivo já possui dados gravados!\nPrograma encerrado!\n");
        return -1;
    }
    
    printf("Nome do arquivo: %s\n", nomeArquivo);

    if(strlen(argv[2]) != 19){
        printf("Formato de data/hora inicio incompleto.\nUse: dd/mm/yyyy-hh:mm:ss\nPrograma encerrado!\n");
        return -1;
    }

    if(strlen(argv[3]) != 19){
        printf("Formato de data/hora fim incompleto.\nUse: dd/mm/yyyy-hh:mm:ss\nPrograma encerrado!\n");
        return -1;
    }

    time_t inicio = ConverteDataHora(argv[2]);
    time_t fim = ConverteDataHora(argv[3]);
    if(inicio == -1 || fim == -1){
        printf("Programa encerrado!\n");
        return -1;
    }

    if(inicio >= fim){
        printf("A data de início não pode ser maior que a data do fim!\n");
        MensagemErro();
        printf("Programa encerrado!\n");
        return -1;
    }

    char *sensoresLinha = argv[4];
    char *sensores[50];
    int numSensores = 0;

    char *token = strtok(sensoresLinha, ",");
    while(token != NULL){
        sensores[numSensores++] = token;
        token = strtok(NULL, ",");
    }

    char *tipoLinha = argv[5];
    char *tipo[50];
    int numTipo = 0;

    token = strtok(tipoLinha, ",");
    while(token != NULL){
        if(strcmp(token, "CONJ_Z") != 0 && strcmp(token, "CONJ_Q") != 0 && strcmp(token, "TEXTO") != 0 && strcmp(token, "BINARIO") != 0){
            printf("O tipo \"%s\" não é reconhecido\n", token);
            MensagemErro();
            return -1;
        }

        tipo[numTipo++] = token;
        token = strtok(NULL, ",");
    }    

    if(numSensores != numTipo){
        printf("O numero de sensores deve ser igual ao numero de tipos!\nPrograma encerrado!\n");
        return -1;
    }

    file = fopen(nomeArquivo, "w");
    if(file == NULL){
        printf("Erro ao criar o arquivo!\nPrograma encerrado!\n");
        return -1;
    }

    srand(time(NULL));

    for (int i = 0; i < 2000; i++) {
        int s = rand() % numSensores; 
        time_t randomTimestamp = inicio + rand() % (fim - inicio + 1);
        
        if (strcmp(tipo[s], "CONJ_Z") == 0) {
            int valor = rand() % 1000;
            fprintf(file, "%ld %s %d\n", randomTimestamp, sensores[s], valor);
        }
        else if (strcmp(tipo[s], "CONJ_Q") == 0) {
            float valor = (float)rand() / RAND_MAX * 100.0;
            fprintf(file, "%ld %s %.2f\n", randomTimestamp, sensores[s], valor);
        }
        else if (strcmp(tipo[s], "BINARIO") == 0) {
            const char *valor = (rand() % 2 == 0) ? "true" : "false";
            fprintf(file, "%ld %s %s\n", randomTimestamp, sensores[s], valor);
        }
        else if (strcmp(tipo[s], "TEXTO") == 0) {
            char codigo[17];
            int len = rand() % 16 + 1;
            for (int j = 0; j < len; j++) {
                codigo[j] = 'A' + rand() % 26;
            }
            codigo[len] = '\0';
            fprintf(file, "%ld %s %s\n", randomTimestamp, sensores[s], codigo);
        }

    }

    printf("Arquivo criado com sucesso!\n");

    return 0;
}

time_t ConverteDataHora(char *dataHora){
    struct tm t;
    int dia = strtol(strtok(dataHora,"/-:"), NULL, 10);
    int mes = strtol(strtok(NULL,"/-:"), NULL, 10);
    int ano = strtol(strtok(NULL,"/-:"), NULL, 10);
    int hora = strtol(strtok(NULL,"/-:"), NULL, 10);
    int min = strtol(strtok(NULL,"/-:"), NULL, 10);
    int seg = strtol(strtok(NULL,"/-:"), NULL, 10);
    printf("%d/%d/%d-%d:%d:%d\n", dia, mes, ano, hora, min, seg);

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

void MensagemErro(){
    printf("A linha de comando deve possuir:\n1 - Nome do programa a ser executado.\n2 - Nome do arquivo a ser gerado sem a extensão.\n");
    printf("3 - Data e hora de inicio no formato especificado no exemplo abaixo.\n4 - Data e hora de fim no formato especificado no exemplo abaixo.\n5 - Nome dos sensores.\n6 - Nome dos tipos de dados.\n");
    printf("\nSegue o formato a seguir:\n");
    printf("N2AT1_Programa3V2 arquivo 25/12/2024-12:00:00 31/12/2024-12:00:00 TEMP,UMID,FLUX CONJ_Z,CONJ_Q,BINARIO\n");
    printf("CONJ_Z é int, CONJ_Q é float, TEXTO é string, BINARIO é bool.\n");
    printf("Programa encerrado!\n");
}