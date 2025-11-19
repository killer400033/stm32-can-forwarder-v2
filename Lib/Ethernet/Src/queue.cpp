#include "queue.hpp"

// Template function implementations

template<typename T, uint16_t SIZE>
void queueInit(Queue<T, SIZE>* q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

template<typename T, uint16_t SIZE>
bool queueIsEmpty(const Queue<T, SIZE>* q) {
    return q->count == 0;
}

template<typename T, uint16_t SIZE>
bool queueIsFull(const Queue<T, SIZE>* q) {
    return q->count >= SIZE;
}

template<typename T, uint16_t SIZE>
uint16_t queueSize(const Queue<T, SIZE>* q) {
    return q->count;
}

template<typename T, uint16_t SIZE>
uint16_t queueCapacity(const Queue<T, SIZE>* q) {
    return SIZE;
}

template<typename T, uint16_t SIZE>
bool queuePushBack(Queue<T, SIZE>* q, const T& item) {
    if (queueIsFull(q)) {
        return false;
    }
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % SIZE;
    q->count++;
    return true;
}

template<typename T, uint16_t SIZE>
bool queuePushFront(Queue<T, SIZE>* q, const T& item) {
    if (queueIsFull(q)) {
        return false;
    }
    q->head = (q->head == 0) ? (SIZE - 1) : (q->head - 1);
    q->data[q->head] = item;
    q->count++;
    return true;
}

template<typename T, uint16_t SIZE>
bool queuePopFront(Queue<T, SIZE>* q, T* item) {
    if (queueIsEmpty(q)) {
        return false;
    }
    *item = q->data[q->head];
    q->head = (q->head + 1) % SIZE;
    q->count--;
    return true;
}

template<typename T, uint16_t SIZE>
bool queuePopBack(Queue<T, SIZE>* q, T* item) {
    if (queueIsEmpty(q)) {
        return false;
    }
    q->tail = (q->tail == 0) ? (SIZE - 1) : (q->tail - 1);
    *item = q->data[q->tail];
    q->count--;
    return true;
}

template<typename T, uint16_t SIZE>
bool queuePeekFront(const Queue<T, SIZE>* q, T* item) {
    if (queueIsEmpty(q)) {
        return false;
    }
    *item = q->data[q->head];
    return true;
}

template<typename T, uint16_t SIZE>
bool queuePeekBack(const Queue<T, SIZE>* q, T* item) {
    if (queueIsEmpty(q)) {
        return false;
    }
    uint16_t back_idx = (q->tail == 0) ? (SIZE - 1) : (q->tail - 1);
    *item = q->data[back_idx];
    return true;
}

template<typename T, uint16_t SIZE>
T* queueFront(Queue<T, SIZE>* q) {
    if (queueIsEmpty(q)) {
        return nullptr;
    }
    return &q->data[q->head];
}

template<typename T, uint16_t SIZE>
T* queueBack(Queue<T, SIZE>* q) {
    if (queueIsEmpty(q)) {
        return nullptr;
    }
    uint16_t back_idx = (q->tail == 0) ? (SIZE - 1) : (q->tail - 1);
    return &q->data[back_idx];
}

template<typename T, uint16_t SIZE>
void queueClear(Queue<T, SIZE>* q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}