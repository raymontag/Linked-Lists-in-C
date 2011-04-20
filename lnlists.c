/*
 * lnlist.c
 *      Author: Karsten-Kai König <KKoenig@posteo.de>
 *      License: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lnlists.h"


head_t *new_list() {
	/*
	 *Initialize a new list
	 *
	 *The return value is a pointer to the new list or NULL if
	 *the operation was not successful
	 */

	head_t *new = calloc(1, sizeof(head_t));

	if (new == NULL)
		return NULL;

	new->first = NULL;
	new->current = NULL;
	new->last = NULL;
	new->index = -1;
	new->size = 0;

	return new;
}

element_t *add_element(head_t *list) {
	/*
	 * Append an empty element to the end of the list. The element
	 * becomes the current element of the list.
	 *
	 * @list: The list
	 *
	 * Return a pointer to the new element or NULL
	 */

	element_t *new;
	new = calloc(1, sizeof(element_t));
	if (new == NULL) {
		fprintf(stderr, "<add_element>: Failed to allocate memory for a new element!\n");
		return NULL;
	}

	if (list->size == 0) {
		new->left = NULL;
		new->right = NULL;

		list->first = new;
		list->current = new;
		list->last = new;
		list->size = 1;
		list->index = 0;
	}
	else if (list->size > 0) {
		new->left = list->last;
		new->right = NULL;

		list->current->right = new;
		list->current = new;
		list->last = new;
		list->size += 1;
		list->index = list->size-1;
	}
	else if (list->size < 0) {
		fprintf(stderr, "<add_element>: Negative size of a list is not possible; list is broken!\n");
		free(new);
		return NULL;
	}

	return new;
}

int change_current_element(head_t *list, element_t *element) {
	/*
	 * Changes the current element, to the given one. The
	 * element must exist in the list.
	 *
	 * @list: The list
	 * @element: A pointer to the element
	 *
	 * The function returns 1 in the case of success or 0 if
	 * the operations was not successful
	 */

	int i;
	element_t *tmp;

	tmp = list->first;

	for (i = 1; i <= list->size; i++) {
		if (tmp == element) {
			list->index = i-1;
			break;
		}
		if (tmp->right == NULL) {
			fprintf(stderr, "<change_current_element>: The element doesn't exist in the list!\n");
			return 0;
		}
		tmp = tmp->right;
	}

	list->current = element;

	return 1;
}

int clear_list(head_t *list) {
	/*
	 * Remove all elements of the list, but the list is still
	 * usable
	 *
	 * @list: The list
	 *
	 * The return value is 1
	 */

	int i;
	element_t *tmp1, *tmp2;

	tmp1 = list->first;

	for(i = 1; i <= list->size; i++) {
		tmp2 = tmp1->right;
		free(tmp1);
		tmp1 = tmp2;
	}

	list->current = NULL;
	list->first = NULL;
	list->last = NULL;
	list->size = 0;
	list->index = -1;

	return 1;
}

int copy_list(head_t *list1, head_t *list2) {
	/*
	 * Copy on list into another, so they are identical
	 *
	 * @list1: The origin list
	 * @list2: The destination of list1
	 *
	 * The return value is 1
	 */

	clear_list(list2);
	list2->first = list1->first;
	list2->current = list1->current;
	list2->last = list1->last;
	list2->size = list1->size;
	list2->index = list1->index;

	return 1;
}

