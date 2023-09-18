#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    string arquivo_M1 = "M1.txt";
    string arquivo_M2 = "M2.txt";
    string arquivo_resultado = "resultado_sequencial.txt";

    ifstream arquivo1(arquivo_M1);
    ifstream arquivo2(arquivo_M2);
    ofstream arquivo_resultado_saida(arquivo_resultado);

    if (!arquivo1.is_open() || !arquivo2.is_open() || !arquivo_resultado_saida.is_open()) {
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

    if (coluna1 != linha2) {
        cout << "As dimensões das matrizes não são compatíveis para multiplicação." << endl;
        return 1;
    }

    vector<vector<int>> resultado(linha1, vector<int>(coluna2, 0));

    auto inicio = high_resolution_clock::now();

    for (int i = 0; i < linha1; i++) {
        for (int j = 0; j < coluna2; j++) {
            for (int k = 0; k < coluna1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    arquivo_resultado_saida << linha1 << " " << coluna2 << endl;
    for (int i = 0; i < linha1; i++) {
        for (int j = 0; j < coluna2; j++) {
            arquivo_resultado_saida << resultado[i][j] << " ";
        }
        arquivo_resultado_saida << endl;
    }

    arquivo_resultado_saida << "Tempo de execução (ms): " << duracao.count() << endl;

    cout << "Multiplicação das matrizes concluída. Resultado salvo em " << arquivo_resultado << endl;

    arquivo1.close();
    arquivo2.close();
    arquivo_resultado_saida.close();

    return 0;
}