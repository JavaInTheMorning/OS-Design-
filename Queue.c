/**
 Created by abdel on 2/1/2018.
 Circular Doubly linked list implementation of Queue. Supports push, pop and peak operations.
 All operations are O(1).
*/
#include "Queue.h"

Queue *create_queue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }

    queue->numNodes = 0;
    queue->head = queue->tail = NULL;
    return queue;
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
    if (!queue->head) {
        queue->head = queue->tail = (QueueNode *) malloc(sizeof(QueueNode));
        if (!queue->head) {
            return 0;
        }

        queue->head->next = queue->head->prev = queue->tail;
        queue->tail->next = queue->tail->prev = queue->head;

        queue->head->data = element;
    } else {
        queue->tail->next = (QueueNode *) malloc(sizeof(QueueNode));
        if (!queue->tail->next) {
            queue->tail->next = queue->head;
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

void headToTail(Queue* queue) {
    if (queue->numNodes < 2) {
        return;
    }

    QueueNode* node = queue_pop(queue);
    if (!node) {
        return;
    }

    queue_push(queue, node->data);
    free(node);
}