# Documentación

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

### Programa principal

El programa principal de ordenamiento está en:

code/sorting/sorting.cpp


**Función general**  
- Recorre `data/array_input/`, lee cada archivo de enterada (un arreglo de enteros por línea/espacio) y ejecuta los algoritmos configurados.
- Para cada algoritmo:
  1) clona el arreglo original,  
  2) mide el **tiempo** con `std::chrono::high_resolution_clock`,  
  3) estima/obtiene el **uso de memoria adicional** (ver notas abajo),  
  4) guarda el arreglo ordenado en `data/array_output/` con el sufijo `_<Algoritmo>.txt`,  
  5) registra `tiempo` y `memoria` en `data/measurements/<caso>_sorting_measurements.txt`.

**Algoritmos soportados (según tu código):**
- `InsertionSort` (habilitado)
- `MergeSort` (comentado)
- `Quicksort` (comentado)
- `PandaSort` (comentado)
- `SortStd` (comentado)

**Convención de medición de memoria (tal como implementaste):**
- `InsertionSort`, `Quicksort`, `SortStd`: **in-place** → memoria adicional registrada como `0` KB (constante y despreciable).
- `MergeSort`: suma explícita de `O(N)` (aprox. `N * sizeof(int)`), además de un delta de RSS observado (`memoria_fin - memoria_inicio`) cuando procede.
- `PandaSort`: suma explícita de `O(sqrt(N))` (colas auxiliares) + delta de RSS cuando procede.


**Salidas:**
- `data/array_output/<caso>_<Algoritmo>.txt`: arreglo ordenado.
- `data/measurements/<caso>_sorting_measurements.txt`: líneas con:
  - `Archivo: <caso> | Tamano arreglo: <N>`
  - `[ok] <Algoritmo>: <tiempo_ms> ms <memoria_kb> KB`


### Scripts


El script de ploteo para ordenamiento está en:

code/sorting/scripts/plot_generator.py


**Qué hace**  
- Lee todos los archivos `.txt` en `code/sorting/data/measurements/`.
- Extrae con expresiones regulares: `Tamano (N)`, `Algoritmo`, `Tiempo_ms`, `Memoria_KB`.
- Construye un `DataFrame` (`pandas`) y genera dos figuras (`matplotlib`):
  - `tiempo.png`: tiempo vs tamaño.
  - `memoria.png`: memoria vs tamaño.

**Entrada**  
- Archivos de mediciones con el formato:
  - `Archivo: <caso> | Tamano arreglo: <N>`
  - `[ok] <Algoritmo>: <tiempo_ms> ms <memoria_kb> KB`

**Salida**  
- Figuras PNG en `code/sorting/data/plots/`:
  - `tiempo.png` (ejes X/Y en **log** en tu script)
  - `memoria.png` (eje X en **log** en tu script)

**Ejecución**  
- Edita, si es necesario, las rutas internas del script (`ruta_mediciones`, `ruta_plots`) para que apunten a tu repo local.  
- Ejecuta desde cualquier terminal con Python:
  ```bash
  python code/sorting/scripts/plot_generator.py
