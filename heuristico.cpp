#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

// Estructura para representar una clase
struct Clase {
    int id;
    int duracion;
    vector<int> estudiantes;
    int profesor;
};

// Estructura para representar un nodo en el �rbol de b�squeda
struct Nodo {
    vector<int> asignaciones; // asignaciones[i] = horario y aula de la clase i
    int conflictos;
    double limiteInferior; // L�mite inferior de conflictos
};

// Funci�n para comparar nodos por l�mite inferior
struct CompararNodo {
    bool operator()(const Nodo& a, const Nodo& b) {
        return a.limiteInferior > b.limiteInferior;
    }
};

// Funci�n para calcular el l�mite inferior de conflictos
double calcularLimiteInferior(const vector<Clase>& clases, const Nodo& nodo) {
    // En este ejemplo, el l�mite inferior es el n�mero de conflictos actual
    return nodo.conflictos;
}

// Funci�n para verificar conflictos entre dos clases
bool hayConflicto(const Clase& clase1, const Clase& clase2, int horario1, int horario2, int aula1, int aula2) {
    // Verificar si las clases se superponen en horario y comparten estudiantes o profesor
    bool superposicionHorario = (horario1 < horario2 + clase2.duracion) && (horario2 < horario1 + clase1.duracion);
    bool mismoProfesor = (clase1.profesor == clase2.profesor);
    bool estudiantesCompartidos = false;
    for (int est : clase1.estudiantes) {
        if (find(clase2.estudiantes.begin(), clase2.estudiantes.end(), est) != clase2.estudiantes.end()) {
            estudiantesCompartidos = true;
            break;
        }
    }
    return superposicionHorario && (mismoProfesor || estudiantesCompartidos);
}

// Funci�n para resolver el problema de optimizaci�n de horarios
Nodo optimizarHorarios(const vector<Clase>& clases, const vector<int>& horarios, const vector<int>& aulas) {
    int n = clases.size();
    Nodo mejorSolucion;
    mejorSolucion.conflictos = INT_MAX;

    // Cola de prioridad para almacenar nodos
    priority_queue<Nodo, vector<Nodo>, CompararNodo> cola;

    // Inicializar el nodo ra�z
    Nodo raiz;
    raiz.asignaciones.resize(n, -1); // -1 significa no asignado
    raiz.conflictos = 0;
    raiz.limiteInferior = calcularLimiteInferior(clases, raiz);
    cola.push(raiz);

    while (!cola.empty()) {
        Nodo actual = cola.top();
        cola.pop();

        // Si es una soluci�n completa
        if (find(actual.asignaciones.begin(), actual.asignaciones.end(), -1) == actual.asignaciones.end()) {
            if (actual.conflictos < mejorSolucion.conflictos) {
                mejorSolucion = actual;
            }
            continue;
        }

        // Encontrar la pr�xima clase sin asignar
        int claseActual = find(actual.asignaciones.begin(), actual.asignaciones.end(), -1) - actual.asignaciones.begin();

        // Generar nodos hijos asignando la clase a todos los horarios y aulas posibles
        for (int horario : horarios) {
            for (int aula : aulas) {
                Nodo hijo = actual;
                hijo.asignaciones[claseActual] = horario * 100 + aula; // Codificar horario y aula

                // Calcular conflictos
                for (int i = 0; i < n; i++) {
                    if (i != claseActual && hijo.asignaciones[i] != -1) {
                        int horarioAsignado = hijo.asignaciones[i] / 100;
                        int aulaAsignada = hijo.asignaciones[i] % 100;
                        if (hayConflicto(clases[claseActual], clases[i], horario, horarioAsignado, aula, aulaAsignada)) {
                            hijo.conflictos++;
                        }
                    }
                }

                // Calcular l�mite inferior
                hijo.limiteInferior = calcularLimiteInferior(clases, hijo);

                // Agregar a la cola si el l�mite inferior es menor que la mejor soluci�n
                if (hijo.limiteInferior < mejorSolucion.conflictos) {
                    cola.push(hijo);
                }
            }
        }
    }

    return mejorSolucion;
}

int main() {
    // Ejemplo de clases
    vector<Clase> clases = {
        {0, 2, {1, 2, 3}, 10}, // Clase 0: duraci�n 2, estudiantes 1, 2, 3, profesor 10
        {1, 1, {2, 3, 4}, 11}, // Clase 1: duraci�n 1, estudiantes 2, 3, 4, profesor 11
        {2, 3, {1, 4, 5}, 10}  // Clase 2: duraci�n 3, estudiantes 1, 4, 5, profesor 10
    };

    // Horarios y aulas disponibles
    vector<int> horarios = {9, 11, 14}; // Horarios: 9:00, 11:00, 14:00
    vector<int> aulas = {101, 102};     // Aulas: 101, 102

    // Resolver el problema
    Nodo solucion = optimizarHorarios(clases, horarios, aulas);

    // Mostrar la soluci�n
    cout << "Mejor soluci�n encontrada con " << solucion.conflictos << " conflictos:" << endl;
    for (int i = 0; i < clases.size(); i++) {
        int horario = solucion.asignaciones[i] / 100;
        int aula = solucion.asignaciones[i] % 100;
        cout << "Clase " << i << ": Horario " << horario << ":00, Aula " << aula << endl;
    }

    return 0;
}
