import os
import re
import pandas as pd
import matplotlib.pyplot as plt

def generar_graficos():
    # Carpeta donde está este script
    ruta_base = os.path.dirname(os.path.abspath(__file__))

    # Rutas relativas a la carpeta del script
    ruta_mediciones = os.path.join(ruta_base, "..", "data", "measurements")
    ruta_plots = os.path.join(ruta_base, "..", "data", "plots")

    # Normalizar rutas (convierte ".." y "\" correctamente)
    ruta_mediciones = os.path.normpath(ruta_mediciones)
    ruta_plots = os.path.normpath(ruta_plots)

    # Crear carpeta de plots si no existe
    os.makedirs(ruta_plots, exist_ok=True)

    datos = []

    # Regex para parsear líneas
    regex_tamano = re.compile(r"Tamano matriz: (\d+)x\d+")
    regex_alg = re.compile(r"(Naive|Strassen): ([0-9.]+) ms ([0-9]+) KB")

    # Recorrer archivos de mediciones
    if not os.path.exists(ruta_mediciones):
        print(f"No se encontró la carpeta de mediciones: {ruta_mediciones}")
        return

    for archivo in os.listdir(ruta_mediciones):
        ruta_archivo = os.path.join(ruta_mediciones, archivo)
        if not os.path.isfile(ruta_archivo):
            continue

        with open(ruta_archivo, "r", encoding="utf-8") as f:
            contenido = f.readlines()

        tamano = None
        for linea in contenido:
            m_tam = regex_tamano.search(linea)
            if m_tam:
                tamano = int(m_tam.group(1))

            m_alg = regex_alg.search(linea)
            if m_alg and tamano is not None:
                algoritmo = m_alg.group(1)
                tiempo = float(m_alg.group(2))
                memoria = int(m_alg.group(3))

                datos.append({
                    "Tamano": tamano,
                    "Algoritmo": algoritmo,
                    "Tiempo_ms": tiempo,
                    "Memoria_KB": memoria
                })

    # Pasar a DataFrame
    df = pd.DataFrame(datos)
    if df.empty:
        print("No se encontraron datos válidos en los archivos.")
        return

    # Gráfico Tiempo
    plt.figure(figsize=(8,5))
    for alg in df["Algoritmo"].unique():
        sub = df[df["Algoritmo"] == alg].sort_values("Tamano")
        plt.plot(sub["Tamano"], sub["Tiempo_ms"], marker="o", label=alg)
    plt.xlabel("Tamaño de matriz (N)")
    plt.ylabel("Tiempo (ms)")
    plt.title("Rendimiento: Naive vs Strassen (Tiempo)")
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(ruta_plots, "tiempo.png"), dpi=200)
    plt.close()

    # Gráfico Memoria
    plt.figure(figsize=(8,5))
    for alg in df["Algoritmo"].unique():
        sub = df[df["Algoritmo"] == alg].sort_values("Tamano")
        plt.plot(sub["Tamano"], sub["Memoria_KB"], marker="o", label=alg)
    plt.xlabel("Tamaño de matriz (N)")
    plt.ylabel("Memoria (KB)")
    plt.title("Uso de Memoria: Naive vs Strassen")
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(ruta_plots, "memoria.png"), dpi=200)
    plt.close()

    print(f"Gráficos guardados en {ruta_plots}")

if __name__ == "__main__":
    generar_graficos()
