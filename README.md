# Simulación de Carrera de Animales

Este proyecto simula una carrera entre tres animales: una tortuga, una liebre y un perro. Cada animal se mueve a diferentes velocidades y puede detenerse aleatoriamente durante la carrera, lo que añade un elemento de imprevisibilidad al resultado final.

## Características

- Tres corredores: Tortuga, Liebre y Perro.
- Cada animal tiene una velocidad diferente.
- Pausas aleatorias para la tortuga y la liebre.
- Actualización de la posición en la consola en tiempo real.
- Salida visual clara en la consola.

## Requisitos

Para compilar y ejecutar este programa, necesitas:

- Un entorno de desarrollo C++ que soporte C++11 o superior.
- Windows, ya que el programa utiliza funciones específicas de la API de Windows (como `SetConsoleOutputCP` y `SetConsoleCursorPosition`).

## Compilación y Ejecución

1. **Compilar el programa**:

   Abre tu terminal y navega al directorio donde se encuentra el archivo fuente. Luego, utiliza el siguiente comando para compilar el programa:

   ```bash
   g++ -o carrera animales.cpp -lpthread
