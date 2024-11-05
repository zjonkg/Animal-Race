#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <mutex>

using namespace std;

mutex mtx; // Mutex para proteger el acceso a variables compartidas

// ╔════════════════════════════════════════════════════════════════════╗
// ║  Configura la consola para que soporte caracteres UTF-8           ║
// ╚════════════════════════════════════════════════════════════════════╝
void setConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // Establecer codificación de entrada UTF-8
}

// ╔════════════════════════════════════════════════════════════════════╗
// ║  Establece la posición del cursor en la consola                    ║
// ╚════════════════════════════════════════════════════════════════════╝
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

// ╔═══════════════════════════════════════════════════════════════════════════╗
// ║  Función principal de la carrera: Cada hilo mueve su "animal" hasta la    ║
// ║  meta o hasta que otro animal gane.                                       ║
// ╚═══════════════════════════════════════════════════════════════════════════╝
void race(string name, int& position, int max_speed, int lane, int goal, bool& race_over) {
    while (!race_over && position < goal) {
        // ╔═══════════════════════════════════════════════════════════════════╗
        // ║  Lógica para detener aleatoriamente al perro o a la liebre        ║
        // ╚═══════════════════════════════════════════════════════════════════╝
        bool shouldStop = (name == "Perro" || name == "Liebre") && (rand() % 10 < 1); // 20% de probabilidad de parar
        if (shouldStop) {
            // Simular pausa
            this_thread::sleep_for(chrono::milliseconds(300 + rand() % 300)); // Detener por un tiempo aleatorio
        }
        else {
            // ╔═══════════════════════════════════════════════════════════════╗
            // ║  Genera un avance aleatorio                                   ║
            // ╚═══════════════════════════════════════════════════════════════╝
            int advance = rand() % (max_speed + 1);

            // Bloquear el mutex antes de modificar la posición
            {
                lock_guard<mutex> lock(mtx);
                position += advance;

                // Limitar la posición a la meta
                if (position > goal) position = goal;

                // Limpiar la línea de avance del animal
                setCursorPosition(0, lane);
                cout << name << ": " << string(goal, ' ') << "\r"; // Limpiar línea
                setCursorPosition(0, lane);
                cout << name << ": " << string(position, '*') << endl; // Mostrar avance con '*'

                // Verificar si el animal ha llegado a la meta
                if (position >= goal && !race_over) {
                    race_over = true;
                    setCursorPosition(0, 5);
                    cout << "¡Ganador! " << name << " llegó a la meta." << endl;
                }
            }
        }

        // Pausas aleatorias entre movimientos
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 100));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    setConsoleUTF8();

    const int goal = 50; // Distancia de la meta
    bool race_over = false; // Variable para saber si la carrera ha terminado

    // Posiciones iniciales y velocidades de cada animal
    int tortoise_position = 0, hare_position = 0, dog_position = 0;
    const int tortoise_speed = 1, hare_speed = 3, dog_speed = 2;
    const int tortoise_lane = 1, hare_lane = 2, dog_lane = 3;

    // ╔═══════════════════════════════════════════════════════════════════╗
    // ║  Crear threads para cada corredor y lanzar la carrera             ║
    // ╚═══════════════════════════════════════════════════════════════════╝
    thread t1(race, "Tortuga", ref(tortoise_position), tortoise_speed, tortoise_lane, goal, ref(race_over));
    thread t2(race, "Liebre", ref(hare_position), hare_speed, hare_lane, goal, ref(race_over));
    thread t3(race, "Perro", ref(dog_position), dog_speed, dog_lane, goal, ref(race_over));

    // ╔═══════════════════════════════════════════════════════════════════╗
    // ║  Esperar a que terminen los threads                               ║
    // ╚═══════════════════════════════════════════════════════════════════╝
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
