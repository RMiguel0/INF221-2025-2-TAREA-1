# Compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ialgorithms

# Archivos fuente
SRC = matrix_multiplication.cpp \
      ./algorithms/naive.cpp \
      ./algorithms/strassen.cpp \

# Archivos objeto
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = multiplication

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Cómo compilar los .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(TARGET)
