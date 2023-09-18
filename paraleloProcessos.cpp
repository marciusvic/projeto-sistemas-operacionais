#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;
using namespace chrono;

void multiplicarESalvar(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2,
                     int linhaInicial, int linhaFinal, int colunaInicial, int colunaFinal,
                     int processID) {
    int numRows = linhaFinal - linhaInicial;
    int numCols = colunaFinal - colunaInicial;

    auto inicio_tempo = high_resolution_clock::now();

    vector<vector<int>> resultado(numRows, vector<int>(numCols, 0));

    for (int i = linhaInicial; i < linhaFinal; i++) {
        for (int j = colunaInicial; j < colunaFinal; j++) {
            for (int k = 0; k < matriz1[i].size(); k++) {
                resultado[i - linhaInicial][j - colunaInicial] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    auto fim_tempo = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim_tempo - inicio_tempo);

    ostringstream nomeArquivoStream;
    nomeArquivoStream << "resultado_processo_" << processID << ".txt";
    string nomeArquivo = nomeArquivoStream.str();

    ofstream arquivoResultado(nomeArquivo);
    if (arquivoResultado.is_open()) {
        arquivoResultado << numRows << " " << numCols << endl;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                arquivoResultado << resultado[i][j] << " ";
            }
            arquivoResultado << endl;
        }
        arquivoResultado << "Tempo de execução (ms): " << duracao.count() << endl;
        arquivoResultado.close();
    }
}

int main() {
    string arquivoMatriz1 = "M1.txt";
    string arquivoMatriz2 = "M2.txt";

    ifstream arquivo1(arquivoMatriz1);
    ifstream arquivo2(arquivoMatriz2);

    if (!arquivo1.is_open() || !arquivo2.is_open()) {
        cout << "Erro ao abrir os arquivos." << endl;
        return 1;
    }

    int linha1, coluna1, linha2, coluna2;
    arquivo1 >> linha1 >> coluna1;
    arquivo2 >> linha2 >> coluna2;

    if (coluna1 != linha2) {
        cout << "As dimensões das matrizes não são compatíveis para multiplicação." << endl;
        return 1;
    }

    vector<vector<int>> matriz1(linha1, vector<int>(coluna1));
    vector<vector<int>> matriz2(linha2, vector<int>(coluna2));

    for (int i = 0; i < linha1; i++) {
        for (int j = 0; j < coluna1; j++) {
            arquivo1 >> matriz1[i][j];
        }
    }

    for (int i = 0; i < linha2; i++) {
        for (int j = 0; j < coluna2; j++) {
            arquivo2 >> matriz2[i][j];
        }
    }

    int numRowsResultado = linha1;
    int numColsResultado = coluna2;

    int numProcessos;
    cout << "Digite o número de processos desejado: ";
    cin >> numProcessos;

    int tamanhoTarefa = ceil(static_cast<double>(numRowsResultado) / numProcessos);

    vector<pid_t> pidsFilhos(numProcessos);

    for (int i = 0; i < numProcessos; i++) {
        int linhaInicio = i * tamanhoTarefa;
        int linhaFim = min((i + 1) * tamanhoTarefa, numRowsResultado);

        pid_t pidFilho = fork();

        if (pidFilho == 0) {
            multiplicarESalvar(matriz1, matriz2, linhaInicio, linhaFim, 0, numColsResultado, i);
            exit(0);
        } else if (pidFilho > 0) {
            pidsFilhos[i] = pidFilho;
        } else {
            cout << "Erro ao criar processo filho." << endl;
            return 1;
        }
    }

    for (int i = 0; i < numProcessos; i++) {
        waitpid(pidsFilhos[i], NULL, 0);
    }

    // Somar as durações de todos os processos
    long long somaDuracoes = 0;
    for (int i = 0; i < numProcessos; i++) {
        ostringstream nomeArquivoStream;
        nomeArquivoStream << "resultado_processo_" << i << ".txt";
        string nomeArquivo = nomeArquivoStream.str();

        ifstream arquivoResultado(nomeArquivo);
        if (arquivoResultado.is_open()) {
            string linha;
            while (getline(arquivoResultado, linha)) {
                if (linha.find("Tempo de execução (ms):") != string::npos) {
                    size_t pos = linha.find(":");
                    if (pos != string::npos) {
                        long long duracao = stoll(linha.substr(pos + 1));
                        somaDuracoes += duracao;
                    }
                }
            }
            arquivoResultado.close();
        }
    }

    ofstream arquivoSomaDuracoes("resultado_tempo_total_processos.txt");
    if (arquivoSomaDuracoes.is_open()) {
        arquivoSomaDuracoes << "Tempo total de todos os processos (ms): " << somaDuracoes << endl;
        arquivoSomaDuracoes.close();
    }

    cout << "Multiplicação das matrizes usando " << numProcessos << " processos concluída." << endl;
    return 0;
}
