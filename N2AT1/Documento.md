# Documentação
## Primeiro código - Organização de dados
**Objetivo**<br>
É responsável por separar, ordenar e arquivar as medições. Cada arquivo gerado é de extensão ".txt". O arquivo terá o nome do sensor.

**Descrição**<br>
Ao escrever o nome do programa na linha de comando, é obrigratório escrever o nome do arquivo que contém as medições dos sensores. O exemplo abaixo demonstra como deve ser feito.

*./N2AT1_Programa1V3 dados.txt*

Caso a condição não seja satisteita, o programa é concluído com erro.

    ``` c
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
    ```
**Função Contador**<br>
Quando as condições são satifeitas, o código seguirá para a seguinte etapa abaixo:

    ``` c
    int cont = Contador(argv);
    if(cont == -1){
        printf("Erro ao contar as linhas do arquivo!\nPrograma encerrado!\n");
        return -1;
    }

    printf("Quantidade de linhas: %d\n", cont);

    DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
    ```
Esta função abre o arquivo de dados e conta o número total de linhas, retornando o valor para ser usado na alocação dinâmica da estrutura que armazenará os dados.<br>

    ```c
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
    ```
**Função CarregarDados**<br>
Após contar as linhas, os dados são carregados do arquivo para a estrutura dinâmica. Cada linha do arquivo é lida e seus dados separados em timestamp, sensor e medição, que são armazenados na estrutura `DadosSensor`.

```c
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

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        strncpy(dados[i].medicao, medicao, sizeof(dados->medicao));
    }

    fclose(file);

    return 0;
}
```

**Função SepararDados**<br>
Esta função identifica os sensores na estrutura, separa as medições correspondentes a cada sensor, ordena por timestamp e grava os dados ordenados em arquivos diferentes, um para cada sensor.

O processo ocorre em etapas:

- Identifica o primeiro sensor disponível na estrutura;
- Conta quantos registros pertencem a este sensor;
- Aloca memória para uma estrutura auxiliar e copia os dados do sensor;
- Apaga os registros copiados da estrutura original para evitar repetição;
- Ordena a estrutura auxiliar pelo timestamp;
- Grava os dados ordenados em um arquivo nomeado com o sensor;
- Reorganiza a estrutura original para mover os registros vazios ao final;
- Atualiza o número total de registros restantes.

Trecho que copia os dados do sensor para a estrutura auxiliar e limpa os registros na estrutura original:

```c
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
```

**Função AlgoritmoOrdenacao**<br>
Ordena a estrutura auxiliar contendo dados de um sensor por ordem crescente do timestamp.

```c
for(int i=0; i<*tamanho; i++){
    for(int j = i+1; j<*tamanho ;j++){
        if(separar[i].timestamp > separar[j].timestamp){
            long timestamp = separar[j].timestamp;
            char sensor[10];
            char medicao[17];

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
```

**Função GravarDados**<br>
Grava os dados ordenados da estrutura auxiliar em um arquivo com nome igual ao sensor e extensão ".txt". Se o arquivo já existir, a gravação é abortada.

```c
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
```

---

**Final do programa**<br>
Após todas as operações, o programa libera a memória alocada dinamicamente e finaliza a execução.

```c
free(dados);
return 0;
```

## Segundo código - Programa de Busca de Medições por Timestamp
**Objetivo**<br>
Este programa é responsável por carregar dados de sensores a partir de um arquivo, decompor timestamps para um formato legível, e realizar uma busca binária para encontrar ou aproximar uma medição específica conforme a data e hora fornecidas na linha de comando.

**Descrição**<br>
Ao executar o programa, é obrigatório passar dois argumentos: o nome do arquivo que contém as medições e a data/hora no formato `dd/mm/yyyy-hh:mm:ss`. O exemplo abaixo demonstra como deve ser feito.

*./programa dados.txt 25/12/2024-14:30:00*

Caso as condições de entrada não sejam satisfeitas, o programa é encerrado com mensagem de erro.

```c
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

if (strlen(argv[2]) != 19) {
    printf("Formato de data/hora incompleto.\nUse: dd/mm/yyyy-hh:mm:ss\nPrograma encerrado!\n");
    return -1;
}
```

**Função Contador**<br>
Quando as condições de entrada são satisfeitas, o programa conta a quantidade de linhas do arquivo para alocar dinamicamente os vetores que irão armazenar as medições e as datas/hora.

