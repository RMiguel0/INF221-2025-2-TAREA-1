# Documentación

# INF-221 – Tarea 1 (2025-2)

**Alumno:** Miguel Rivero Medrano  
**Curso:** Algoritmos y Complejidad  
**Tarea 1 – Análisis experimental de algoritmos de ordenamiento y multiplicación de matrices**

---

## Multiplicación de matrices

### Programa principal

El programa principal para la multiplicación de matrices se encuentra en:

code/matrix_multiplication/matrix_multiplication.cpp

Su función es recorrer la carpeta `data/matrix_input/` y detectar automáticamente los pares de archivos
`*_1.txt` y `*_2.txt` que representan dos matrices de entrada. Por cada par válido:

1. **Lectura**: se cargan ambas matrices desde los archivos usando la función `leer_matriz`.
2. **Ejecución de algoritmos**: se aplican las dos implementaciones:
   - **Naive**: algoritmo clásico de complejidad $O(n^3)$.
   - **Strassen**: algoritmo recursivo de complejidad aproximada $O(n^{2.81})$.
3. **Mediciones**: para cada ejecución se registran:
   - El tiempo total de ejecución en milisegundos (medido con `std::chrono::high_resolution_clock`).
   - El uso de memoria teórico (calculado en función de $n^2$ para Naive y $\tfrac{4}{3}n^2$ para Strassen).
   - Además, el programa reporta en consola el RSS antes y después de cada llamada para referencia.
4. **Salida de resultados**:
   - La matriz resultante se guarda en `data/matrix_output/` con el nombre `<caso>_<algoritmo>.txt`.
   - Los tiempos y estimaciones de memoria se almacenan en `data/measurements/<caso>_multiplication_measurements.txt`.

De esta forma, el programa genera automáticamente tanto los resultados numéricos como las mediciones necesarias
para graficar posteriormente el rendimiento y consumo de memoria de cada algoritmo.


### Scripts

El script de ploteo se encuentra en `code/matrix_multiplication/scripts/plot_generator.py`.  
Su función es procesar los archivos de mediciones generados por el programa principal y producir las gráficas comparativas.

- **Entrada**: Archivos `.txt` ubicados en `data/measurements/`, los cuales contienen el tamaño de la matriz, 
  el nombre del algoritmo, el tiempo de ejecución y la memoria estimada.
- **Proceso**: 
  1. Se recorren todos los archivos de la carpeta de mediciones.  
  2. Mediante expresiones regulares se extraen los valores de tamaño, tiempo y memoria.  
  3. Con la librería `pandas` se organiza la información en un `DataFrame`.  
  4. Con `matplotlib` se generan las gráficas de líneas para cada algoritmo.
- **Salida**:  
  - `tiempo.png`: compara el tiempo de ejecución de Naive y Strassen en función del tamaño de la matriz.  
  - `memoria.png`: compara el consumo de memoria (KB) de ambos algoritmos.  
  Ambos archivos se guardan automáticamente en la carpeta `data/plots/`.

De esta manera, el script automatiza la creación de los gráficos y asegura que las figuras incluidas en el informe
provienen directamente de los resultados experimentales.

## Ordenamiento de arreglo unidimensional

El programa principal de ordenamiento está en:

code/sorting/sorting.cpp

### Programa principal

**Función general**  
- Recorre la carpeta `data/array_input/`, lee cada archivo de entrada (un arreglo de enteros) y ejecuta los algoritmos configurados.  
- Para cada algoritmo:
  1. Clona el arreglo original.  
  2. Mide el **tiempo** de ejecución con `std::chrono::high_resolution_clock`.  
  3. Estima/obtiene el **uso de memoria adicional** según la naturaleza del algoritmo.  
  4. Guarda el arreglo ordenado en `data/array_output/` con el sufijo `_<Algoritmo>.txt`.  
  5. Registra tiempo y memoria en `data/measurements/<caso>_sorting_measurements.txt`.

**Algoritmos soportados (según implementación):**
- `InsertionSort` (habilitado en el código base).  
- `MergeSort`, `QuickSort`, `PandaSort`, `SortStd` (descomentando se activan en `sorting.cpp`).  

**Convención de medición de memoria:**
- `InsertionSort`, `QuickSort`, `SortStd`: considerados *in-place*, se registra 0 KB de memoria adicional.  
- `MergeSort`: se suma un ajuste teórico de $O(n)$ (aprox. `N * sizeof(int)`), además del delta de RSS observado.  
- `PandaSort`: se suma un ajuste teórico de $O(\sqrt{n})` (colas auxiliares), además del delta de RSS observado.  

> **Nota:** en los gráficos de memoria, los valores para `MergeSort` y `PandaSort` representan una mezcla de medición empírica (RSS) más un ajuste teórico, con el fin de reflejar de manera más fiel su complejidad espacial.

**Salidas:**  
- `data/array_output/<caso>_<Algoritmo>.txt`: arreglo ordenado.  
- `data/measurements/<caso>_sorting_measurements.txt`: mediciones de tiempo y memoria.

---

### Scripts

El script de ploteo para ordenamiento está en:


**Qué hace**  
- Recorre `data/measurements/` y procesa los archivos `.txt`.  
- Extrae con expresiones regulares: `Tamano (N)`, `Algoritmo`, `Tiempo_ms`, `Memoria_KB`.  
- Construye un `DataFrame` con `pandas`.  
- Genera dos figuras con `matplotlib`:  
  - `tiempo.png`: tiempo vs tamaño (ejes en log).  
  - `memoria.png`: memoria vs tamaño (eje X en log).  

**Entrada:**  
- Archivos de mediciones en `data/measurements/`, con formato:  
  - `Archivo: <caso> | Tamano arreglo: <N>`  
  - `[ok] <Algoritmo>: <tiempo_ms> ms <memoria_kb> KB`  

**Salida:**  
- PNG en `data/plots/`:  
  - `tiempo.png`  
  - `memoria.png`  

**Ejecutar:**  
```bash
python code/sorting/scripts/plot_generator.py

