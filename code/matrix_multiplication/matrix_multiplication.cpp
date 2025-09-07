#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <filesystem>
#include <sys/resource.h>
#include <map>
#include <sstream>
#include <unistd.h> // Para getpagesize()
#include <cstdio>   // Para fopen, fscanf, fclose

using namespace std;
namespace fs = filesystem;
namespace cr = chrono;

// Declaraciones de funciones externas
extern void mulMat(vector<vector<int>>& m1, vector<vector<int>>& m2, vector<vector<int>>& res);
extern vector<vector<int>> multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2);

// ----------------------------------------------------------------------------------
// Funciones auxiliares
// ----------------------------------------------------------------------------------

vector<vector<int>> leer_matriz(const fs::path& file_path) {
    ifstream in(file_path);
    if (!in.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada: " << file_path << endl;
        return {};
    }
    
    vector<vector<int>> mat;
    string line;
    while (getline(in, line)) {
        vector<int> row;
        stringstream ss(line);
        int val;
        while (ss >> val) {
            row.push_back(val);
        }
        if (!row.empty()) {
            mat.push_back(row);
        }
    }
    in.close();
    return mat;
}

// Guardar matriz en archivo
void guardar_matriz_resultado(const vector<vector<int>>& mat, const fs::path& file_path) {
    try {
        fs::path dir_path = file_path.parent_path();
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
            cout << "Se ha creado el directorio de salida: " << dir_path << endl;
        }

        ofstream out(file_path);
        if (out.is_open()) {
            for (const auto& row : mat) {
                for (size_t i = 0; i < row.size(); ++i) {
                    out << row[i] << (i == row.size() - 1 ? "" : " ");
                }
                out << "\n";
            }
            cout << "Matriz resultado guardada en: " << file_path << endl;
        } else {
            cerr << "Error: No se pudo abrir el archivo de salida para escribir: " << file_path << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error de filesystem al guardar el resultado: " << e.what() << endl;
    }
}

long getCurrentMemoryUsageKB() {
    long rss = 0;
    FILE* fp = fopen("/proc/self/statm", "r");
    if (fp) {
        if (fscanf(fp, "%*s%ld", &rss) == 1) {
            fclose(fp);
            return rss * (getpagesize() / 1024);
        }
        fclose(fp);
    }
    return 0;
}

