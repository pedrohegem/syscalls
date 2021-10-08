// Copyright pedrohegem
#include <barrier.h>

int barrier_init(barrier_t barrier, int nrThreads) {
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->tQueue, NULL);

  barrier->nrThreads = nrThreads;
  barrier->count = 0;

  return 0;
}

int barrier_block(barrier_t barrier) {
  pthread_mutex_lock(&barrier->mutex);

  barrier->count++;
  if (barrier->count < barrier->nrThreads) {
      pthread_cond_wait(&barrier->tQueue, &barrier->mutex);
  } else {
      pthread_cond_broadcast(&barrier->tQueue);
      barrier->count = 0;
  }

  pthread_mutex_unlock(&barrier->mutex);
  return 0;
}
