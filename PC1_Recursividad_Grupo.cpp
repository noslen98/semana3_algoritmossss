// ============================================================
//  PC1_Recursividad_Grupo.cpp
//  Practica Calificada 1 - Recursividad, Recurrencias y Divide y Venceras
//  Curso: Algoritmos y Estructuras de Datos (SIS210) - Semana 3
//  Escuela Profesional de Ingenieria de Sistemas - UNA Puno
//
//  Integrantes:
//      1. PARILLO UMINA JHONATAN
//      2. RODRIGUEZ AGUILAR ALAIN NELSON
//      3. LUIGGI LEONEL YEFRY CHOQUE GALVEZ
//
//  Compilar y ejecutar:
//      g++ -std=c++17 -O2 -o pc1 PC1_Recursividad_Grupo.cpp && ./pc1
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>
#include <iomanip>
#include <cassert>

using namespace std;

// ============================================================
// ACTIVIDAD 1: Recursividad basica - suma de digitos
// ============================================================

int sumaDigitos(int n) {
    // Caso base: numero de un solo digito
    if (n < 10) return n;
    // Caso recursivo: ultimo digito + suma del resto
    return n % 10 + sumaDigitos(n / 10);
}

// Misma funcion, pero imprimiendo la pila de llamadas.
int sumaDigitosTraza(int n, int nivel = 0) {
    string sangria(2 * nivel, ' ');
    if (n < 10) {
        cout << sangria << "suma_digitos(" << n << ") <- caso base, retorna " << n << '\n';
        return n;
    }
    cout << sangria << "suma_digitos(" << n << ") -> n%10=" << n % 10
         << ", llama suma_digitos(" << n / 10 << ")\n";
    int resto = sumaDigitosTraza(n / 10, nivel + 1);
    int total = n % 10 + resto;
    cout << sangria << "suma_digitos(" << n << ") retorna " << n % 10
         << " + " << resto << " = " << total << '\n';
    return total;
}

void actividad1() {
    cout << "============================================================\n";
    cout << "ACTIVIDAD 1: Recursividad basica - suma de digitos\n";
    cout << "============================================================\n";

    assert(sumaDigitos(12345) == 15);
    assert(sumaDigitos(0) == 0);
    assert(sumaDigitos(9) == 9);
    cout << "suma_digitos(12345) = " << sumaDigitos(12345) << '\n';

    cout << "\nTraza de la pila de llamadas para suma_digitos(12345):\n";
    sumaDigitosTraza(12345);
    cout << "Profundidad de la pila: 5 marcos (uno por cada digito).\n\n";
}

// ============================================================
// ACTIVIDAD 2: Busqueda binaria
// ============================================================

pair<int,int> busquedaBinaria(const vector<int>& arr, int obj) {
    int bajo = 0;                   // limite izquierdo del rango de busqueda
    int alto = (int)arr.size() - 1; // limite derecho del rango de busqueda
    int comparaciones = 0;          // contador para medir el rendimiento

    // Mientras el rango [bajo, alto] no este vacio, sigue buscando.
    while (bajo <= alto) {
        comparaciones++;
        // Punto medio del rango actual. Se calcula asi (en vez de
        // (bajo+alto)/2) para evitar overflow si bajo+alto fuera muy grande.
        int medio = bajo + (alto - bajo) / 2;

        if (arr[medio] == obj) {
            // Encontrado: retornamos su posicion y cuantas comparaciones costo.
            return {medio, comparaciones};
        } else if (arr[medio] < obj) {
            // El objetivo, si existe, esta a la derecha del medio.
            bajo = medio + 1;
        } else {
            // El objetivo, si existe, esta a la izquierda del medio.
            alto = medio - 1;
        }
    }
    // El rango quedo vacio: el elemento no esta en el arreglo.
    return {-1, comparaciones};
}

void actividad2() {
    cout << "============================================================\n";
    cout << "ACTIVIDAD 2: Busqueda Binaria\n";
    cout << "============================================================\n";

    // Arreglo del silabo (debe estar ordenado para que funcione bien).
    vector<int> arr = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    auto [idx, comp] = busquedaBinaria(arr, 23);
    cout << "Indice: " << idx << "  Comparaciones: " << comp << '\n';

    // Cota teorica: ceil(log2(n+1)) viene de resolver T(n) = T(n/2) + O(1).
    int cota = (int)ceil(log2(arr.size() + 1));
    cout << "Cota teorica: " << cota << '\n';
    assert(comp <= cota);
    cout << "Cota verificada correctamente.\n\n";

    // --- Tabla de comparaciones empiricas (peor caso: elemento inexistente) ---
    cout << "Tabla de comparaciones empiricas (peor caso):\n";
    cout << left << setw(12) << "n" << setw(16) << "lineal(max)"
         << setw(16) << "binaria(max)" << setw(8) << "cota" << "cumple\n";

    vector<int> tamanos = {10, 100, 1000, 1000000};
    for (int n : tamanos) {
        // Arreglo ordenado de numeros pares: 0, 2, 4, ...
        vector<int> datos(n);
        for (int i = 0; i < n; i++) datos[i] = 2 * i;

        // Se busca un valor inexistente para forzar el peor caso.
        int inexistente = 2 * n + 1;
        auto res = busquedaBinaria(datos, inexistente);
        int cotaN = (int)ceil(log2(n + 1));

        cout << left << setw(12) << n << setw(16) << n
             << setw(16) << res.second << setw(8) << cotaN
             << (res.second <= cotaN ? "Si" : "NO") << '\n';
    }
    cout << '\n';
}

