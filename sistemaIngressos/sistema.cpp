#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

// ============================================================
// ESTRUTURA 1: Array estatico - Pontos Faciais
// Justificativa: tamanho fixo (70 bytes), acesso O(1) por indice
// ============================================================
struct PontosFaciais {
    unsigned char pontos[70]; // 70 pontos, cada um 1 byte
};

// ============================================================
// ESTRUTURA 2: Lista Encadeada - Cadastro de Usuarios
// Justificativa: quantidade de usuarios desconhecida previamente,
// crescimento dinamico sem realocacao de memoria
// ============================================================
struct Usuario {
    int numeroIngresso;
    char cpf[12];          // CPF com 11 digitos + '\0'
    char nome[100];
    PontosFaciais face;
    bool revistado;
    Usuario* proximo;
};

struct ListaUsuarios {
    Usuario* cabeca;
    int quantidade;
};

// ============================================================
// ESTRUTURA 3: Fila - Chamada para Revista
// Justificativa: usuarios sao chamados por ordem de compra
// do ingresso (FIFO - First In, First Out)
// ============================================================
struct ElementoFila {
    int numeroIngresso;
    ElementoFila* proximo;
};

struct FilaRevista {
    ElementoFila* frente;
    ElementoFila* tras;
    int tamanho;
};

// ============================================================
// ESTRUTURA 4: Lista Encadeada - Relacoes de Parentesco
// Justificativa: o enunciado pede apenas o registro de parentesco
// entre usuarios, sem impor hierarquia fixa
// ============================================================
struct RelacaoParentesco {
    char cpfUsuario1[12];
    char nomeUsuario1[100];
    char cpfUsuario2[12];
    char nomeUsuario2[100];
    char tipo[40];
    RelacaoParentesco* proximo;
};

struct ListaParentescos {
    RelacaoParentesco* cabeca;
    int quantidade;
};

// ============================================================
// ESTRUTURA 5: Lista Encadeada - Ocorrencias
// Justificativa: ocorrencias sao registradas conforme aparecem
// e depois percorridas para gerar arquivo
// ============================================================
struct Ocorrencia {
    int numeroIngresso;
    char cpf[12];
    char descricao[256];
    PontosFaciais face;
    Ocorrencia* proximo;
};

struct ListaOcorrencias {
    Ocorrencia* cabeca;
    Ocorrencia* cauda;
    int quantidade;
};

// ============================================================
// ESTRUTURA 6: Lista - Registros para Policia Federal
// Justificativa: agrupa face + CPF para envio em arquivo,
// quantidade dinamica de registros
// ============================================================
struct RegistroPF {
    char cpf[12];
    PontosFaciais face;
    RegistroPF* proximo;
};

struct ListaRegistrosPF {
    RegistroPF* cabeca;
    RegistroPF* cauda;
    int quantidade;
};

// ============================================================
// FUNCOES - Lista de Usuarios
// ============================================================

void inicializarListaUsuarios(ListaUsuarios &lista) {
    lista.cabeca = NULL;
    lista.quantidade = 0;
}

// Simula a captura de 70 pontos faciais
PontosFaciais capturarReconhecimentoFacial() {
    PontosFaciais face;
    for (int i = 0; i < 70; i++) {
        face.pontos[i] = (unsigned char)(rand() % 256);
    }
    return face;
}

Usuario* inserirUsuario(ListaUsuarios &lista, const char* cpf, const char* nome) {
    Usuario* novo = new Usuario;

    lista.quantidade++;
    novo->numeroIngresso = lista.quantidade;
    strncpy(novo->cpf, cpf, 11);
    novo->cpf[11] = '\0';
    strncpy(novo->nome, nome, 99);
    novo->nome[99] = '\0';
    novo->face = capturarReconhecimentoFacial();
    novo->revistado = false;
    novo->proximo = lista.cabeca;
    lista.cabeca = novo;

    cout << "[VENDA] Ingresso #" << novo->numeroIngresso
         << " gerado para " << novo->nome
         << " (CPF: " << novo->cpf << ")" << endl;
    cout << "        Reconhecimento facial capturado (70 pontos)." << endl;

    return novo;
}

