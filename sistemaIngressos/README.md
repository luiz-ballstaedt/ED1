# Sistema de Venda de Ingressos

Este trabalho mostra uma proposta de estruturas de dados para um sistema de venda de ingressos com reconhecimento facial, parentesco entre usuários, revista e geração de arquivos.

## Estruturas usadas

Foram usadas 6 estruturas principais:

1. `Array` para os 70 pontos faciais.
2. `Lista encadeada` para os usuários.
3. `Fila` para a ordem da revista.
4. `Lista encadeada` para as relações de parentesco.
5. `Lista encadeada` para as ocorrências.
6. `Lista encadeada` para os registros enviados à Polícia Federal.

## Justificativa

### 1. Pontos faciais

Foi usado um array porque a quantidade de pontos é fixa. Como o sistema sempre captura 70 pontos e cada um ocupa 1 byte, o array atende bem.

### 2. Usuários

Foi usada uma lista encadeada porque a quantidade de usuários não é conhecida antes. Assim, é possível cadastrar novos usuários dinamicamente.

### 3. Revista

Foi usada uma fila porque os usuários são chamados para revista pela ordem do ingresso. Ou seja, o primeiro a entrar é o primeiro a ser chamado.

### 4. Parentesco

Foi usada uma lista encadeada de relações. O motivo é que o enunciado só fala que pode haver parentesco entre usuários, mas não diz que precisa ser uma árvore ou hierarquia.

### 5. Ocorrências

Foi usada uma lista encadeada para guardar as ocorrências que aparecerem durante a execução do sistema e depois gerar o arquivo.

### 6. Registros da Polícia Federal

Também foi usada uma lista encadeada, porque a quantidade de registros depende de quantos usuários passaram pela revista.

## Dados de cada estrutura

### `PontosFaciais`

- `pontos[70]`: guarda os 70 pontos da face

### `Usuario`

- `numeroIngresso`: número do ingresso
- `cpf`: CPF do usuário
- `nome`: nome do usuário
- `face`: dados faciais
- `revistado`: informa se passou pela revista
- `proximo`: ponteiro para o próximo usuário

### `ElementoFila`

- `numeroIngresso`: número do ingresso na fila
- `proximo`: ponteiro para o próximo elemento

### `RelacaoParentesco`

- `cpfUsuario1`: CPF do primeiro usuário
- `nomeUsuario1`: nome do primeiro usuário
- `cpfUsuario2`: CPF do segundo usuário
- `nomeUsuario2`: nome do segundo usuário
- `tipo`: tipo de parentesco
- `proximo`: ponteiro para a próxima relação

### `Ocorrencia`

- `numeroIngresso`: ingresso ligado à ocorrência
- `cpf`: CPF do usuário
- `descricao`: descrição da ocorrência
- `face`: dados faciais do usuário
- `proximo`: ponteiro para a próxima ocorrência

### `RegistroPF`

- `cpf`: CPF do usuário
- `face`: dados da face
- `proximo`: ponteiro para o próximo registro

## Como executar

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o sistema_ingressos sistema.cpp
./sistema_ingressos
```

Ao final, o programa gera os arquivos:

- `dados_policia_federal.txt`
- `ocorrencias.txt`
