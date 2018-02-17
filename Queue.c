/**
 Created by abdel on 2/1/2018.
 Circular Doubly linked list implementation of Queue. Supports push, pop and peak operations.
 All operations are O(1).
*/

#include <malloc.h>
#include <stdio.h>

#include "Queue.h"

Queue *create_queue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }

    queue->nodeCount = 0;
    queue->head = queue->tail = NULL;
    return queue;
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

    queue->nodeCount++;
    return 1;
}

QueueNode *queue_pop(Queue *queue) {
    if (queue->nodeCount == 0) {
        return NULL;
    }

    QueueNode *head = queue->head;
    if (queue->nodeCount == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head->prev->next = queue->head->next;

        queue->head = head->next;
        queue->head->prev = head->prev;
    }
    queue->nodeCount--;
    return head;
}

void traverse_q(Queue *queue) {
    QueueNode *ptr = queue->head;
    printf("FRONT -> ");
    do {
        printf("Value: %d ", (ptr->data));

        ptr = ptr->next;

    } while (ptr != queue->head);

    ptr = queue->tail;
    printf("\nBack -> ");
    do {
        printf("Value: %d ", (ptr->data));

        ptr = ptr->prev;

    } while (ptr != queue->tail);

    printf("\nTotal queue size: %d\n\n", queue->nodeCount);
}

void test_q() {
    Queue* queue = create_queue();
    if (!queue) {
        return;
    }

    queue_push(queue, 50);
    queue_push(queue, 60);
    queue_push(queue, 90);
    queue_push(queue, 100);

    do {
        traverse_q(queue);

        QueueNode *node = queue_pop(queue);
        printf("Top should be gone: %d\n", (node->data));

        free(node);
    } while (queue->nodeCount != 0);

    free(queue);
}