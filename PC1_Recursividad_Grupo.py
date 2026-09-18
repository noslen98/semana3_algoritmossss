"""
PC1_Recursividad_Grupo.py
Practica Calificada 1 - Recursividad, Recurrencias y Divide y Venceras
Curso: Algoritmos y Estructuras de Datos (SIS210) - Semana 3
Escuela Profesional de Ingenieria de Sistemas - UNA Puno

Integrantes:
    1. PARILLO UMINA JHONATAN
    2. RODRIGUEZ AGUILAR ALAIN NELSON
    3. LUIGGI LEONEL YEFRY CHOQUE GALVEZ

Ejecutar con:  python3 PC1_Recursividad_Grupo.py
"""

import math
import random
import time


# ==========================================================
# ACTIVIDAD 1: Recursividad basica - suma de digitos
# ==========================================================

def suma_digitos(n: int) -> int:
    """Suma los digitos de un entero no negativo de forma recursiva."""
    if n < 10:                              # CASO BASE (incluye n = 0)
        return n
    return n % 10 + suma_digitos(n // 10)   # CASO RECURSIVO


def suma_digitos_traza(n: int, nivel: int = 0) -> int:
    """Misma funcion, pero imprimiendo la pila de llamadas."""
    sangria = "  " * nivel
    if n < 10:
        print(f"{sangria}suma_digitos({n}) <- caso base, retorna {n}")
        return n
    print(f"{sangria}suma_digitos({n}) -> n%10={n % 10}, llama suma_digitos({n // 10})")
    resto = suma_digitos_traza(n // 10, nivel + 1)
    total = n % 10 + resto
    print(f"{sangria}suma_digitos({n}) retorna {n % 10} + {resto} = {total}")
    return total


def actividad_1():
    print("=" * 60)
    print("ACTIVIDAD 1: Recursividad basica - suma de digitos")
    print("=" * 60)

    # --- Verificaciones exigidas por la guia ---
    assert suma_digitos(12345) == 15, 'Error: 1+2+3+4+5=15'
    assert suma_digitos(0) == 0, 'Error: caso base n=0'
    assert suma_digitos(9) == 9, 'Error: un solo digito'
    print('suma_digitos(12345) =', suma_digitos(12345))

    # --- Traza de la pila de llamadas ---
    print("\nTraza de la pila de llamadas para suma_digitos(12345):")
    suma_digitos_traza(12345)

    print("\nTabla de traza:")
    print(f"{'Llamada':<22}{'n':>8}{'n%10':>8}{'Retorna':>16}")
    filas = [("suma_digitos(12345)", 12345, 5, "5 + 10 = 15"),
             ("suma_digitos(1234)", 1234, 4, "4 + 6 = 10"),
             ("suma_digitos(123)", 123, 3, "3 + 3 = 6"),
             ("suma_digitos(12)", 12, 2, "2 + 1 = 3"),
             ("suma_digitos(1) <- base", 1, 1, "1")]
    for llamada, n, d, ret in filas:
        print(f"{llamada:<22}{n:>8}{d:>8}{ret:>16}")
    print("Profundidad de la pila: 5 marcos (uno por cada digito).\n")


# ==========================================================
# ACTIVIDAD 2: Busqueda binaria
# ==========================================================

def busqueda_binaria(arr: list, objetivo: int) -> tuple[int, int]:
    """Retorna (indice, n_comparaciones). Retorna -1 si no se encuentra.
    Precondicion: arr debe estar ordenado de forma ascendente.
    """
    bajo, alto, comparaciones = 0, len(arr) - 1, 0
    while bajo <= alto:
        comparaciones += 1                  # operacion dominante
        medio = (bajo + alto) // 2
        if arr[medio] == objetivo:
            return medio, comparaciones
        elif arr[medio] < objetivo:
            bajo = medio + 1                # descarta la mitad izquierda
        else:
            alto = medio - 1                # descarta la mitad derecha
    return -1, comparaciones


def busqueda_lineal(arr: list, objetivo: int) -> tuple[int, int]:
    """Version secuencial, usada solo como referencia para la tabla."""
    for i, valor in enumerate(arr):
        if valor == objetivo:
            return i, i + 1
    return -1, len(arr)


def actividad_2():
    print("=" * 60)
    print("ACTIVIDAD 2: Busqueda Binaria")
    print("=" * 60)

    # --- Prueba con el arreglo del silabo (n = 10) ---
    arr = [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]
    idx, comp = busqueda_binaria(arr, 23)
    print(f'23 encontrado en indice {idx}, comparaciones={comp}')

    cota_teorica = math.ceil(math.log2(len(arr) + 1))
    print(f'Cota teorica: ceil(log2(10+1)) = {cota_teorica} comparaciones')

    assert comp <= cota_teorica, f'Supera la cota teorica: {comp} > {cota_teorica}'
    print('Cota verificada OK\n')

    # --- Tabla de comparaciones empiricas (peor caso) ---
    print("Tabla de comparaciones empiricas (peor caso):")
    print(f"{'n':>9}{'lineal(max)':>14}{'binaria(max)':>14}{'cota':>7}{'cumple':>9}")

    random.seed(42)
    for n in [10, 100, 1_000, 1_000_000]:
        datos = list(range(0, 2 * n, 2))    # arreglo ordenado de tamano n
        # Para n grande se muestrea, por costo de ejecucion.
        objetivos = list(datos) if n <= 1000 else random.sample(datos, 2000)
        objetivos += [-1, 2 * n + 1, datos[0], datos[-1], datos[n // 2]]

        maximo = max(busqueda_binaria(datos, v)[1] for v in objetivos)
        cota = math.ceil(math.log2(n + 1))
        cumple = 'Si' if maximo <= cota else 'NO'
        print(f"{n:>9}{n:>14}{maximo:>14}{cota:>7}{cumple:>9}")
    print()


# ==========================================================
# ACTIVIDAD 3: Merge Sort y benchmark
# ==========================================================

def merge(izq: list, der: list) -> list:
    """Mezcla dos listas ya ordenadas en una sola lista ordenada."""
    res, i, j = [], 0, 0
    while i < len(izq) and j < len(der):
        if izq[i] <= der[j]:            # '<=' preserva la estabilidad
            res.append(izq[i]); i += 1
        else:
            res.append(der[j]); j += 1
    res.extend(izq[i:])                 # restantes de la mitad izquierda
    res.extend(der[j:])                 # restantes de la mitad derecha
    return res


def merge_sort(arr: list) -> list:
    """Ordena una lista por Merge Sort. No modifica la lista original."""
    if len(arr) <= 1:                   # CASO BASE
        return arr[:]
    mid = len(arr) // 2                 # DIVIDIR
    izq = merge_sort(arr[:mid])         # VENCER (mitad izquierda)
    der = merge_sort(arr[mid:])         # VENCER (mitad derecha)
    return merge(izq, der)              # COMBINAR


def actividad_3():
    print("=" * 60)
    print("ACTIVIDAD 3: Merge Sort y benchmark")
    print("=" * 60)

    # --- Prueba con los codigos de matricula UNA-PUNO ---
    codigos_originales = [20211200, 20210300, 20211050, 20210780, 20211500,
                          20210100, 20211800, 20210640, 20210950, 20211400]

    t0 = time.perf_counter()
    codigos_ordenados = merge_sort(codigos_originales)
    ms = (time.perf_counter() - t0) * 1000

    print('Ordenados:', codigos_ordenados)
    assert codigos_ordenados == sorted(codigos_originales), 'Error en Merge Sort'
    print(f'Tiempo (n=10): {ms:.4f} ms\n')

    # --- Benchmark con time.perf_counter() ---
    print("Benchmark Merge Sort:")
    for n in [100, 1_000, 10_000, 100_000]:
        datos = random.sample(range(20_000_000, 29_999_999), n)  # fuera del reloj
        t0 = time.perf_counter()
        resultado = merge_sort(datos)
        ms_n = (time.perf_counter() - t0) * 1000
        assert resultado == sorted(datos), 'Error en Merge Sort'
        print(f'n={n:>8} tiempo={ms_n:10.3f} ms   t/(n*log2 n)={ms_n * 1000 / (n * math.log2(n)):.3f} us')
    print()


# ==========================================================
# ACTIVIDAD 4: Teorema Maestro
# ==========================================================

def actividad_4():
    print("=" * 60)
    print("ACTIVIDAD 4: Teorema Maestro - resolucion de recurrencias")
    print("=" * 60)

    # (recurrencia, a, b, exponente de f(n) = n^k, solucion)
    recurrencias = [
        ("(a) T(n) = 4T(n/2) + O(n)",   4, 2, 1, "Theta(n^2)"),
        ("(b) T(n) = T(n/3) + O(1)",    1, 3, 0, "Theta(log n)"),
        ("(c) T(n) = 2T(n/2) + O(n^2)", 2, 2, 2, "Theta(n^2)"),
        ("(d) T(n) = 3T(n/4) + O(n)",   3, 4, 1, "Theta(n)"),
    ]

    print(f"{'Recurrencia':<30}{'a':>3}{'b':>3}{'log_b(a)':>10}{'Caso':>7}  Solucion")
    for texto, a, b, k, solucion in recurrencias:
        critico = math.log(a, b)            # exponente critico log_b(a)
        if k < critico - 1e-9:
            caso = 1                        # dominan las llamadas recursivas
        elif abs(k - critico) <= 1e-9:
            caso = 2                        # equilibrio
        else:
            caso = 3                        # domina el trabajo local f(n)
        print(f"{texto:<30}{a:>3}{b:>3}{critico:>10.3f}{caso:>7}  {solucion}")
    print()


# ==========================================================
if __name__ == "__main__":
    actividad_1()
    actividad_2()
    actividad_3()
    actividad_4()
    print("Todas las actividades se ejecutaron correctamente.")
