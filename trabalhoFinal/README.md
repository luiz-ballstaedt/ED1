# Trabalho Final — Estrutura de Dados

**Disciplina:** Estrutura de Dados — 1º Semestre 2026

---

## Descrição

Programa em C++ que importa um arquivo `.txt` contendo departamentos e suas palavras-chave, e oferece opções de pesquisa por palavra-chave ou por nome de departamento.

## Formato do arquivo de entrada

Cada linha representa um departamento com 7 campos separados por espaço:

```
<código> <departamento> <kw1> <kw2> <kw3> <kw4> <kw5>
```

Exemplo:

```
001 FINANCEIRO CAIXA FLUXO ENTRADA SAÍDA DÉBITO
002 CONTABILIDADE BALANÇO BALANCETE DIÁRIO DEPRECIAÇÃO IMPOSTO
```

## Estrutura de dados

```cpp
struct Departamento {
    string codigo;
    string nome;
    string keywords[5];
};
```

Os registros são armazenados em um `vector<Departamento>`. Dois índices `unordered_map` (tabela hash) mapeiam keywords e nomes de departamento para seus respectivos registros, permitindo busca em O(1).

## Compilação

```bash
g++ -std=c++17 -o programa main.cpp
```

## Execução

```bash
./programa
```

O programa solicitará o caminho do arquivo de dados ao iniciar.

## Funcionalidades

| Opção | Descrição |
|-------|-----------|
| 1 | Busca por palavra-chave → exibe o departamento correspondente |
| 2 | Busca por departamento → exibe código e palavras-chave |
| 3 | Lista todos os departamentos ordenados por código |
| 0 | Encerra o programa |

## Observações

- Buscas são case-insensitive e aceitam entrada com ou sem acentuação
- A ordenação da listagem usa `std::sort` (baseado em quicksort — O(n log n))
