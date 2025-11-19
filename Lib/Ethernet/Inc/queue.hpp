/**
 * @file queue.hpp
 * @brief Generic queue implementation using C++ templates with C-style API
 * @author AI Assistant
 * @date 2025-11-19
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Queue structure
 * @tparam T Type of elements to store
 * @tparam SIZE Maximum capacity of the queue
 */
template<typename T, uint16_t SIZE>
struct Queue {
    T data[SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
};

/**
 * @brief Initialize queue
 * @param q Pointer to queue
 */
template<typename T, uint16_t SIZE>
void queueInit(Queue<T, SIZE>* q);

/**
 * @brief Check if queue is empty
 * @param q Pointer to queue
 * @return true if empty, false otherwise
 */
template<typename T, uint16_t SIZE>
bool queueIsEmpty(const Queue<T, SIZE>* q);

/**
 * @brief Check if queue is full
 * @param q Pointer to queue
 * @return true if full, false otherwise
 */
template<typename T, uint16_t SIZE>
bool queueIsFull(const Queue<T, SIZE>* q);

/**
 * @brief Get number of elements in queue
 * @param q Pointer to queue
 * @return Number of elements
 */
template<typename T, uint16_t SIZE>
uint16_t queueSize(const Queue<T, SIZE>* q);

/**
 * @brief Get capacity of queue
 * @return Maximum capacity
 */
template<typename T, uint16_t SIZE>
uint16_t queueCapacity(const Queue<T, SIZE>* q);

/**
 * @brief Push element to back of queue (enqueue)
 * @param q Pointer to queue
 * @param item Item to push
 * @return true if successful, false if queue is full
 */
template<typename T, uint16_t SIZE>
bool queuePushBack(Queue<T, SIZE>* q, const T& item);

/**
 * @brief Push element to front of queue
 * @param q Pointer to queue
 * @param item Item to push
 * @return true if successful, false if queue is full
 */
template<typename T, uint16_t SIZE>
bool queuePushFront(Queue<T, SIZE>* q, const T& item);

/**
 * @brief Pop element from front of queue (dequeue)
 * @param q Pointer to queue
 * @param item Pointer to store popped item
 * @return true if successful, false if queue is empty
 */
template<typename T, uint16_t SIZE>
bool queuePopFront(Queue<T, SIZE>* q, T* item);

/**
 * @brief Pop element from back of queue
 * @param q Pointer to queue
 * @param item Pointer to store popped item
 * @return true if successful, false if queue is empty
 */
template<typename T, uint16_t SIZE>
bool queuePopBack(Queue<T, SIZE>* q, T* item);

/**
 * @brief Peek at front element without removing it
 * @param q Pointer to queue
 * @param item Pointer to store peeked item
 * @return true if successful, false if queue is empty
 */
template<typename T, uint16_t SIZE>
bool queuePeekFront(const Queue<T, SIZE>* q, T* item);

/**
 * @brief Peek at back element without removing it
 * @param q Pointer to queue
 * @param item Pointer to store peeked item
 * @return true if successful, false if queue is empty
 */
template<typename T, uint16_t SIZE>
bool queuePeekBack(const Queue<T, SIZE>* q, T* item);

/**
 * @brief Get pointer to front element (without removing)
 * @param q Pointer to queue
 * @return Pointer to front element, NULL if empty
 */
template<typename T, uint16_t SIZE>
T* queueFront(Queue<T, SIZE>* q);

/**
 * @brief Get pointer to back element (without removing)
 * @param q Pointer to queue
 * @return Pointer to back element, NULL if empty
 */
template<typename T, uint16_t SIZE>
T* queueBack(Queue<T, SIZE>* q);

/**
 * @brief Clear the queue
 * @param q Pointer to queue
 */
template<typename T, uint16_t SIZE>
void queueClear(Queue<T, SIZE>* q);

#endif /* QUEUE_HPP */
