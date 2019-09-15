#pragma once

#include "threads.h"
void barrier_init(int n) {
  pthread_cond_init(&thread_barrier.complete, NULL);
  pthread_mutex_init(&thread_barrier.mutex, NULL);
  thread_barrier.count = n;
  thread_barrier.crossing = 0;
}

void barrier_cross() {
  pthread_mutex_lock(&thread_barrier.mutex);
  /* One more thread through */
  thread_barrier.crossing++;
  /* If not all here, wait */
  if (thread_barrier.crossing < thread_barrier.count) {
    pthread_cond_wait(&thread_barrier.complete, &thread_barrier.mutex);
  } else {
    pthread_cond_broadcast(&thread_barrier.complete);
    /* Reset for next time */
    thread_barrier.crossing = 0;
  }
  pthread_mutex_unlock(&thread_barrier.mutex);
}

void initializeSystem(config_t& cfg) {
  srand((int)time(0));
  barrier_init(cfg.threads + 1);
}

thread_data_t constructThreadData(
  int id, 
  unsigned int seed, 
  unsigned int keyRange, 
  unsigned int iters, 
  float deposit, 
  simplemap_t<int, float>* accounts,
  bool lockfree
) {
  return (thread_data_t) {
    .id = id,
    .seed = seed,
    .keyRange = keyRange,
    .iters = iters,
    .deposit = deposit,
    .nb_deposits = 0,
    .deposits = 0,
    .nb_balances = 0,
    .balances = 0,
    .nb_aborts = 0,
    .accounts = accounts,
    .lockfree = lockfree
  };
}

void printMultithreaded(const config_t &cfg, const thread_data_t* data) {
  double totalTime = 0;
  int deposits = 0;
  int balances = 0;
  for (int i = 0; i < cfg.threads; i++){
    totalTime += (data[i].finish.tv_sec - data[i].start.tv_sec)
              +  (data[i].finish.tv_nsec - data[i].start.tv_nsec) / 1000000000.0;
    deposits  += data[i].nb_deposits;
    balances  += data[i].nb_balances;
  }
  std::cout << "------------------------------MULTI THREADED-------------------------" << std::endl;
  std::cout << "Performance:" << std::endl
            << "Total time " << totalTime << std::endl
            << "Time per thread " << totalTime / cfg.threads << std:: endl
            << "Balances " << balances << " (" << balances * 1.0 / (balances + deposits) * 100 << "%)" << std::endl
            << "Deposits " << deposits << " (" << deposits * 1.0 / (balances + deposits) * 100 << "%)" << std::endl;
}

void printSinglethreaded(const thread_data_t datum) {
  double totalTime = (datum.finish.tv_sec - datum.start.tv_sec)
                   + (datum.finish.tv_nsec - datum.start.tv_nsec) / 1000000000.0;
  int deposits = datum.nb_deposits;
  int balances = datum.nb_balances;

  std::cout << "------------------------------SINGLE THREADED-------------------------" << std::endl;
  std::cout << "Performance:" << std::endl
            << "Total time " << totalTime << std::endl
            << "Balances " << balances << " (" << balances * 1.0 / (balances + deposits) * 100 << "%)" << std::endl
            << "Deposits " << deposits << " (" << deposits * 1.0 / (balances + deposits) * 100 << "%)" << std::endl;
}
