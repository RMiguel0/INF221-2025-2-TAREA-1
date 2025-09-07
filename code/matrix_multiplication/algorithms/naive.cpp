// Codigo obtenido desde la pagina: https://www.geeksforgeeks.org/cpp/cpp-matrix-multiplication
// Solo tiene agregadas dos casos de errores que implemente por si llegaba a fallar en cualquier
// ejecucion.

#include <iostream>
#include <vector>

using namespace std;
using Matrix = std::vector<std::vector<int>>;

void mulMat(vector<vector<int>>& m1, vector<vector<int>>& m2, Matrix& res) {
    if (m1.empty() || m2.empty()) {
        cerr << "Error: Las matrices de entrada no pueden estar vacías." << endl;
        return;
    }

    size_t n = m1.size();
    if (m2.size() != n || m1[0].size() != n || m2[0].size() != n) {
        cerr << "Error: Las matrices deben ser cuadradas y del mismo tamaño." << endl;
        return;
    }

    res.assign(n, vector<int>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}
