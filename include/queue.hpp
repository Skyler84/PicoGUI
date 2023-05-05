#pragma once

#include "pico/util/queue.h"

template<class T>
class Queue{
public:
  Queue(size_t count){
    queue_init(&queue, sizeof(T), count);
  }
  ~Queue(){
    queue_free(&queue);
  }
  bool try_add(const T& t){
    return queue_try_add(&queue, &t);
  }
  bool try_remove(T& t){
    return queue_try_remove(&queue, &t);
  }
  bool is_empty() const{
    return queue_is_empty(&queue);
  }
  bool is_full() const{
    return queue_is_full(&queue);
  }


private:
  queue_t queue;
};