```c
int cont = Contador(argv);

if(cont <= 1){
    printf("Erro com a contagem de linhas do arquivo!\nPrograma encerrado!\n");
    return -1;
}

DadosSensor *dados = malloc(cont * sizeof(DadosSensor));
DataHora *dataHora = malloc(cont * sizeof(DataHora));
```

A função `Contador` abre o arquivo e retorna a quantidade de linhas para uso na alocação.

```c
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

    fclose(file);

    return cont; 
}
```

**Função CarregarDados**<br>
Em seguida, os dados do arquivo são carregados nas estruturas `DadosSensor` (com timestamp, sensor e medição).

```c
if(CarregarDados(dados, &cont, argv) != 0){
    printf("Erro ao carregar os dados!\nPrograma encerrado!\n");
    return -1;
}
```

A função `CarregarDados` realiza a leitura linha a linha, separa os campos e armazena na estrutura.

```c
int CarregarDados(DadosSensor *dados, int *tamanho, char *argv[]){
    if(dados == NULL || argv[1] == NULL){
        printf("Erro de argumento!\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        printf("Arquivo não encontrado!\n");
        return -1;
    }

    char frase[100];
    for(int i = 0; i < *tamanho; i++){
        fgets(frase, sizeof(frase), file);
        char *timestamp = strtok(frase, " ");
        char *sensor = strtok(NULL, " ");
        char *medicao = strtok(NULL, "\n");

        dados[i].timestamp = strtol(timestamp, NULL, 10);
        strncpy(dados[i].sensor, sensor, sizeof(dados->sensor));
        strncpy(dados[i].medicao, medicao, sizeof(dados->medicao));
    }

    fclose(file);

    return 0;
}
```

**Função decompor_timestamp**<br>
Cada timestamp em formato numérico é convertido para uma estrutura de data e hora legível.

```c
for(int i=0; i<cont; i++){
    if(decompor_timestamp(dados, dataHora, i) != 0){
        printf("Erro ao decompor os timestamp!\n");
        return -1;
    }
}
```

A função `decompor_timestamp` realiza essa conversão.

```c
time_t decompor_timestamp(DadosSensor *dados, DataHora *dataHora, int i) {
    if(dados == NULL || dataHora == NULL){
        printf("Erro nas estruturas!\n");
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
```

**Função capturar_timestamp_valido**<br>
O programa captura o argumento da data/hora informado na linha de comando e converte para um timestamp válido.

```c
long timestamp = capturar_timestamp_valido(argv);
if(timestamp == -1){
    printf("Erro ao carregar Timestamp válido!\nPrograma encerrado!\n");
    return -1;
}
```

A função executa a conversão e valida os valores.

```c
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

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || 
        hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) {
        printf("Valores fora dos limites aceitáveis.\n");
        return -1;
    }

    struct tm t = {0};
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
        return -1;
    }
    return timestamp;
}
```

**Função buscaBinaria**<br>
Realiza uma busca binária para encontrar o índice do timestamp informado.

```c
int enc = buscaBinaria(dados, &timestamp, cont); 
if(enc == -1){
    printf("Programa encerrado!\n");
    return -1;
}
```

A função busca o índice ou retorna -2 se não encontrar.

```c
int buscaBinaria(DadosSensor *dados, long *timestamp, int fim){
    if(dados == NULL){
        printf("Erro com a estrutura DadosSensor!\n");
        return -1;
    }

    int inicio = 0;

    while(inicio <= fim){
        int meio = (inicio + fim)/2;

        if(dados[meio].timestamp == *timestamp) return meio;
        if(dados[meio].timestamp < *timestamp) inicio=meio+1;
        else fim = meio-1;
    }

    return -2;
}
```

**Função Encontrado**<br>
Se o timestamp for encontrado, imprime o dado do sensor correspondente. Caso contrário, imprime o dado com o timestamp mais próximo.

```c
Encontrado(&enc, &timestamp, dados, dataHora, &cont);
```

Função para impressão do resultado.

