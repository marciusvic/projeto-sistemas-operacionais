#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    int linha1, coluna1, linha2, coluna2;

    cout << "Digite as dimensoes da primeira matriz (linha1 coluna1): ";
    cin >> linha1 >> coluna1;

    cout << "Digite as dimensoes da segunda matriz (linha2 coluna2): ";
    cin >> linha2 >> coluna2;

    if (coluna1 != linha2) {
        cout << "As dimensões das matrizes não são compatíveis para multiplicação." << endl;
        return 1;
    }

    srand(time(nullptr));

    ofstream file1("M1.txt");
    ofstream file2("M2.txt");

    file1 << linha1 << " " << coluna1 << endl;
    file2 << linha2 << " " << coluna2 << endl;

    for (int i = 0; i < linha1; i++) {
        for (int j = 0; j < coluna1; j++) {
            file1 << rand() % 100 << " ";
        }
        file1 << endl;
    }

    for (int i = 0; i < linha2; i++) {
        for (int j = 0; j < coluna2; j++) {
            file2 << rand() % 100 << " ";
        }
        file2 << endl;
    }

    cout << "Matrizes M1 e M2 geradas e salvas nos arquivos M1.txt e M2.txt." << endl;

    return 0;
}