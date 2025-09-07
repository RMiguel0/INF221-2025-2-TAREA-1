#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional> 
#include <chrono>
#include <filesystem> 
#include <sys/resource.h>
#include <unistd.h>
#include <cstdio>
#include <map>
#include <cmath> // Se agregó para usar la función sqrt

// Incluye tus propios algoritmos de ordenamiento
extern void insertionSort(std::vector<int>& arr);
extern void mergeSort(std::vector<int>& vec);
extern std::vector<int> pandasort(std::vector<int> arr);
extern void quickSort(std::vector<int>& arr);
extern std::vector<int> sortArray(std::vector<int>& arr);


using namespace std;
namespace fs = filesystem;
namespace cr = chrono;

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


void guardar_array_ordenado(const vector<int>& arr, const fs::path& file_path) {
    try {
        fs::path dir_path = file_path.parent_path();
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
            cout << "Se ha creado el directorio de salida: " << dir_path << endl;
        }

        ofstream out(file_path);
        if (out.is_open()) {
            for (size_t i = 0; i < arr.size(); ++i) {
                out << arr[i] << (i == arr.size() - 1 ? "" : " ");
            }
            out << "\n";
            cout << "Array ordenado guardado en: " << file_path << endl;
        } else {
            cerr << "Error: No se pudo abrir el archivo de salida para escribir: " << file_path << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error de filesystem al guardar el resultado: " << e.what() << endl;
    }
}


void guardar_tiempos(const string& algoritmo, const string& caso, const string& tamano_arr, double tiempo_ms, long memoria_kb) {
    fs::path dir_path = "data/measurements/";
    
    try {
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
            cout << "Se ha creado el directorio: " << dir_path << endl;
        }

        fs::path file_path = dir_path / (caso + "_sorting_measurements.txt");
        ofstream out(file_path, ios::app);

        if (out.is_open()) {
            out << "Archivo: " << caso << " | Tamano arreglo: " << tamano_arr << "\n";
            out << "[ok] " << algoritmo << ": " << tiempo_ms << " ms " << memoria_kb << " KB\n";
            cout << "Datos guardados en: " << file_path << endl;
        } else {
            cerr << "Error: No se pudo abrir el archivo para escribir." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error de filesystem: " << e.what() << endl;
    }
}


void record(const fs::path& file) {
    ifstream in(file);
    if (!in.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de entrada: " << file << endl;
        return;
    }
    vector<int> arr;
    int x;
    while (in >> x) arr.push_back(x);
    in.close();
    
    string file_name = file.stem().string();
    string tamano_arr = file_name.substr(0, file_name.find("_"));
    
    cout << "Archivo: " << file_name << " | Tamano arreglo: " << tamano_arr << endl;

    vector<pair<string, function<std::vector<int>(std::vector<int>)>>> algoritmos = {
        {"InsertionSort", [](std::vector<int> a){ insertionSort(a); return a; }},
        //{"MergeSort", [](std::vector<int> a){ mergeSort(a); return a; }},
        //{"Quicksort", [](std::vector<int> a){ quickSort(a); return a; }},
        //{"PandaSort", [](std::vector<int> a){ return pandasort(a); }},
        //{"SortStd", [](std::vector<int> a){ return sortArray(a); }},
    };

    const string output_base_path = "data/array_output/";

    for (auto& [nombre, funcion] : algoritmos) {
        vector<int> copia = arr;

        long memoria_inicio = getCurrentMemoryUsageKB();
        auto start = cr::high_resolution_clock::now();

        vector<int> resultado = funcion(copia);

        auto end = cr::high_resolution_clock::now();
        long memoria_fin = getCurrentMemoryUsageKB();

        cr::duration<double, milli> duration = end - start;
        
        long memoria_usada;
        if (nombre == "InsertionSort" || nombre == "Quicksort" || nombre == "SortStd") {
            // Estos algoritmos son in-place, su uso de memoria es constante y despreciable
            memoria_usada = 0;
        } else {
            // Para MergeSort y PandaSort, que requieren memoria adicional O(N)
            memoria_usada = (memoria_fin - memoria_inicio);
            if (memoria_usada < 0) { // En caso de errores en la medición
                memoria_usada = 0;
            }
        }
        
        if (nombre == "PandaSort") { // PandaSort usa O(sqrt(N)) colas
            memoria_usada += sqrt(arr.size()) * sizeof(int) / 1024;
        }
        
        if (nombre == "MergeSort") { // MergeSort usa O(N) memoria temporal
            memoria_usada += arr.size() * sizeof(int) / 1024;
        }


        cout << "[ok] " << nombre << ": " << duration.count() << " ms " << memoria_usada << " KB" << endl;
        guardar_tiempos(nombre, file.filename().string(), tamano_arr, duration.count(), memoria_usada);

        fs::path output_path = fs::path(output_base_path) / (file.stem().string() + "_" + nombre + ".txt");
        guardar_array_ordenado(resultado, output_path);
    }
}


int main(){

    fs::path carpeta = "data/array_input";
    
    
    for (const auto& entry : fs::recursive_directory_iterator(carpeta))
    {
        if (fs::is_regular_file(entry.path())) {
            cout << "Procesando archivo: " << entry.path() << endl;
            record(entry.path());
        }
    }
    
    return 0;
}
