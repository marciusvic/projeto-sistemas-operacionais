#include <iostream>
#include <fstream>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>

const int MaxLinhas = 100;
const int MaxColunas = 100;

// Define uma mutex para proteger o acesso a variáveis compartilhadas
std::mutex mtx;

void preencherMatrizAleatoriamente(int matriz[][MaxColunas], int linhas, int colunas)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            matriz[i][j] = rand() % 100;
        }
    }
}

void escreverMatrizEmArquivo(const int matriz[][MaxColunas], int linhas, int colunas, const std::string &nomeArquivo)
{
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "." << std::endl;
        return;
    }

    arquivo << linhas << " " << colunas << std::endl;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo << matriz[i][j] << "\t";
        }
        arquivo << std::endl;
    }

    arquivo.close();
}

void lerMatrizDeArquivo(int matriz[][MaxColunas], int &linhas, int &colunas, const std::string &nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "." << std::endl;
        return;
    }

    arquivo >> linhas >> colunas;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo >> matriz[i][j];
        }
    }

    arquivo.close();
}

void escreverMatrizResultanteETempoDeCalculo(int matriz[][MaxColunas], int linhas, int colunas, const std::string &nomeArquivo, double tempoDeCalculo)
{
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << "." << std::endl;
        return;
    }

    arquivo << linhas << " " << colunas << std::endl;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            arquivo << "c" << i + 1 << j + 1 << " " << matriz[i][j] << std::endl;
        }
    }

    arquivo << tempoDeCalculo << std::endl;

    arquivo.close();
}

void multiplicarMatrizes(const int matriz1[][MaxColunas], int linhas1, int colunas1, const int matriz2[][MaxColunas], int linhas2, int colunas2, int resultado[][MaxColunas])
{
    if (colunas1 != linhas2)
    {
        std::cerr << "Erro: Não é possível multiplicar as matrizes. O número de colunas da matriz 1 deve ser igual ao número de linhas da matriz 2." << std::endl;
        return;
    }

    for (int i = 0; i < linhas1; i++)
    {
        for (int j = 0; j < colunas2; j++)
        {
            resultado[i][j] = 0;
            for (int k = 0; k < colunas1; k++)
            {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

void multiplicarMatrizParte(const int matriz1[][MaxColunas], const int matriz2[][MaxColunas], int colunas1, int colunas2, int inicio, int fim, int resultado[][MaxColunas])
{
    for (int i = inicio; i < fim; i++)
    {
        for (int j = 0; j < colunas2; j++)
        {
            resultado[i][j] = 0;
            for (int k = 0; k < colunas1; k++)
            {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

int main()
{
    int Mat1Line = 0, Mat1Col = 0, Mat2Line = 0, Mat2Col = 0;

    std::cout << "Informe o número de linhas da Matriz 1: ";
    std::cin >> Mat1Line;
    std::cout << "Informe o número de colunas da Matriz 1: ";
    std::cin >> Mat1Col;
    std::cout << "Informe o número de linhas da Matriz 2: ";
    std::cin >> Mat2Line;
    std::cout << "Informe o número de colunas da Matriz 2: ";
    std::cin >> Mat2Col;

    int mat1[MaxLinhas][MaxColunas];
    int mat2[MaxLinhas][MaxColunas];

    preencherMatrizAleatoriamente(mat1, Mat1Line, Mat1Col);
    preencherMatrizAleatoriamente(mat2, Mat2Line, Mat2Col);

    escreverMatrizEmArquivo(mat1, Mat1Line, Mat1Col, "mat1.txt");
    escreverMatrizEmArquivo(mat2, Mat2Line, Mat2Col, "mat2.txt");

    std::cout << "As matrizes foram escritas nos arquivos 'mat1.txt' e 'mat2.txt'." << std::endl;

    int Mat1LineLida = 0, Mat1ColLida = 0;
    int mat1Lida[MaxLinhas][MaxColunas];
    lerMatrizDeArquivo(mat1Lida, Mat1LineLida, Mat1ColLida, "mat1.txt");

    std::cout << "Matriz 1 lida do arquivo 'mat1.txt':" << std::endl;
    for (int i = 0; i < Mat1LineLida; i++)
    {
        for (int j = 0; j < Mat1ColLida; j++)
        {
            std::cout << mat1Lida[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    int Mat2LineLida = 0, Mat2ColLida = 0;
    int mat2Lida[MaxLinhas][MaxColunas];
    lerMatrizDeArquivo(mat2Lida, Mat2LineLida, Mat2ColLida, "mat2.txt");

    std::cout << "Matriz 2 lida do arquivo 'mat2.txt':" << std::endl;
    for (int i = 0; i < Mat2LineLida; i++)
    {
        for (int j = 0; j < Mat2ColLida; j++)
        {
            std::cout << mat2Lida[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    int mat3Result[MaxLinhas][MaxColunas];

    clock_t inicio = clock();

    multiplicarMatrizes(mat1Lida, Mat1LineLida, Mat1ColLida, mat2Lida, Mat2LineLida, Mat2ColLida, mat3Result);

    clock_t fim = clock();

    double tempoDeCalculo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    std::cout << "A multiplicação de matrizes sequencial foi concluída." << std::endl;

    escreverMatrizResultanteETempoDeCalculo(mat3Result, Mat1LineLida, Mat2ColLida, "mat3_sequencial.txt", tempoDeCalculo);

    std::cout << "A matriz resultante e o tempo necessário para o cálculo sequencial foram escritos no arquivo 'mat3_sequencial.txt'." << std::endl;

    int P; // Número de threads desejado
    std::cout << "Informe o número de threads (P): ";
    std::cin >> P;

    std::vector<std::thread> threads;

    // Divide o trabalho entre as threads
    int linhasPorThread = Mat1LineLida / P;
    int inicioThread = 0;
    int fimThread = 0;

    for (int i = 0; i < P; i++)
    {
        inicioThread = i * linhasPorThread;
        fimThread = (i == P - 1) ? Mat1LineLida : (i + 1) * linhasPorThread;

        threads.emplace_back([&, i, inicioThread, fimThread]()
                             { multiplicarMatrizParte(mat1Lida, mat2Lida, Mat1ColLida, Mat2ColLida, inicioThread, fimThread, mat3Result); });
    }

    // Aguarda todas as threads terminarem
    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << "A multiplicação de matrizes com threads foi concluída." << std::endl;

    escreverMatrizResultanteETempoDeCalculo(mat3Result, Mat1LineLida, Mat2ColLida, "mat3_threads.txt", tempoDeCalculo);

    std::cout << "A matriz resultante e o tempo necessário para o cálculo com threads foram escritos no arquivo 'mat3_threads.txt'." << std::endl;

    return 0;
}
