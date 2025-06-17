# Documentação
## Primeiro código - Organização de dados
**Objetivo**<br>
É responsável por separar, ordenar e arquivar as medições. Cada arquivo gerado é de extensão ".txt". O arquivo terá o nome do sensor.

**Descrição**<br>
Ao escrever o nome do programa na linha de comando, é obrigratório escrever o nome do arquivo que contém as medições dos sensores. O exemplo abaixo demonstra como deve ser feito.

*./programa arquivo.txt*


## Segundo código - Programa de Busca de Medições por Timestamp
**Objetivo**<br>
Este programa é responsável por carregar dados de sensores a partir de um arquivo, decompor timestamps para um formato legível, e realizar uma busca binária para encontrar ou aproximar uma medição específica conforme a data e hora fornecidas na linha de comando.

**Descrição**<br>
Ao executar o programa, é obrigatório passar dois argumentos: o nome do arquivo que contém as medições e a data/hora no formato `dd/mm/yyyy-hh:mm:ss`. O exemplo abaixo demonstra como deve ser feito.

*./programa arquivo.txt 25/12/2024-14:30:00*


## Terceiro código - Gerador de dados aleatórios

**Objetivo**<br>
É responsável por gerar um arquivo de medições simuladas de sensores com dados aleatórios. O usuário informa o intervalo de datas, os nomes dos sensores e os tipos de dados que cada sensor gera. O arquivo gerado tem extensão ".txt".

**Descrição**<br>
Ao escrever o nome do programa na linha de comando, é obrigatório fornecer os seguintes argumentos:

1. Nome do arquivo a ser gerado (sem a extensão)
2. Data e hora de início no formato **dd/mm/yyyy-hh:mm:ss**
3. Data e hora de fim no formato **dd/mm/yyyy-hh:mm:ss**
4. Lista de sensores separados por vírgula
5. Lista de tipos de dados separados por vírgula (**CONJ_Z(int)**, **CONJ_Q(float)**, **BINARIO(bool)**, ou **TEXTO(string)**)

O exemplo abaixo demonstra como deve ser feito:

*./programa arquivo 01/01/2020-00:00:00 31/12/2024-23:59:59 TEMP,UMID,FLUX CONJ_Z,CONJ_Q,BINARIO*

