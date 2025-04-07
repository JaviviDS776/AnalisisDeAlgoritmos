#include <iostream>
#include <chrono>  //para medir el tiempo que le cuesta realizar acciones
#include <iomanip>
#include <vector>
#include <cstdlib>  // Para rand()

/*
ANGEL EDUARDO MU�OZ PEREZ
ICOMP CUALTOS 25A
ANALISIS DE ALGORITMOS
Introducci�n a la complejidad de algoritmos
*/

using namespace std;
using namespace std::chrono;

// Funci�n para ordenar un arreglo con Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);  // Intercambia si est�n en desorden
            }
        }
    }
}

// Genera un arreglo aleatorio de tama�o n
void generarArregloAleatorio(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 10000;  // N�meros aleatorios entre 0 y 9999
    }
}

// Mide el tiempo de ejecuci�n de Bubble Sort
//  Cada repetici�n genera un nuevo arreglo aleatorio y mide el tiempo de ordenamiento.
//  Por defecto, se hacen 2 repeticiones (para cada n).
vector<double> medirTiempoBubbleSort(int n, int repeticiones = 2) {
    vector<double> tiempos;
    int* arr = new int[n];  // Arreglo din�mico

    for (int r = 0; r < repeticiones; ++r) {
        generarArregloAleatorio(arr, n);  // Llena el arreglo con valores aleatorios

        auto inicio = high_resolution_clock::now(); //para iniciar el cronometro
        bubbleSort(arr, n);  // Ordena el arreglo
        auto fin = high_resolution_clock::now();   //para finalizar el cronometro

        duration<double> duracion = fin - inicio;
        tiempos.push_back(duracion.count());
    }

    delete[] arr;  // Libera la memoria
    return tiempos;
}

int main() {
    // Valores de n (tama�os del arreglo)
    vector<int> lista_n = {100, 500, 1000, 2000, 3000, 4000, 5000};

    // Cabecera de la tabla
    cout << "+-------------------+---------------+------------------+" << endl;
    cout << "| n (Tama�o Arreglo)| Repetici�n    | Tiempo (s)       |" << endl;
    cout << "+-------------------+---------------+------------------+" << endl;

    // Medici�n y visualizaci�n
    for (int n : lista_n) {
        vector<double> tiempos = medirTiempoBubbleSort(n);
        for (size_t r = 0; r < tiempos.size(); ++r) {
            cout << "| " << setw(17) << n << " | " << setw(13) << r + 1 << " | " 
                 << setw(16) << fixed << setprecision(6) << tiempos[r] << " |" << endl;
        }
    }
    cout << "+-------------------+---------------+------------------+" << endl;

    return 0;
}
