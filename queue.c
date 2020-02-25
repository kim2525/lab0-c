#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;  // if malloc failed return NULL
    q->head = NULL;
    q->tail = NULL;
    q->q_size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /*free all value and list element*/
    for (list_ele_t *tmp = q->head; !tmp; tmp = tmp->next) {
        free(tmp->value);
        free(tmp);
    }
    /*free the queue*/
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *value;
    value = malloc(sizeof(s));
    if (!value) {
        free(newh);
        return false;
    }
    /*strcpy is not safe, used snprintf instead*/
    snprintf(value, strlen(s) + 1, "%s", s);
    newh->value = value;
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    q->q_size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *value;
    value = malloc(sizeof(s));
    if (!value) {
        free(newh);
        return false;
    }
    /*strcpy is not safe, used snprintf instead*/
    snprintf(value, strlen(s) + 1, "%s", s);
    newh->value = value;
    if (!q->tail) {
        q->tail = newh;
        q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->q_size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    snprintf(sp, bufsize, "%s", q->head->value);
    q->head = q->head->next;
    free(q->head);
    q->q_size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->q_size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;
    list_ele_t *next, *cur, *last;
    last = NULL;
    cur = q->head;
    q->head = q->tail;
    q->tail = cur;
    while (cur) {
        next = cur->next;
        cur->next = last;
        last = cur;
        cur = next;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge_sort(list_ele_t *head, int size)
{
    if (!head || !head->next)
        return head;
    list_ele_t *left, *right, *mid;
    mid = head;
    for (int i = 0; i < (size / 2) - 1; i++)
        mid = mid->next;
    left = head;
    right = mid->next;
    mid->next = NULL;
    left = merge_sort(left, (size / 2));
    right = merge_sort(right, (size - size / 2));

    list_ele_t *merge = NULL;
    while (left != NULL || right != NULL) {
        while (left != NULL &&
               (right == NULL || strcmp(left->value, right->value) < 0)) {
            if (merge == NULL)
                head = merge = left;
            else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        }
        while (right != NULL &&
               (left == NULL || strcmp(right->value, left->value) <= 0)) {
            if (merge == NULL)
                head = merge = right;
            else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        }
    }
    return head;
}
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->q_size == 1)
        return;
    q->head = merge_sort(q->head, q->q_size);
}
