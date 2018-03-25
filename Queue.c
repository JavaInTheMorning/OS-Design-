/**
 Created by abdel on 2/1/2018.
 Circular Doubly linked list implementation of Queue. Supports push, pop and peak operations.
 All operations are O(1).
*/
#include <signal.h>
#include <stdlib.h>
#include "Queue.h"

Queue *create_queue(int maxPriority) {
    Queue *queue = (Queue *) malloc(sizeof(Queue) * (maxPriority + 1));
    if (!queue) {
        return NULL;
    }

    int priority = 0;
    for (; priority < maxPriority; priority++) {
        (queue + priority)->numNodes = 0;
        (queue + priority)->head = (queue + priority)->tail = NULL;
    }

    return queue;
}

int release_queue(Queue *queue) {
    QueueNode *node = queue->head;
    for (; node != queue->tail; node = node->next) {
        free(node->data);
        free(node);
    }

    free(queue);
}

QueueNode *queue_pop(Queue *queue) {
    if (queue->numNodes == 0) {
        return NULL;
    }

    QueueNode *head = queue->head;
    if (queue->numNodes == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head->prev->next = queue->head->next;

        queue->head = head->next;
        queue->head->prev = head->prev;
    }
    queue->numNodes--;
    return head;
}

int queue_push(Queue *queue, void *element) {
    if (queue->numNodes == 0) {
        queue->head = queue->tail = (QueueNode *) malloc(sizeof(QueueNode));
        if (!queue->head) {
            perror("Could not allocate head for push.\n");
            return 0;
        }

        queue->head->next = queue->head->prev = queue->tail;
        queue->tail->next = queue->tail->prev = queue->head;

        queue->head->data = element;
    } else {
        queue->tail->next = (QueueNode *) malloc(sizeof(QueueNode));
        if (!queue->tail->next) {
            queue->tail->next = queue->head;
            perror("Could not allocate tail for push.\n");
            return 0;
        }

        queue->head->prev = queue->tail->next;

        queue->tail->next->next = queue->head;
        queue->tail->next->prev = queue->tail;
        queue->tail = queue->tail->next;

        queue->tail->data = element;
    }

    queue->numNodes++;
    return 1;
}

int queue_empty(Queue *queue) {
    return queue->numNodes == 0;
}