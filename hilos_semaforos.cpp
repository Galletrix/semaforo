#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

sem_t semaforo;
pthread_mutex_t mutexSalida;

void* contar(void* argumento) {
    int id = *static_cast<int*>(argumento);

    for (int i = 1; i <= 10000; ++i) {
        // Antes y despues de la seccion critica, el semaforo protege
        // cada impresion. Entre 4000 y 6000 se conserva ocupado.
        if (i == 4000) {
            sem_wait(&semaforo);
        } else if (i < 4000 || i > 6000) {
            sem_wait(&semaforo);
        }

        pthread_mutex_lock(&mutexSalida);
        cout << "Hilo " << id << ": " << i << endl;

        if (i == 4000) {
            cout << "Hilo " << id
                 << " entra a la seccion critica." << endl;
        }

        if (i == 6000) {
            cout << "Hilo " << id
                 << " sale de la seccion critica." << endl;
        }
        pthread_mutex_unlock(&mutexSalida);

        // No liberar el semaforo dentro del intervalo 4000..6000.
        if (i == 6000 || i < 4000 || i > 6000) {
            sem_post(&semaforo);
        }
    }

    return nullptr;
}

int main() {
    pthread_t hilo1, hilo2;
    int id1 = 1;
    int id2 = 2;

    sem_init(&semaforo, 0, 1);
    pthread_mutex_init(&mutexSalida, nullptr);

    pthread_create(&hilo1, nullptr, contar, &id1);
    pthread_create(&hilo2, nullptr, contar, &id2);

    pthread_join(hilo1, nullptr);
    pthread_join(hilo2, nullptr);

    pthread_mutex_destroy(&mutexSalida);
    sem_destroy(&semaforo);

    cout << "Ambos hilos terminaron su ejecucion." << endl;
    return 0;
}
