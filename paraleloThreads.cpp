#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

void multiplicarESalvar(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2, vector<vector<int>>& resultado, int linhaInicial, int linhaFinal, int colunaInicial, int colunaFinal, int threadID, vector<long long>& temposIndividuais) {
    auto inicio_tempo = high_resolution_clock::now();
    for (int i = linhaInicial; i < linhaFinal; i++) {
        for (int j = colunaInicial; j < colunaFinal; j++) {
            for (int k = 0; k < matriz1[i].size(); k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
    auto fim_tempo = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim_tempo - inicio_tempo);  

    string nomeArquivo = "resultado_thread_" + to_string(threadID) + ".txt";
    ofstream arquivoResultado(nomeArquivo);
    if (!arquivoResultado.is_open()) {
        cout << "Erro ao criar arquivo de resultado para a thread " << threadID << endl;
        return;
    }

    for (int i = linhaInicial; i < linhaFinal; i++) {
        for (int j = colunaInicial; j < colunaFinal; j++) {
            arquivoResultado << resultado[i][j] << " ";
        }
        arquivoResultado << endl;
    }

    arquivoResultado << "Tempo de execução (ms): " << duracao.count() << endl;
    arquivoResultado.close();

    temposIndividuais[threadID] = duracao.count();
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

    int numThreads;
    cout << "Digite o número de threads desejado: ";
    cin >> numThreads;

    vector<vector<int>> resultado(numRowsResultado, vector<int>(numColsResultado, 0));

    int P = ceil(static_cast<double>(numRowsResultado) / numThreads);

    vector<thread> threads;
    vector<long long> temposIndividuais(numThreads);

    for (int i = 0; i < numThreads; i++) {
        int linhaInicial = i * P;
        int linhaFinal = (i == numThreads - 1) ? numRowsResultado : (i + 1) * P;

        threads.emplace_back(multiplicarESalvar, ref(matriz1), ref(matriz2), ref(resultado), linhaInicial, linhaFinal, 0, numColsResultado, i, ref(temposIndividuais));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    long long tempoTotal = 0;
    for (long long tempo : temposIndividuais) {
        tempoTotal += tempo;
    }

    ofstream arquivoTempoTotal("resultado_tempo_total_threads.txt");
    if (arquivoTempoTotal.is_open()) {
        arquivoTempoTotal << "Tempo total de todas as threads (ms): " << tempoTotal << endl;
        arquivoTempoTotal.close();
    }

    cout << "Multiplicação das matrizes usando " << numThreads << " threads concluída." << endl;

    return 0;
}