void guardar_tiempos(const string& algoritmo, const string& caso, const string& tamano_mat, double tiempo_ms, long memoria_kb) {
    fs::path dir_path = "data/measurements/";
    
    try {
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
            cout << "Se ha creado el directorio: " << dir_path << endl;
        }

        fs::path file_path = dir_path / (caso + "_multiplication_measurements.txt");
        ofstream out(file_path, ios::app);

        if (out.is_open()) {
            out << "Archivo: " << caso << " | Tamano matriz: " << tamano_mat << "x" << tamano_mat << "\n";
            out << "[ok] " << algoritmo << ": " << tiempo_ms << " ms " << memoria_kb << " KB\n";
            cout << "Datos guardados en: " << file_path << endl;
        } else {
            cerr << "Error: No se pudo abrir el archivo para escribir." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error de filesystem: " << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------
// Estimación teórica de memoria usada
// - Naive: n^2 enteros  -> bytes = n^2 * sizeof(int)
// - Strassen: ~4/3 n^2  -> bytes = (4/3) * n^2 * sizeof(int)
// Guardamos la estimación teórica; además mostramos el RSS observado tras ejecutar.
// ----------------------------------------------------------------------------------
long estimacion_memoria_kb(const string& algoritmo, size_t n) {
    using u64 = unsigned long long;
    u64 elems = static_cast<u64>(n) * static_cast<u64>(n);
    u64 bytes = elems * static_cast<u64>(sizeof(int));

    if (algoritmo == "Naive") {
        return static_cast<long>(bytes / 1024ULL);
    } else { // "Strassen"
        // (4/3) * bytes, con redondeo hacia arriba
        // evitar pérdida por división entera: (bytes * 4 + 2) / 3
        u64 bytes_strassen = (bytes * 4ULL + 2ULL) / 3ULL;
        return static_cast<long>(bytes_strassen / 1024ULL);
    }
}

// ----------------------------------------------------------------------------------
// Función principal de medición
// ----------------------------------------------------------------------------------

void record(const fs::path& file1, const fs::path& file2) {
    vector<vector<int>> M1 = leer_matriz(file1);
    vector<vector<int>> M2 = leer_matriz(file2);

    if (M1.empty() || M2.empty()) {
        cerr << "Error: No se pudieron leer las matrices de los archivos." << endl;
        return;
    }

    if (M1.size() != M2.size() || (M1.size() > 0 && M1[0].size() != M2[0].size())) {
        cerr << "Error: Las matrices no tienen dimensiones compatibles." << endl;
        return;
    }

    string caso_nombre = file1.filename().string();
    size_t pos = caso_nombre.rfind("_1.txt");
    if (pos != string::npos) {
        caso_nombre = caso_nombre.substr(0, pos);
    }
    string tamano_mat = to_string(M1.size());

    cout << "Archivo: " << caso_nombre << " | Tamano matriz: " << tamano_mat << "x" << tamano_mat << endl;

    vector<pair<string, function<vector<vector<int>>(const vector<vector<int>>&, const vector<vector<int>>& )>>> algoritmos = {
        {"Naive", [](const vector<vector<int>>& A, const vector<vector<int>>& B) {
            vector<vector<int>> res(A.size(), vector<int>(A.size()));
            mulMat(const_cast<vector<vector<int>>&>(A), const_cast<vector<vector<int>>&>(B), res);
            return res;
        }},
        {"Strassen", [](const vector<vector<int>>& A, const vector<vector<int>>& B) {
            return multiply(const_cast<vector<vector<int>>&>(A), const_cast<vector<vector<int>>&>(B));
        }}
    };

    const string output_base_path = "data/matrix_output/";

    for (auto& [nombre, funcion] : algoritmos) {
        // Estimación teórica antes de ejecutar (depende solo de n)
        long memoria_teorica_kb = estimacion_memoria_kb(nombre, M1.size());

        long rss_before_kb = getCurrentMemoryUsageKB();

        auto start = cr::high_resolution_clock::now();
        vector<vector<int>> resultado = funcion(M1, M2);
        auto end = cr::high_resolution_clock::now();

        long rss_after_kb = getCurrentMemoryUsageKB();

        cr::duration<double, milli> duration = end - start;

        // Reporte por consola: tiempo + memoria teórica + RSS observado
        cout << "[ok] " << nombre
             << ": " << duration.count() << " ms "
             << "| mem(teor): " << memoria_teorica_kb << " KB"
             << " | RSS(antes): " << rss_before_kb << " KB"
             << " | RSS(despues): " << rss_after_kb << " KB"
             << endl;

        // Guardamos la estimación teórica (consistente entre ejecuciones)
        guardar_tiempos(nombre, caso_nombre, tamano_mat, duration.count(), memoria_teorica_kb);

        fs::path output_file = fs::path(output_base_path) / (caso_nombre + "_" + nombre + ".txt");
        guardar_matriz_resultado(resultado, output_file);
    }
}

// ----------------------------------------------------------------------------------
// MAIN
// ----------------------------------------------------------------------------------

int main() {
    fs::path carpeta = "data/matrix_input";
    
    cout << "Buscando archivos de matrices en la carpeta: " << carpeta << endl;

    for (const auto& entry : fs::recursive_directory_iterator(carpeta)) {
        if (fs::is_regular_file(entry.path())) {
            string filename = entry.path().filename().string();
            if (filename.find("_1.txt") != string::npos) {
                string base_name = filename.substr(0, filename.rfind("_1.txt"));
                fs::path file2_path = entry.path().parent_path() / (base_name + "_2.txt");
                if (fs::exists(file2_path)) {
                    record(entry.path(), file2_path);
                } else {
                    cerr << "Error: No se encontró el archivo par para: " << entry.path() << endl;
                }
            }
        }
    }
    
    return 0;
}
