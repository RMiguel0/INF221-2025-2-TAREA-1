# Compilador
CXX = g++
CXXFLAGS = -Wall

# Archivos fuente
SRC = sorting.cpp \
      ./algorithms/insertionsort.cpp \
      ./algorithms/mergesort.cpp \
      ./algorithms/pandasort.cpp \
      ./algorithms/quicksort.cpp \
      ./algorithms/sort.cpp

# Archivos objeto
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = sorting

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
