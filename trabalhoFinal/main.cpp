#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

const int NUM_KEYWORDS = 5;

struct Departamento {
    string codigo;
    string nome;
    string keywords[NUM_KEYWORDS];
};

// Converte para uppercase e remove acentos (para uso nas chaves de busca)
string normalizar(const string& s) {
    string result;
    size_t i = 0;
    while (i < s.size()) {
        unsigned char c = s[i];
        if (c == 0xC3 && i + 1 < s.size()) {
            unsigned char c2 = (unsigned char)s[i + 1];
            char rep = 0;
            if      (c2 >= 0x80 && c2 <= 0x86) rep = 'A'; // À Á Â Ã Ä Å
            else if (c2 == 0x87)                rep = 'C'; // Ç
            else if (c2 >= 0x88 && c2 <= 0x8B) rep = 'E'; // È É Ê Ë
            else if (c2 >= 0x8C && c2 <= 0x8F) rep = 'I'; // Ì Í Î Ï
            else if (c2 == 0x91)                rep = 'N'; // Ñ
            else if (c2 >= 0x92 && c2 <= 0x96) rep = 'O'; // Ò Ó Ô Õ Ö
            else if (c2 >= 0x99 && c2 <= 0x9C) rep = 'U'; // Ù Ú Û Ü
            else if (c2 >= 0xA0 && c2 <= 0xA6) rep = 'A'; // à á â ã ä å
            else if (c2 == 0xA7)                rep = 'C'; // ç
            else if (c2 >= 0xA8 && c2 <= 0xAB) rep = 'E'; // è é ê ë
            else if (c2 >= 0xAC && c2 <= 0xAF) rep = 'I'; // ì í î ï
            else if (c2 == 0xB1)                rep = 'N'; // ñ
            else if (c2 >= 0xB2 && c2 <= 0xB6) rep = 'O'; // ò ó ô õ ö
            else if (c2 >= 0xB9 && c2 <= 0xBC) rep = 'U'; // ù ú û ü
            if (rep != 0) { result += rep; i += 2; continue; }
        }
        result += (char)toupper(c);
        i++;
    }
    return result;
}

bool carregarArquivo(const string& caminho,
                     vector<Departamento>& deps,
                     unordered_map<string, int>& indexPorKeyword,
                     unordered_map<string, int>& indexPorNome) {
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo \"" << caminho << "\".\n";
        return false;
    }

    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        istringstream ss(linha);
        Departamento dep;

        ss >> dep.codigo >> dep.nome;
        for (int i = 0; i < NUM_KEYWORDS; i++)
            ss >> dep.keywords[i];

        int idx = deps.size();
        deps.push_back(dep);

        indexPorNome[normalizar(dep.nome)] = idx;
        for (int i = 0; i < NUM_KEYWORDS; i++)
            indexPorKeyword[normalizar(dep.keywords[i])] = idx;
    }

    arquivo.close();
    return true;
}

void listarDepartamentos(const vector<Departamento>& deps) {
    for (const Departamento& dep : deps)
        cout << dep.codigo << " | " << dep.nome << "\n";
}

void buscarPorKeyword(const string& keyword,
                      const vector<Departamento>& deps,
                      const unordered_map<string, int>& indexPorKeyword) {
    auto it = indexPorKeyword.find(normalizar(keyword));
    if (it == indexPorKeyword.end()) {
        cout << "Palavra-chave \"" << keyword << "\" nao encontrada.\n";
        return;
    }
    const Departamento& dep = deps[it->second];
    cout << "Departamento: " << dep.nome << " (codigo: " << dep.codigo << ")\n";
}

void buscarPorDepartamento(const string& nome,
                           const vector<Departamento>& deps,
                           const unordered_map<string, int>& indexPorNome) {
    auto it = indexPorNome.find(normalizar(nome));
    if (it == indexPorNome.end()) {
        cout << "Departamento \"" << nome << "\" nao encontrado.\n";
        return;
    }
    const Departamento& dep = deps[it->second];
    cout << "\nCodigo: " << dep.codigo << "\n";
    cout << "Palavras-chave: ";
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        cout << dep.keywords[i];
        if (i < NUM_KEYWORDS - 1) cout << ", ";
    }
    cout << "\n";
}

int main() {
    string caminho;
    cout << "Informe o caminho do arquivo de dados: ";
    getline(cin, caminho);

    vector<Departamento> deps;
    // unordered_map usa tabela hash: busca em O(1) vs O(n) da busca linear no vetor
    unordered_map<string, int> indexPorKeyword;
    unordered_map<string, int> indexPorNome;

    if (!carregarArquivo(caminho, deps, indexPorKeyword, indexPorNome))
        return 1;

    // std::sort usa quicksort como base: O(n log n) na média, eficiente e in-place
    sort(deps.begin(), deps.end(), [](const Departamento& a, const Departamento& b) {
        return a.codigo < b.codigo;
    });

    cout << deps.size() << " departamento(s) carregado(s).\n\n";

    while (true) {
        cout << "=== MENU ===\n";
        cout << "1. Buscar por palavra-chave\n";
        cout << "2. Buscar por departamento\n";
        cout << "3. Listar todos os departamentos\n";
        cout << "0. Sair\n";
        cout << "Opcao: ";

        int opcao;
        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcao invalida. Digite um numero do menu.\n\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n";

        if (opcao == 1) {
            string keyword;
            cout << "Digite a palavra-chave: ";
            getline(cin, keyword);
            buscarPorKeyword(keyword, deps, indexPorKeyword);
        } else if (opcao == 2) {
            string nome;
            cout << "Digite o nome do departamento: ";
            getline(cin, nome);
            buscarPorDepartamento(nome, deps, indexPorNome);
        } else if (opcao == 3) {
            listarDepartamentos(deps);
        } else if (opcao == 0) {
            break;
        } else {
            cout << "Opcao invalida. Digite 0, 1, 2 ou 3.\n";
        }

        cout << "\n";
    }

    cout << "Encerrando.\n";
    return 0;
}