```c
void Encontrado(int *enc, long *timestamp, DadosSensor *dados, DataHora *dataHora, int *tamanho){
    if(*enc == -2){
        int j = 0;
        long minimaDiferenca = labs(*timestamp - dados[0].timestamp);

        printf("Dados pesquisados não encontrados!\nValor do sensor mais próximo: ");

        for(int i = 1; i < *tamanho; i++) {
            long diferencaAtual = labs(*timestamp - dados[i].timestamp);
            if(diferencaAtual < minimaDiferenca) {
                minimaDiferenca = diferencaAtual;
                j = i;
            }
        }    

        printf("%d/%d/%d %d:%d:%d %s %s\n",dataHora[j].dia, dataHora[j].mes, dataHora[j].ano,
            dataHora[j].hora, dataHora[j].minuto, dataHora[j].segundo, dados[j].sensor, dados[j].medicao);

    }else{
        printf("Dados do sensor encontrado!\n");
        printf("%d/%d/%d %d:%d:%d %s %s\n",dataHora[*enc].dia, dataHora[*enc].mes, dataHora[*enc].ano,
            dataHora[*enc].hora, dataHora[*enc].minuto, dataHora[*enc].segundo, dados[*enc].sensor, dados[*enc].medicao);
    } 
}
```

**Liberação de memória**<br>
Ao final, as estruturas alocadas dinamicamente são liberadas.

```c
free(dados);
free(dataHora);
```


## Terceiro código - Gerador de dados aleatórios

**Objetivo**<br>
É responsável por gerar um arquivo de medições simuladas de sensores com dados aleatórios. O usuário informa o intervalo de datas, os nomes dos sensores e os tipos de dados que cada sensor gera. O arquivo gerado tem extensão ".txt".

**Descrição**<br>
Ao escrever o nome do programa na linha de comando, é obrigatório fornecer os seguintes argumentos:

1. Nome do arquivo a ser gerado (sem a extensão)
2. Data e hora de início no formato **dd/mm/yyyy-hh\:mm****:ss**
3. Data e hora de fim no formato **dd/mm/yyyy-hh\:mm****:ss**
4. Lista de sensores separados por vírgula
5. Lista de tipos de dados separados por vírgula (**int**, **float**, **bool**, ou **string**)

O exemplo abaixo demonstra como deve ser feito:

*./N2AT1\_Programa3V2 dados 01/01/2020-00:00:00 31/12/2024-23:59:59 TEMP,UMID,FLUX int,float,bool*

Caso as condições não sejam satisfeitas, o programa será encerrado com erro.

```c
if(argc != 6){
    printf("Numero de argumentos inválidos!\n");
    MensagemErro();
    return -1;
}
```

O programa verifica se o nome do arquivo possui menos de 20 caracteres, incluindo o nome sem a extensão.

```c
if(strlen(argv[1]) >= 20){
    printf("O nome do seu arquivo é igual ou maior a 20 caracteres!\n");
    printf("Programa encerrado!\n");
    return -1;
}
```

Em seguida, adiciona a extensão ".txt" ao nome do arquivo e verifica se o arquivo já existe. Se existir, o programa encerra para evitar sobrescrever arquivos anteriores.

```c
FILE *file = fopen(nomeArquivo, "r");
if(file != NULL){
    printf("Arquivo já possui dados gravados!\nPrograma encerrado!\n");
    return -1;
}
```

As datas de início e fim devem ter exatamente 19 caracteres e estar no formato correto. Caso contrário, o programa será encerrado.

```c
if(strlen(argv[2]) != 19){
    printf("Formato de data/hora inicio incompleto.\nUse: dd/mm/yyyy-hh:mm:ss\nPrograma encerrado!\n");
    return -1;
}
```

**Função ConverteDataHora()**<br>
Esta função tem como objetivo converter uma string contendo data e hora no formato **"dd/mm/yyyy-hh\:mm****:ss****"** para o tipo **time\_t** (timestamp). Isso facilita o tratamento e a comparação de datas dentro do programa.

Ela é chamada no seguinte trecho:

```c
time_t inicio = ConverteDataHora(argv[2]);
time_t fim = ConverteDataHora(argv[3]);
if(inicio == -1 || fim == -1){
    printf("Programa encerrado!\n");
    return -1;
}
```

Se o retorno for **-1**, significa que a data ou hora informada era inválida (exemplo: 31/02/2024 ou hora fora do intervalo permitido). O programa então será encerrado.

**Como a função trabalha:**