// ============================================================
// ACTIVIDAD 3: Merge Sort y benchmark con std::chrono
// ============================================================

vector<int> merge(const vector<int>& izq, const vector<int>& der) {
    vector<int> res;
    res.reserve(izq.size() + der.size()); // reserva memoria para evitar realojos
    size_t i = 0, j = 0; // punteros: i recorre "izq", j recorre "der"

    // Mientras haya elementos en ambos lados, se compara el frente
    // de cada uno y se agrega el menor al resultado.
    while (i < izq.size() && j < der.size()) {
        if (izq[i] <= der[j]) res.push_back(izq[i++]);
        else                  res.push_back(der[j++]);
    }
    // Si a uno de los dos lados le sobraron elementos (el otro se
    // agoto primero), se agregan directamente: ya estaban ordenados.
    while (i < izq.size()) res.push_back(izq[i++]);
    while (j < der.size()) res.push_back(der[j++]);
    return res;
}

// Ordena "arr" usando Merge Sort: divide, ordena cada mitad
// (recursivamente) y luego combina los resultados.
vector<int> mergeSort(const vector<int>& arr) {
    // ---------- CASO BASE ----------
    // Un arreglo de 0 o 1 elementos ya esta ordenado: no hay nada
    // que dividir. Esto detiene la recursion.
    if (arr.size() <= 1) return arr;

    // ---------- DIVIDE ----------
    size_t mid = arr.size() / 2;
    vector<int> izq(arr.begin(), arr.begin() + mid);
    vector<int> der(arr.begin() + mid, arr.end());

    // ---------- VENCERAS + COMBINA ----------
    return merge(mergeSort(izq), mergeSort(der));
}

void actividad3() {
    cout << "============================================================\n";
    cout << "ACTIVIDAD 3: Merge Sort y benchmark\n";
    cout << "============================================================\n";
    cout << fixed << setprecision(3);

    // --- Prueba con los codigos de matricula UNA-PUNO (n = 10) ---
    vector<int> codigos = {20211200, 20210300, 20211050, 20210780, 20211500,
                           20210100, 20211800, 20210640, 20210950, 20211400};

    auto t0 = chrono::high_resolution_clock::now();
    vector<int> ordenado = mergeSort(codigos);
    auto t1 = chrono::high_resolution_clock::now();
    double msN10 = chrono::duration<double, milli>(t1 - t0).count();

    cout << "Ordenados (n=10): ";
    for (int c : ordenado) cout << c << " ";
    cout << '\n';

    // Verificamos comparando contra std::sort.
    vector<int> esperado = codigos;
    sort(esperado.begin(), esperado.end());
    assert(ordenado == esperado);
    cout << "Merge Sort verificado correctamente (n=10)\n";
    cout << "Tiempo (n=10): " << msN10 << " ms\n\n";

    // --- Benchmark con std::chrono ---
    cout << "Benchmark Merge Sort:\n";
    cout << left << setw(12) << "n" << "tiempo (ms)\n";

    mt19937 rng(42); // semilla fija = resultados reproducibles
    uniform_int_distribution<int> dist(20000000, 29999999);
    vector<int> tamanos = {100, 1000, 10000, 100000};

    for (int n : tamanos) {
        vector<int> datos(n);
        for (int i = 0; i < n; i++) datos[i] = dist(rng);

        auto ini = chrono::high_resolution_clock::now();
        vector<int> res = mergeSort(datos);
        auto fin = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(fin - ini).count();

        assert(is_sorted(res.begin(), res.end()));
        cout << left << setw(12) << n << ms << " ms\n";
    }
    cout << '\n';
}

// ============================================================
// ACTIVIDAD 4: Teorema Maestro
// ============================================================

void actividad4() {
    cout << "============================================================\n";
    cout << "ACTIVIDAD 4: Teorema Maestro - resolucion de recurrencias\n";
    cout << "============================================================\n";

    struct Recurrencia {
        string texto;
        int a, b;
        double k;           // exponente de f(n) = n^k
        string solucion;
    };

    vector<Recurrencia> recs = {
        {"(a) T(n) = 4T(n/2) + O(n)",   4, 2, 1, "Theta(n^2)"},
        {"(b) T(n) = T(n/3) + O(1)",    1, 3, 0, "Theta(log n)"},
        {"(c) T(n) = 2T(n/2) + O(n^2)", 2, 2, 2, "Theta(n^2)"},
        {"(d) T(n) = 3T(n/4) + O(n)",   3, 4, 1, "Theta(n)"}
    };

    cout << fixed << setprecision(3);
    cout << left << setw(32) << "Recurrencia" << setw(4) << "a" << setw(4) << "b"
         << setw(11) << "log_b(a)" << setw(7) << "Caso" << "Solucion\n";

    for (const auto& r : recs) {
        double critico = log((double)r.a) / log((double)r.b); // log_b(a)
        int caso;
        if (r.k < critico - 1e-9)            caso = 1; // dominan las llamadas
        else if (fabs(r.k - critico) <= 1e-9) caso = 2; // equilibrio
        else                                  caso = 3; // domina el trabajo local

        cout << left << setw(32) << r.texto << setw(4) << r.a << setw(4) << r.b
             << setw(11) << critico << setw(7) << caso << r.solucion << '\n';
    }
    cout << '\n';
}

// ============================================================
int main() {
    actividad1();
    actividad2();
    actividad3();
    actividad4();
    cout << "Todos los tests pasaron correctamente.\n";
    return 0;
}
