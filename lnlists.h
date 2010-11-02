/*
 * lnlists.h
 *      Author: Karsten König <KarstenKoenig@gmx.net>
 *      License: MIT
 *
 *      Hosted and documented on http://github.com/raymontag/Linked-Lists-in-C
 */

#ifndef LNLISTS_H_
#define LNLISTS_H_

struct _element_t {
	/*
	 * A single element within a list.
	 *
	 * @data: A pointer to an allocated variable
	 * @right: A pointer to the element rightward
	 * @left: A pointer to the element leftward
	 */

	void *data;
	struct _element_t *right;
	struct _element_t *left;
};

typedef struct _element_t element_t;

struct _head_t {
	/*
	 * The head of the list. All operations work with this
	 * structure.
	 *
	 * @first: A pointer to the first element within the list
	 * @current: A pointer to the current selected element
	 * @last: A pointer to the last element within the list
	 * @index: The index of the current element
	 * @size: The number of elements
	 */

	element_t *first;
	element_t *current;
	element_t *last;
	int index;
	int size;
};

typedef struct _head_t head_t;

/*Initialize a new list*/
head_t *new_list();

/*Append an element to the end of the list*/
element_t *add_element(head_t *list);

/*Changes the current element, to the given one*/
int change_current_element(head_t *list, element_t *element);

/*Remove all elements of the list, but the list is still usable*/
int clear_list(head_t *list);

/*Copy on list into another, so they are identical*/
int copy_list(head_t *list1, head_t *list2);

/*Delete the current element*/
int delete_element(head_t *list, ...);

/*Set the list to the first element*/
element_t *first_element(head_t *list);

/*Delete the whole list including the head*/
int free_list(head_t *list);

/*Insert element before the current one*/
element_t *insert_element(head_t *list);

/*Set the list to the last element*/
element_t *last_element(head_t *list);

/*Return the index of the current element*/
int list_index(head_t *list);

/*Return the number of elements of the list*/
int list_size(head_t *list);

/*Set the list to the element after the current one*/
element_t *next_element(head_t *list);

/*Set the list to the element before the current one*/
element_t *previous_element(head_t *list);

/*Set the list to nothing*/
int reset_list(head_t *list);

/*Set the list to the element with the given index*/
int select_element(head_t *list, int index);

/*Swap two elements with the given indexes*/
int swap_elements(head_t *list, element_t *element1, element_t *element2);

#endif /* LNLISTS_H_ */
