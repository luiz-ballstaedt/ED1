#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

const int TOTAL_DATAS = 3;
const string NOME_ARQUIVO = "dados_professor.txt";

struct Data {
    int dia;
    int mes;
    int ano;
};

struct Pessoa {
    string nome;
    Data datas[TOTAL_DATAS];
};

struct No {
    Pessoa pessoa;
    No* prox;
};

Data lerData(const string& texto) {
    Data data;
    int anoCurto = stoi(texto.substr(4, 2));

    data.dia = stoi(texto.substr(0, 2));
    data.mes = stoi(texto.substr(2, 2));
    data.ano = (anoCurto <= 29) ? 2000 + anoCurto : 1900 + anoCurto;

    return data;
}

bool dataVemAntes(const Data& primeira, const Data& segunda) {
    if (primeira.ano != segunda.ano) {
        return primeira.ano < segunda.ano;
    }

    if (primeira.mes != segunda.mes) {
        return primeira.mes < segunda.mes;
    }

    return primeira.dia < segunda.dia;
}

void ordenarDatas(Data datas[]) {
    for (int i = 0; i < TOTAL_DATAS - 1; i++) {
        for (int j = 0; j < TOTAL_DATAS - 1 - i; j++) {
            if (dataVemAntes(datas[j + 1], datas[j])) {
                Data auxiliar = datas[j];
                datas[j] = datas[j + 1];
                datas[j + 1] = auxiliar;
            }
        }
    }
}

void inserirNoFim(No*& inicio, const Pessoa& pessoa) {
    No* novo = new No;
    novo->pessoa = pessoa;
    novo->prox = nullptr;

    if (inicio == nullptr) {
        inicio = novo;
        return;
    }

    No* atual = inicio;
    while (atual->prox != nullptr) {
        atual = atual->prox;
    }

    atual->prox = novo;
}

void ordenarListaPorNome(No* inicio) {
    if (inicio == nullptr) {
        return;
    }

    bool trocou;

    do {
        trocou = false;
        No* atual = inicio;

        while (atual->prox != nullptr) {
            if (atual->pessoa.nome > atual->prox->pessoa.nome) {
                Pessoa auxiliar = atual->pessoa;
                atual->pessoa = atual->prox->pessoa;
                atual->prox->pessoa = auxiliar;
                trocou = true;
            }

            atual = atual->prox;
        }
    } while (trocou);
}

void imprimirData(const Data& data) {
    cout << setfill('0') << setw(2) << data.dia << "/"
         << setw(2) << data.mes << "/" << data.ano;
}

bool lerPessoaDaLinha(const string& linha, Pessoa& pessoa) {
    if (linha.size() < 18) {
        return false;
    }

    int inicioDatas = static_cast<int>(linha.size()) - 18;
    pessoa.nome = linha.substr(0, inicioDatas);

    for (int i = 0; i < TOTAL_DATAS; i++) {
        string textoData = linha.substr(inicioDatas + i * 6, 6);
        pessoa.datas[i] = lerData(textoData);
    }

    return true;
}

void imprimirLista(const No* inicio) {
    const No* atual = inicio;

    while (atual != nullptr) {
        cout << "Nome: " << atual->pessoa.nome << endl;

        for (int i = 0; i < TOTAL_DATAS; i++) {
            cout << "  Data " << i + 1 << ": ";
            imprimirData(atual->pessoa.datas[i]);
            cout << endl;
        }

        cout << endl;
        atual = atual->prox;
    }
}

void liberarLista(No*& inicio) {
    while (inicio != nullptr) {
        No* apagar = inicio;
        inicio = inicio->prox;
        delete apagar;
    }
}

int main() {
    ifstream arquivo(NOME_ARQUIVO);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    No* lista = nullptr;
    string linha;

    while (getline(arquivo, linha)) {
        if (linha.empty()) {
            continue;
        }
        Pessoa pessoa;

        if (!lerPessoaDaLinha(linha, pessoa)) {
            continue;
        }

        ordenarDatas(pessoa.datas);
        inserirNoFim(lista, pessoa);
    }

    arquivo.close();
    ordenarListaPorNome(lista);

    cout << "=== Lista ordenada ===" << endl << endl;
    imprimirLista(lista);

    liberarLista(lista);
    return 0;
}