Usuario* buscarUsuarioPorIngresso(ListaUsuarios &lista, int numero) {
    Usuario* atual = lista.cabeca;
    while (atual != NULL) {
        if (atual->numeroIngresso == numero) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void liberarListaUsuarios(ListaUsuarios &lista) {
    Usuario* atual = lista.cabeca;
    while (atual != NULL) {
        Usuario* temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    lista.cabeca = NULL;
    lista.quantidade = 0;
}

// ============================================================
// FUNCOES - Fila de Revista
// ============================================================

void inicializarFila(FilaRevista &fila) {
    fila.frente = NULL;
    fila.tras = NULL;
    fila.tamanho = 0;
}

void enfileirar(FilaRevista &fila, int numeroIngresso) {
    ElementoFila* novo = new ElementoFila;
    novo->numeroIngresso = numeroIngresso;
    novo->proximo = NULL;

    if (fila.tras == NULL) {
        fila.frente = novo;
        fila.tras = novo;
    } else {
        fila.tras->proximo = novo;
        fila.tras = novo;
    }
    fila.tamanho++;
    cout << "[FILA]  Ingresso #" << numeroIngresso
         << " adicionado a fila de revista." << endl;
}

int desenfileirar(FilaRevista &fila) {
    if (fila.frente == NULL) {
        cout << "[FILA]  Fila vazia!" << endl;
        return -1;
    }
    ElementoFila* temp = fila.frente;
    int numero = temp->numeroIngresso;
    fila.frente = fila.frente->proximo;
    if (fila.frente == NULL) {
        fila.tras = NULL;
    }
    delete temp;
    fila.tamanho--;
    return numero;
}

void liberarFila(FilaRevista &fila) {
    while (fila.frente != NULL) {
        desenfileirar(fila);
    }
}

// ============================================================
// FUNCOES - Lista de Parentesco
// ============================================================

void inicializarListaParentescos(ListaParentescos &lista) {
    lista.cabeca = NULL;
    lista.quantidade = 0;
}

void inserirRelacaoParentesco(ListaParentescos &lista,
                              const char* cpf1, const char* nome1,
                              const char* cpf2, const char* nome2,
                              const char* tipo) {
    RelacaoParentesco* nova = new RelacaoParentesco;
    strncpy(nova->cpfUsuario1, cpf1, 11);
    nova->cpfUsuario1[11] = '\0';
    strncpy(nova->nomeUsuario1, nome1, 99);
    nova->nomeUsuario1[99] = '\0';
    strncpy(nova->cpfUsuario2, cpf2, 11);
    nova->cpfUsuario2[11] = '\0';
    strncpy(nova->nomeUsuario2, nome2, 99);
    nova->nomeUsuario2[99] = '\0';
    strncpy(nova->tipo, tipo, 39);
    nova->tipo[39] = '\0';

    nova->proximo = lista.cabeca;
    lista.cabeca = nova;
    lista.quantidade++;

    cout << "[PARENTESCO] " << nova->nomeUsuario1
         << " e " << nova->nomeUsuario2
         << " registrados como " << nova->tipo << endl;
}

void imprimirParentescos(const ListaParentescos &lista) {
    const RelacaoParentesco* atual = lista.cabeca;
    while (atual != NULL) {
        cout << "- " << atual->nomeUsuario1 << " (" << atual->cpfUsuario1 << ")"
             << " <-> " << atual->nomeUsuario2 << " (" << atual->cpfUsuario2 << ")"
             << " | Relacao: " << atual->tipo << endl;
        atual = atual->proximo;
    }
}

void liberarListaParentescos(ListaParentescos &lista) {
    RelacaoParentesco* atual = lista.cabeca;
    while (atual != NULL) {
        RelacaoParentesco* temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    lista.cabeca = NULL;
    lista.quantidade = 0;
}

// ============================================================
// FUNCOES - Registros para Policia Federal
// ============================================================

void inicializarListaPF(ListaRegistrosPF &lista) {
    lista.cabeca = NULL;
    lista.cauda = NULL;
    lista.quantidade = 0;
}

void inserirRegistroPF(ListaRegistrosPF &lista, const char* cpf, PontosFaciais face) {
    RegistroPF* novo = new RegistroPF;
    strncpy(novo->cpf, cpf, 11);
    novo->cpf[11] = '\0';
    novo->face = face;
    novo->proximo = NULL;

    if (lista.cauda == NULL) {
        lista.cabeca = novo;
        lista.cauda = novo;
    } else {
        lista.cauda->proximo = novo;
        lista.cauda = novo;
    }
    lista.quantidade++;
}

void gerarArquivoPF(ListaRegistrosPF &lista) {
    ofstream arquivo("dados_policia_federal.txt");
    if (!arquivo.is_open()) {
        cout << "[ERRO] Nao foi possivel criar arquivo para PF." << endl;
        return;
    }

    RegistroPF* atual = lista.cabeca;
    int contador = 1;
    while (atual != NULL) {
        arquivo << "=== Registro " << contador << " ===" << endl;
        arquivo << "CPF: " << atual->cpf << endl;
        arquivo << "Pontos faciais: ";
        for (int i = 0; i < 70; i++) {
            arquivo << (int)atual->face.pontos[i];
            if (i < 69) arquivo << ",";
        }
        arquivo << endl << endl;
        atual = atual->proximo;
        contador++;
    }

    arquivo.close();
    cout << "[PF]    Arquivo 'dados_policia_federal.txt' gerado com "
         << lista.quantidade << " registro(s)." << endl;
}

void liberarListaPF(ListaRegistrosPF &lista) {
    RegistroPF* atual = lista.cabeca;
    while (atual != NULL) {
        RegistroPF* temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    lista.cabeca = NULL;
    lista.cauda = NULL;
    lista.quantidade = 0;
}

// ============================================================
// FUNCOES - Lista de Ocorrencias
// ============================================================

void inicializarListaOcorrencias(ListaOcorrencias &lista) {
    lista.cabeca = NULL;
    lista.cauda = NULL;
    lista.quantidade = 0;
}

void inserirOcorrencia(ListaOcorrencias &lista, int numeroIngresso,
                       const char* cpf, const char* descricao, PontosFaciais face) {
    Ocorrencia* nova = new Ocorrencia;
    nova->numeroIngresso = numeroIngresso;
    strncpy(nova->cpf, cpf, 11);
    nova->cpf[11] = '\0';
    strncpy(nova->descricao, descricao, 255);
    nova->descricao[255] = '\0';
    nova->face = face;
    nova->proximo = NULL;

    if (lista.cauda == NULL) {
        lista.cabeca = nova;
        lista.cauda = nova;
    } else {
        lista.cauda->proximo = nova;
        lista.cauda = nova;
    }
    lista.quantidade++;

    cout << "[OCORRENCIA] Registrada para ingresso #" << numeroIngresso
         << ": " << descricao << endl;
}

void gerarArquivoOcorrencias(ListaOcorrencias &lista) {
    ofstream arquivo("ocorrencias.txt");
    if (!arquivo.is_open()) {
        cout << "[ERRO] Nao foi possivel criar arquivo de ocorrencias." << endl;
        return;
    }

    Ocorrencia* atual = lista.cabeca;
    int contador = 1;
    while (atual != NULL) {
        arquivo << "=== Ocorrencia " << contador << " ===" << endl;
        arquivo << "Ingresso: #" << atual->numeroIngresso << endl;
        arquivo << "CPF: " << atual->cpf << endl;
        arquivo << "Descricao: " << atual->descricao << endl;
        arquivo << endl;
        atual = atual->proximo;
        contador++;
    }

    arquivo.close();
    cout << "[OCORRENCIA] Arquivo 'ocorrencias.txt' gerado com "
         << lista.quantidade << " ocorrencia(s)." << endl;
}

void liberarListaOcorrencias(ListaOcorrencias &lista) {
    Ocorrencia* atual = lista.cabeca;
    while (atual != NULL) {
        Ocorrencia* temp = atual;
        atual = atual->proximo;
        delete temp;
    }
    lista.cabeca = NULL;
    lista.cauda = NULL;
    lista.quantidade = 0;
}

// ============================================================
// MAIN - Simulacao do fluxo completo
// ============================================================
int main() {
    srand(42);

    cout << "========================================" << endl;
    cout << " SISTEMA DE VENDA DE INGRESSOS" << endl;
    cout << " DCC205 - Estruturas de Dados - UFMG" << endl;
    cout << "========================================" << endl << endl;

    // Inicializar estruturas
    ListaUsuarios usuarios;
    inicializarListaUsuarios(usuarios);

    FilaRevista fila;
    inicializarFila(fila);

    ListaParentescos parentescos;
    inicializarListaParentescos(parentescos);

    ListaRegistrosPF registrosPF;
    inicializarListaPF(registrosPF);

    ListaOcorrencias ocorrencias;
    inicializarListaOcorrencias(ocorrencias);

    // ---------------------------------------------------------
    // ETAPA 1: Venda de ingressos com reconhecimento facial
    // ---------------------------------------------------------
    cout << "--- ETAPA 1: Venda de Ingressos ---" << endl;
    Usuario* u1 = inserirUsuario(usuarios, "11122233344", "Carlos Silva");
    Usuario* u2 = inserirUsuario(usuarios, "55566677788", "Maria Oliveira");
    Usuario* u3 = inserirUsuario(usuarios, "99988877766", "Joao Santos");
    Usuario* u4 = inserirUsuario(usuarios, "12312312300", "Ana Silva");
    cout << endl;

    // ---------------------------------------------------------
    // ETAPA 2: Usuarios entram na fila de revista (FIFO)
    // ---------------------------------------------------------
    cout << "--- ETAPA 2: Fila de Revista ---" << endl;
    enfileirar(fila, u1->numeroIngresso);
    enfileirar(fila, u2->numeroIngresso);
    enfileirar(fila, u3->numeroIngresso);
    enfileirar(fila, u4->numeroIngresso);
    cout << endl;

    // ---------------------------------------------------------
    // ETAPA 3: Registrar parentesco entre usuarios
    // ---------------------------------------------------------
    cout << "--- ETAPA 3: Registro de Parentesco ---" << endl;
    inserirRelacaoParentesco(parentescos, u1->cpf, u1->nome, u4->cpf, u4->nome, "pai e filha");
    inserirRelacaoParentesco(parentescos, u1->cpf, u1->nome, u3->cpf, u3->nome, "responsavel e dependente");
    cout << endl;

    cout << "Relacoes de parentesco registradas:" << endl;
    imprimirParentescos(parentescos);
    cout << endl;

    // ---------------------------------------------------------
    // ETAPA 4: Realizar revista (desenfileirar) e gerar registros PF
    // ---------------------------------------------------------
    cout << "--- ETAPA 4: Revista e Envio para PF ---" << endl;
    while (fila.tamanho > 0) {
        int numIngresso = desenfileirar(fila);
        Usuario* u = buscarUsuarioPorIngresso(usuarios, numIngresso);
        if (u != NULL) {
            u->revistado = true;
            cout << "[REVISTA] Ingresso #" << u->numeroIngresso
                 << " - " << u->nome << " revistado." << endl;

            // Gerar registro para PF
            inserirRegistroPF(registrosPF, u->cpf, u->face);
        }
    }
    cout << endl;

    gerarArquivoPF(registrosPF);
    cout << endl;

    // ---------------------------------------------------------
    // ETAPA 5: Registrar ocorrencias (se houver)
    // ---------------------------------------------------------
    cout << "--- ETAPA 5: Ocorrencias ---" << endl;
    // Simulando uma ocorrencia para o usuario Joao
    Usuario* suspeito = buscarUsuarioPorIngresso(usuarios, u3->numeroIngresso);
    if (suspeito != NULL) {
        inserirOcorrencia(ocorrencias, suspeito->numeroIngresso,
                          suspeito->cpf,
                          "Objeto proibido encontrado durante revista",
                          suspeito->face);
    }
    cout << endl;

    gerarArquivoOcorrencias(ocorrencias);
    cout << endl;

    // ---------------------------------------------------------
    // Liberar memoria
    // ---------------------------------------------------------
    cout << "--- Liberando memoria ---" << endl;
    liberarListaUsuarios(usuarios);
    liberarFila(fila);
    liberarListaParentescos(parentescos);
    liberarListaPF(registrosPF);
    liberarListaOcorrencias(ocorrencias);
    cout << "Memoria liberada com sucesso." << endl;

    return 0;
}
