void memory_load_thread_func(float *a, int size) {

  for (int i = 0; i < size; i += 8) {
    float batch[8]; // create array of 8 elements on each loop

    for (int j = 0; j < 8; j++) {
      batch[j] = a[i + j]; // load current i + j to batch
    }

    memory_to_trig.enq_8(batch); // send over 8 array floats
  }
}

void trig_thread_func(int size) {

  for (int i = 0; i < size; i += 8) {

    float batch[8];
    memory_to_trig.deq_8(batch);

    for (int j = 0; j < 8; j++) {
      batch[j] = cos(batch[j]);
    }

    trig_to_memory.enq_8(batch);
  }
}

void memory_store_thread_func(float *a, int size) {

  for (int i = 0; i < size; i += 8) {

    float batch[8];
    trig_to_memory.deq_8(batch);

    for (int j = 0; j < 8; j++) {
      a[i + j] = batch[j];
    }
  }
}