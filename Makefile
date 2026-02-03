# -------------------------------------------------------------------
# Makefile para la Práctica 5: Sistemas Cliente-Servidor (Sockets)
# -------------------------------------------------------------------

# Compilador y flags
CXX = g++

# CXXFLAGS corregidos:
# -I. : Busca en el directorio actual.
# -Ilibrerias/Socket : Permite hacer #include "Socket.hpp" directamente.
# -Ilibrerias/MultiBuffer : Permite hacer #include "MultiBuffer.hpp" directamente.
CXXFLAGS = -std=c++11 -pthread -I. -Ilibrerias/Socket -Ilibrerias/MultiBuffer -g

# Lista de ejecutables a generar
TARGETS = ServidorTareas ServidorMatriz Cliente

# Regla por defecto
all: $(TARGETS)

# -------------------------------------------------------------------
# Reglas de enlazado (Linking)
# -------------------------------------------------------------------

ServidorTareas: ServidorTareas.o Socket.o mtBuffer.o
	$(CXX) $(CXXFLAGS) -o ServidorTareas ServidorTareas.o Socket.o mtBuffer.o

ServidorMatriz: ServidorMatriz.o Socket.o mtResultados.o
	$(CXX) $(CXXFLAGS) -o ServidorMatriz ServidorMatriz.o Socket.o mtResultados.o

Cliente: Cliente.o Socket.o
	$(CXX) $(CXXFLAGS) -o Cliente Cliente.o Socket.o

# -------------------------------------------------------------------
# Reglas de compilación de objetos (.o)
# -------------------------------------------------------------------

# -- Librería Socket --
# CORRECCIÓN: La ruta real es librerias/Socket/...
Socket.o: librerias/Socket/Socket.cpp librerias/Socket/Socket.hpp
	$(CXX) $(CXXFLAGS) -c librerias/Socket/Socket.cpp -o Socket.o

# -- Monitores y Clases Auxiliares --

# Monitor del Buffer de Tareas
# Depende de MultiBuffer.hpp que está en librerias/MultiBuffer/
mtBuffer.o: mtBuffer.cpp mtBuffer.hpp librerias/MultiBuffer/MultiBuffer.hpp
	$(CXX) $(CXXFLAGS) -c mtBuffer.cpp

# Monitor de Resultados
mtResultados.o: mtResultados.cpp mtResultados.hpp
	$(CXX) $(CXXFLAGS) -c mtResultados.cpp

# -- Archivos Principales --

ServidorTareas.o: ServidorTareas.cpp mtBuffer.hpp librerias/MultiBuffer/MultiBuffer.hpp librerias/Socket/Socket.hpp
	$(CXX) $(CXXFLAGS) -c ServidorTareas.cpp

ServidorMatriz.o: ServidorMatriz.cpp mtResultados.hpp librerias/Socket/Socket.hpp
	$(CXX) $(CXXFLAGS) -c ServidorMatriz.cpp

Cliente.o: Cliente.cpp librerias/Socket/Socket.hpp
	$(CXX) $(CXXFLAGS) -c Cliente.cpp

# -------------------------------------------------------------------
# Limpieza
# -------------------------------------------------------------------
clean:
	rm -f *.o $(TARGETS)