int delete_element(head_t *list, ...) {
	/*
	 * Delete the current element and sets the current element
	 * to the previous one. If the deleted element was the
	 * first one the new current element is NULL. If you set
	 * the optional argument to 1 than the current element will
	 * be the second one.
	 *
	 * @list: The list
	 * @optional: 1 or nothing (if it's something else nothing
	 * will happen)
	 *
	 * Returns 1 or 0, if the operation was not successful
	 */

	va_list ptr;
	int flag;
	element_t *current;

	va_start(ptr, *list);
	flag = va_arg(ptr, int);
	va_end(ptr);
	if (flag != 1) {
		flag = 0;
	}

	current = list->current;
	if (current == NULL) {
		fprintf(stderr, "<delete_element>: There is no current element!}\n");
		return 0;
	}
	if (list->first == current) {
		if (list->size > 1)
			current->right->left = NULL;
		if (flag) {
			list->current = current->right;
			list->index += 1;
		}
		else {
			list->current = NULL;
			list->index -= 1;
		}
	}
	else if (list->last == current) {
		if (list->size > 1)
			current->left->right = NULL;
		list->current = current->left;
		list->index -= 1;
	}
	else {
		current->left->right = current->right;
		current->right->left = current->left;
		list->current = current->left;
		list->index -= 1;
	}

	list->size -= 1;
	free(current);

	return 1;
}

element_t *first_element(head_t *list) {
	/*
	 * Set the list to the first element.
	 *
	 * @list: The address of a list head
	 *
	 * Returns a pointer to the first element
	 */

	list->current = list->first;
	list->index = 0;
	return list->current;
}

int free_list(head_t *list) {
	/*
	 * Delete the whole list including it's head, so the memory
	 * is no longer allocated and the list is no longer usable
	 *
	 * @list: The list
	 *
	 * Returns 1
	 */

	int i;
	element_t *tmp1, *tmp2;

	tmp1 = list->first;

	for(i = 1; i <= list->size; i++) {
		tmp2 = tmp1->right;
		free(tmp1);
		tmp1 = tmp2;
	}

	free(list);

	return 1;
}

element_t *insert_element(head_t *list) {
	/*
	 * Inserts an empty element before the current one. If the
	 * list is empty the new element is the first. The new
	 * element will be the current one.
	 *
	 * @list: The list
	 *
	 * Returns a pointer to the new element.
	 */

	element_t *new;

	new = calloc(1, sizeof(element_t));
	if (new == NULL) {
		fprintf(stderr, "<insert_element>: Failed to allocate memory for a new element!\n");
		return NULL;
	}

	if (list->size == 0) {
		list->first = new;
		list->current = new;
		list->last = new;
		list->size = 1;
		list->index = 0;

		new->right = NULL;
		new->left = NULL;
	}
	else if (list->size > 0) {
		new->right = list->current;
		new->left = list->current->left;

		if (list->current == list->first) {
			list->current->left = new;
			list->first = new;
		}
		else {
			list->current->left->right = new;
			list->current->left = new;
		}
		list->current = new;
		list->size++;
	}
	else if (list->size < 0) {
		fprintf(stderr, "<insert_element>: Negative size of a list is not possible; list is broken!\n");
		free(new);
		return NULL;
	}

	return new;
}

element_t *last_element(head_t *list) {
	/*
	 * Sets the current element of the given list to it's last
	 * one.
	 *
	 * @list: The list
	 *
	 * Returns a pointer to the last element or NULL.
	 */

	list->current = list->last;
	list->index = list->size-1;
	return list->current;
}

int list_index(head_t *list) {
	/*
	 * Returns the index number of the current element starting
	 * with 0.
	 *
	 * @list: The list
	 *
	 * Returns the index starting with 0 for the first element.
	 * Returns -1 if there is no current element.
	 */

	return list->index;
}

int list_size(head_t *list) {
	/*
	 * Returns the number of elements in the list.
	 *
	 * @list: The list
	 *
	 * The return value is the size of the list.
	 */

	return list->size;
}

