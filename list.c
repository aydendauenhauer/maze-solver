/*
 * File:	list.c
 *
 * Copyright:	2023, Ayden Dauenhauer
 *
 * Description:	This file will contain the necessary functions to run a maze and radix through the use of a doubly linked list with a head and tail pointer
 * 		The stack and queue operations can be done through the use of 2 add functions and 2 remove functions at the start and ends of the list.
 */

# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include <stdbool.h>
# include "list.h"

// Node data structure and definition
typedef struct node {
        void **data;
        struct node *next;
        struct node *prev;
        int first;
        int count;
        int length;
} NODE;

// List data structure and definition
typedef struct list {
        int count;
        int numNodes;
	NODE *head;
	NODE *tail;
} LIST;

// Creates and returns a linked list lp with a sentinel node where each doubly linked list starts
// Runtime: O(1)
LIST *createList(void) {
	LIST *lp = malloc(sizeof(LIST));
	assert(lp != NULL);

	lp->numNodes = 0;
	lp->count = 0;

	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);

	lp->tail = lp->head;
	lp->head->next = NULL;
	lp->head->prev = NULL;
	return lp;
}

// Deletes all elements in a list and frees the memory associated
// Runtime: O(1)
void destroyList(LIST *lp) {
	assert(lp != NULL);

	NODE *p = lp->head;

	while(p != NULL){
		free(p->data);
		p = p->next;
	}

	free(lp);
}

// Returns the number of elements in a list, lp
// Runtime: O(1)
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->count;
}

// Adds an element into the doubly linked list at the start of the list
// Runtime: O(1)
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
	if (lp->numNodes == 0 || lp->head->count == lp->head->length) {
		NODE *temp = malloc(sizeof(NODE));
		assert(temp != NULL);

		temp->first = 0;
		temp->length = 8;
		temp->count = 0;

		temp->data = malloc(sizeof(void*)*temp->length);
		assert(temp->data != NULL);

		temp->data[temp->first] = item;
		temp->prev = NULL;
		temp->next = NULL;

		if (lp->numNodes == 0) {
			lp->head = temp;
			lp->tail = temp;
		}

		else if (lp->head->count == lp->head->length) {
			lp->head->prev = temp;
			temp->next = lp->head;
			lp->head = temp;
		}

		lp->numNodes++;
	}

	int idx = (lp->head->first + lp->head->count) % lp->head->length;
	lp->head->data[idx] = item;
	lp->head->count++;
	lp->count++;
}

// Adds an element into the doubly linked list at the end of the list
// Runtime: O(1)
void addLast(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);

	if (lp->numNodes == 0 || lp->tail->count == lp->tail->length) {
		NODE *temp;

		temp = malloc(sizeof(NODE));
		assert(temp != NULL);

		temp->length = 8;
		temp->first = 0;

		temp->data = malloc(sizeof(void*) * temp->length);
		assert(temp->data != NULL);

		temp->count = 0;
		temp->prev = NULL;
		temp->next = NULL;

		if (lp->numNodes == 0) {
			lp->head = temp;
			lp->tail = temp;
		}

		else if (lp->tail->count == lp->tail->length) {
			temp->prev = lp->tail;
			lp->tail->next = temp;
			lp->tail = temp;
		}

		lp->numNodes++;
	}

	int idx = (lp->tail->first + lp->tail->count) % lp->tail->length;
	lp->tail->data[idx] = item;
	lp->tail->count++;
	lp->count++;
}

// Removes an item from the list in its first position
// Runtime: O(1)
void *removeFirst(LIST *lp) {
	assert(lp != NULL && lp->count > 0);
	void *dataDel;

	if (lp->head->count == 0) {
		NODE *deleted = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(deleted);
		lp->numNodes--;
	}

	dataDel = lp->head->data[lp->head->first];
	lp->head->data[lp->head->first] = NULL;
	lp->head->first = (lp->head->first + 1) % lp->head->length;
	lp->head->count--;
	lp->count--;
	return dataDel;
}

// Removes an item from the list in its last position
// Runtime: O(1)
void *removeLast(LIST *lp) {
	assert(lp != NULL && lp->count > 0);

	void *dataDel;

	if(lp->tail->count == 0) {
		NODE *deleted = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(deleted);
		lp->numNodes--;
	}

	int idx = (lp->tail->first + lp->tail->count - 1) % lp->tail->length;
	dataDel = lp->tail->data[idx];
	lp->tail->count--;
	lp->count--;
	return dataDel;
}

// Returns the first element in the list
// Runtime: O(1)
void *getFirst(LIST *lp) {
        assert(lp != NULL);
        return lp->head->next->data;
}

// Returns the last element in the list
// Runtime: O(1)
void *getLast(LIST *lp) {
        assert(lp != NULL);
        return lp->tail->data;
}

// Returns the element at the given index
// Runtime: O(n)
void *getItem(LIST *lp, int index) {
	assert(lp != NULL && index >= 0 && index < lp->count);
	NODE *temp = lp->head;
	int i, x;

	for(i = 0; i < lp->count; i += temp->count) {
		if (index < temp->count) {
			x = index;
			break;
		}

		else {
			index-= temp->count;
		}

		temp = temp->next;
	}

	int idx = (temp->first + x) % temp->length;
	return temp->data[idx];
}

// Sets the element at the given index to the item
// Runtime: O(n)
void setItem(LIST *lp, int index, void *item) {
	assert(lp != NULL && item != NULL);

	int counter = index;
	NODE *temp = lp->head;

	while (temp != NULL) {
		if((counter - temp->count) < 0) {
			temp->data[(temp->first + counter) % temp->length] = item;
			return;
		}

		counter = counter - temp->count;
		temp = temp->next;
	}
}
