#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

const int TOTAL_DATAS = 3;
const int MAX_PESSOAS = 100;
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

void ordenarPessoas(Pessoa pessoas[], int totalPessoas) {
    for (int i = 0; i < totalPessoas - 1; i++) {
        for (int j = 0; j < totalPessoas - 1 - i; j++) {
            if (pessoas[j].nome > pessoas[j + 1].nome) {
                Pessoa auxiliar = pessoas[j];
                pessoas[j] = pessoas[j + 1];
                pessoas[j + 1] = auxiliar;
            }
        }
    }
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

int main() {
    ifstream arquivo(NOME_ARQUIVO);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    Pessoa pessoas[MAX_PESSOAS];
    int totalPessoas = 0;
    string linha;

    while (getline(arquivo, linha) && totalPessoas < MAX_PESSOAS) {
        if (linha.empty()) {
            continue;
        }
        Pessoa pessoa;

        if (!lerPessoaDaLinha(linha, pessoa)) {
            continue;
        }

        ordenarDatas(pessoa.datas);
        pessoas[totalPessoas] = pessoa;
        totalPessoas++;
    }

    arquivo.close();
    ordenarPessoas(pessoas, totalPessoas);

    cout << "=== Lista ordenada ===" << endl << endl;

    for (int i = 0; i < totalPessoas; i++) {
        cout << "Nome: " << pessoas[i].nome << endl;

        for (int j = 0; j < TOTAL_DATAS; j++) {
            cout << "  Data " << j + 1 << ": ";
            imprimirData(pessoas[i].datas[j]);
            cout << endl;
        }

        cout << endl;
    }

    return 0;
}

