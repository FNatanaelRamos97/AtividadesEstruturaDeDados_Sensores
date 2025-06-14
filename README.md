 # Atividades de Estrutura de Dados (Sensores)

**Atividade**

Uma planta industrial inteligente realiza medições constantes por meio de sensores distribuídos na linha de produção. As leituras desses sensores são registradas sequencialmente em um arquivo bruto, sem distinção de tipo de sensor nem garantia de ordem temporal.

Com o crescimento do volume de dados, tornou-se necessário implementar um sistema que permita consultas rápidas e precisas com base em instantes específicos de tempo.

Como desenvolvedor de software, você  é responsável por implementar 3 programas para este sistema:

**Programa 1 – Organização dos Dados**

O primeiro programa deve processar um arquivo contendo diversas leituras no seguinte formato:

 TIMESTAMP, ID_SENSOR, VALOR

**Onde os campos representam**
TIMESTAMP: data e hora da medida no formato unix epoch.
ID_SENSOR: string sem espaço que representa cada sensor
VALOR: Valor numérico em ponto flutuante que representa a medida fornecida pelo sensor.
Essas leituras referem-se a diferentes sensores, que aparecem misturados ao longo do arquivo.

**Esse programa deverá:**
- Identificar os diferentes sensores presentes.
- Separar os dados em arquivos distintos para cada sensor (ex: TEMP.txt, PRES.txt, VIBR.txt).
- Em cada arquivo, ordenar as leituras por timestamp em ordem crescente.

A ordenação dos dados dentro de cada arquivo não é opcional: ela é necessária para permitir o uso de busca binária no processo de consulta descrito no Programa 2.

**Programa 2 – Consulta por Instante**

O segundo programa permitirá que o operador informe:
- O nome de um sensor (por exemplo, TEMP)
- Uma data e hora da medida consultada
- O programa deve então localizar, usando busca binária, a leitura registrada com timestamp mais próximo daquele instante no arquivo correspondente ao sensor, e exibir as informações da leitura.
- A busca binária é obrigatória para garantir desempenho adequado em grandes volumes de dados.

**Programa 3 – Geração de Arquivo de Teste**

Crie um programa adicional que gere um arquivo de teste com as seguintes características:
- O programa deve receber a data (dia, mês e ano) como entrada do usuário.
- O programa deve gerar 5 tipos diferentes de sensores (ex: TEMP, PRES, VIBR, UMID, FLUX).

Para cada sensor, o programa deve criar 1000 leituras aleatórias com valores numéricos aleatórios e timestamps sorteados dentro da data fornecida pelo usuário.

O arquivo gerado deve seguir o formato: TIMESTAMP, ID_SENSOR, VALOR.

TIMESTAMP: data e hora da medida no formato unix epoch.

ID_SENSOR: string sem espaço que representa cada sensor.

VALOR: Valor numérico em ponto flutuante que representa a medida fornecida pelo sensor.
