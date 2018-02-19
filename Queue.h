//
// Created by abdel on 2/1/2018.
//

#ifndef USER_THREADS_QUEUE_H
#define USER_THREADS_QUEUE_H

#include <malloc.h>
#include <stdio.h>

/**
 * Doubly linked node.
 */
typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
    struct QueueNode *prev;
} QueueNode;

/**
 * Double linked Queue with access to both head and tail nodes.
 */
typedef struct {
    int numNodes;

    QueueNode *head;
    QueueNode *tail;
} Queue;

/**
 * Constructs an empty Doubly Circular Linked List Queue.
 * @return An instance of the @Queue struct or NULL if the @Queue could not be allocated.
 */
Queue *create_queue();

/**
 * Pops the 'head' of the @Queue.
 * @return The 'head' @QueueNode reference. Needs to be freed to avoid memory leaks.
 */
QueueNode *queue_pop(Queue *);

/**
 * Creates a new instance of @QueueNode struct and inserts into the 'tail' end of the Queue.
 * @return One if the operation was successful, Zero if the operation could not be performed.
 */
int queue_push(Queue *, void *);

/**
 * Moves the @QueueNode from the front of the queue to the tail.
 */
void headToTail(Queue *);

#endif //USER_THREADS_QUEUE_H