element_t *next_element(head_t *list) {
	/*
	 * Set the current element to the next one. If there is no
	 * current element, the next element is the first.
	 *
	 * @list: The list
	 *
	 * The function returns a pointer to the new current element
	 * or NULL if it's the end of the list.
	 */

	if (list->current != NULL) {
		if (list->current->right != NULL) {
			list->current = list->current->right;
			list->index += 1;
			return list->current;
		}
		else
			return NULL;
	}
	else if (list->current == NULL && list->size != 0) {
		list->current = list->first;
		list->index += 1;
		return list->current;
	}
	else {
		fprintf(stderr, "<next_element>: There are no elements in the list!\n");
		return NULL;
	}
}

element_t *previous_element(head_t *list) {
	/*
	 * Sets the current element to the list to the previous one.
	 *
	 * @list: The list
	 *
	 * Returns the pointer to the new current element or NULL
	 * if the list is empty or if the old current element is the
	 * first one. If there is no current element, the return
	 * value is NULL, too.
	 */

	if (list->current != NULL && list->current != list->first) {
		list->current = list->current->left;
		list->index -= 1;
		return list->current;
	}
	else
		return NULL;
}

int reset_list(head_t *list) {
	/*
	 * Sets the current element to NULL, so there is no current
	 * element.
	 *
	 * @list: The list
	 *
	 * Returns 1
	 */

	list->current = NULL;
	list->index = -1;

	return 1;
}

int select_element(head_t *list, int index) {
	/*
	 * Sets the current element to that one with the specified
	 * index. The indexes start with 0.
	 *
	 * @list: The list
	 * @index: The index
	 *
	 * Returns 1 if everything was fine or 0
	 */

	int i;

	if (index > list->size-1 || index < 0) {
		fprintf(stderr, "<select_element>: The given index doesn't exists!\n");
		return 0;
	}
	else {
		list->current = list->first;
		for (i = 0; i < index; i++) {
			list->current = list->current->right;
		}
	}

	list->index = index;
	return 1;
}

int swap_elements(head_t *list, element_t *element1, element_t *element2) {
	/*
	 * This function swaps the given elements. They must be in
	 * the same list.
	 *
	 * @list: The list
	 * @element1: Pointer to the 1st element
	 * @element2:Pointer to the 2nd element
	 *
	 * Returns 1 if everything was fine or 0
	 */

	int i, counter;
	element_t *tmp;

	counter = 0;
	tmp = list->first;

	for (i = 1; i <= list->size; i++) {
		if (tmp == element1 || tmp == element2) {
			if (counter < 1) {
				counter++;
				continue;
			}
			break;
		}
		if (tmp->right == NULL) {
			fprintf(stderr, "<swap_elements>: One ore more elements doesn't exist in the list!\n");
			return 0;
		}
		tmp = tmp->right;
	}

	if (element1 == list->first && element2 == list->last) {
		element1->right->left = element2;
		element2->left->right = element1;
		list->first = element2;
		list->last = element1;
	}
	else if (element2 == list->first && element1 == list->last) {
		element2->right->left = element1;
		element1->left->right = element2;
		list->first = element1;
		list->last = element2;
	}
	else if (element1 == list->first) {
		element1->right->left = element2;
		element2->right->left = element1;
		element2->left->right = element1;
		list->first = element2;
	}
	else if (element1 == list->last) {
		element1->left->right = element2;
		element2->right->left = element1;
		element2->left->right = element1;
		list->last = element2;
	}
	else if (element2 == list->first) {
		element2->right->left = element1;
		element1->right->left = element2;
		element1->left->right = element2;
		list->first = element1;
	}
	else if (element2 == list->last) {
		element2->left->right = element1;
		element1->right->left = element2;
		element1->left->right = element2;
		list->last = element1;
	}
	else {
		element1->left->right = element2;
		element1->right->left = element2;
		element2->right->left = element1;
		element2->left->right = element1;
	}

	tmp = (element_t *) malloc(sizeof(element_t));
	tmp->right = element1->right;
	tmp->left = element1->left;

	element1->right = element2->right;
	element1->left = element2->left;

	element2->right = tmp->right;
	element2->left = tmp->left;

	free(tmp);
	return 1;
}
