#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, wrt;
int readcount = 0;

void* reader(void* id) {
    sem_wait(&mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&wrt);
    sem_post(&mutex);
    printf("Scientist %ld reading satellite record\n", (long)id);
    sem_wait(&mutex);
    readcount--;
    if (readcount == 0)
        sem_post(&wrt);
    sem_post(&mutex);
    return NULL;
}

void* writer(void* id) {
    sem_wait(&wrt);
    printf("Engineer updating satellite record\n");
    sem_post(&wrt);
    return NULL;
}

int main() {
    pthread_t r[3], w;
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    for (long i = 0; i < 3; i++)
        pthread_create(&r[i], NULL, reader, (void*)i);
    pthread_create(&w, NULL, writer, (void*)1);
    for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);
    pthread_join(w, NULL);
    return 0;
}