Ela divide a string de entrada usando delimitadores (**/**, **-**, e **:**), separando assim **dia**, **mês**, **ano**, **hora**, **minuto** e **segundo**.

```c
int dia = strtol(strtok(dataHora,"/-:"), NULL, 10);
int mes = strtol(strtok(NULL,"/-:"), NULL, 10);
int ano = strtol(strtok(NULL,"/-:"), NULL, 10);
int hora = strtol(strtok(NULL,"/-:"), NULL, 10);
int min = strtol(strtok(NULL,"/-:"), NULL, 10);
int seg = strtol(strtok(NULL,"/-:"), NULL, 10);
```

A função faz uma validação dos valores:

- Verifica se o **mês** está entre 1 e 12.
- Verifica o número máximo de **dias** permitido no mês, incluindo o ajuste para **anos bissextos (fevereiro com 29 dias)**.
- Verifica se **hora**, **minutos** e **segundos** estão dentro dos limites corretos.

Se algum valor estiver fora dos limites permitidos:

```c
printf("Valores fora do intervalo permitido na data/hora: %d/%d/%d-%d:%d:%d\n", dia, mes, ano, hora, min, seg);
return -1;
```

Se os dados forem válidos, a estrutura **struct tm** é preenchida e a função **mktime()** é usada para gerar o **timestamp**.

```c
t.tm_year = ano - 1900;
t.tm_mon = mes - 1;
t.tm_mday = dia;
t.tm_hour = hora;
t.tm_min = min;
t.tm_sec = seg;
t.tm_isdst = -1;

time_t timestamp = mktime(&t);
```

Se a conversão for bem-sucedida, a função retorna o **timestamp**. Caso contrário:

```c
if (timestamp == -1) {
    printf("Data inválida. Tente novamente.\n");
}
```

**Leitura dos sensores e tipos**<br>
As listas de sensores e tipos são separadas usando a função `strtok()` com base na vírgula.

```c
char *token = strtok(sensoresLinha, ",");
while(token != NULL){
    sensores[numSensores++] = token;
    token = strtok(NULL, ",");
}

token = strtok(tipoLinha, ",");
while(token != NULL){
    if(strcmp(token, "int") != 0 && strcmp(token, "float") != 0 && strcmp(token, "string") != 0 && strcmp(token, "bool") != 0){
        printf("O tipo \"%s\" não é reconhecido\n", token);
        printf("Programa encerrado!\n");
        return -1;
    }
    tipo[numTipo++] = token;
    token = strtok(NULL, ",");
}
```

O número de sensores deve obrigatoriamente ser igual ao número de tipos informados:

```c
if(numSensores != numTipo){
    printf("O numero de sensores deve ser igual ao numero de tipos!\nPrograma encerrado!\n");
    return -1;
}
```

**Geração dos dados aleatórios**<br>
Após todas as validações, o programa abre o arquivo para gravação. Em seguida, gera 2000 linhas com os seguintes formatos, dependendo do tipo de dado:

- **int:** Número inteiro aleatório entre 0 e 999.
- **float:** Número real aleatório de 0.00 a 100.00.
- **bool:** Valor textual **true** ou **false**.
- **string:** String de 1 a 16 caracteres aleatórios (somente letras maiúsculas).

Exemplo de geração para cada tipo:

```c
if (strcmp(tipo[s], "int") == 0) {
    int valor = rand() % 1000;
    fprintf(file, "%ld %s %d\n", randomTimestamp, sensores[s], valor);
}
else if (strcmp(tipo[s], "float") == 0) {
    float valor = (float)rand() / RAND_MAX * 100.0;
    fprintf(file, "%ld %s %.2f\n", randomTimestamp, sensores[s], valor);
}
else if (strcmp(tipo[s], "bool") == 0) {
    const char *valor = (rand() % 2 == 0) ? "true" : "false";
    fprintf(file, "%ld %s %s\n", randomTimestamp, sensores[s], valor);
}
else if (strcmp(tipo[s], "string") == 0) {
    char codigo[17];
    int len = rand() % 16 + 1;
    for (int j = 0; j < len; j++) {
        codigo[j] = 'A' + rand() % 26;
    }
    codigo[len] = '\0';
    fprintf(file, "%ld %s %s\n", randomTimestamp, sensores[s], codigo);
}
```

**Finalização**<br>
Se todas as etapas forem realizadas com sucesso, o programa exibe a seguinte mensagem:

```c
printf("Arquivo criado com sucesso!\n");
```

