# Reader-Writer Problem using Semaphores – Satellite Health Records

## Aim
To implement the Reader-Writer problem using semaphores, where multiple scientists can read ISRO satellite health records simultaneously, while only one engineer can update the records at a time.

## Algorithm
1. Initialize the semaphores `mutex = 1` and `wrt = 1`, and the variable `readcount = 0`.
2. Scientist (Reader) process:
3. `wait(mutex); readcount++; if readcount == 1, wait(wrt); signal(mutex).`
4. Read the satellite health record from the ISRO database.
5. `wait(mutex); readcount--; if readcount == 0, signal(wrt); signal(mutex).`
6. Engineer (Writer) process:
7. `wait(wrt).`
8. Update the satellite health record in the ISRO database.
9. `signal(wrt).`
10. End of the Reader-Writer process.

## Program (`rw.c`)
```c
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
```

## Compile and Run
```bash
gcc rw.c -o reader -pthread
./reader
```

## Definitions
- **Shared Resource:** The common data item, file or database accessed by both readers and writers (here, the satellite health records).
- **Reader:** A process or thread that only reads data and does not change it. Multiple readers can access the resource together.
- **Writer:** A process or thread that modifies or updates the shared data. It requires total, exclusive access.
- **Mutual Exclusion:** A rule ensuring that only one process, or a specific group of compatible processes, accesses a resource at any given time.
- **Critical Section:** The specific part of the program code where processes access or modify shared resources.
- **Semaphore:** A synchronization variable used to ensure exclusive access for writers and to block readers when writing occurs.
- **Mutex:** A binary semaphore or lock used to protect the counter variable (`readcount`) when multiple readers update it simultaneously.
- **Starvation:** A situation where a waiting process is delayed indefinitely because other processes keep getting priority. In this reader-priority solution a writer can starve if new readers keep arriving.

## Domain and Use Case
**Domain:** Operating Systems (Process Synchronization and Concurrency Control) applied to Space Technology and Database Systems.

**Use Case:** Space agencies such as ISRO continuously store health data of satellites (battery level, temperature, signal status and so on). Many scientists need to read this data at the same time to monitor the satellite, while an engineer occasionally has to update the records. Using the `mutex` and `wrt` semaphores, any number of scientists can read together, but when an engineer is updating, no one else can read or write. This avoids inconsistent or half-updated data. The same method is used in databases, file systems, banking systems and booking systems where reads are frequent and writes are rare.

## Output
The `reader_writer_output.png` image is the output screenshot included in the lab manual.

## Result
The Reader-Writer problem using semaphores is implemented to allow multiple readers to read simultaneously while providing exclusive access to the writer.
