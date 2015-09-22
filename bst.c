/**
 * \file 	bst.c
 * \author 	Richard Degenne
 * \date 	17-09-15
 *
 * \brief 	Implementation file for the bst module
 *
 * This file defines the implementation of a binary search tree
 * library.
 *
 * \see bst.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h> // For memcpy()
#include <assert.h>

#include "bst.h"


/**
 * \brief 	Initializes a tree
 *
 * Use this function to create a new tree. Make sure you respect
 * the following conditions since some @c asserts are used to check the
 * parameters relevance.
 * 
 * To deallocate the tree, use @ref bst_destroy
 *
 * \param 	tree 	The tree to initialize
 * \param 	data_size 	The size of an element; must be > 0.
 * \param 	orderFun 	The tree's binary relation; cannot be @c NULL.
 * \param 	freeFun 	The tree's data freeing function; can be NULL.
 *
 * \warning 	Breaking any of the above conditions will cause the program to
 * abort!
 * \warning 	Not deallocating the tree with @ref bst_destroy will result in
 * memory leaks!
 *
 * \see 	bst_destroy
 */
bst* bst_new(size_t data_size, orderFun compare, freeFun free) {
	assert(data_size > 0);
	assert(compare != NULL);

	bst* tree = malloc(sizeof(bst));
	tree->data_size = data_size;
	tree->compare = compare;
	tree->free = free;
	tree->root = NULL;

	return tree;
}


/**
 * \brief 	Destroys a tree
 *
 * A call to bst_destroy deallocates a tree and all its nodes by calling
 * bst#free. Make sure the program calls it when the tree is not needed anymore.
 *
 * \param 	tree 	The tree to deallocate
 *
 * \warning 	Not using this function to deallocate a bst will cause memory
 * leaks!
 * */
void bst_destroy(bst* tree) {
	if(tree->root)
		bst_destroy_rec(tree, tree->root);
	free(tree);
}

static void bst_destroy_rec(bst* tree, bst_node* current) {
	if(current->left)
		bst_destroy_rec(tree, current->left);
	if(current->right)
		bst_destroy_rec(tree, current->right);
	if(tree->free)
		tree->free(current->data);
	free(current);
}


/**
 * \brief 	Adds a new node to a tree
 *
 * Calling this function dynamically allocates a new node and inserts it into
 * the binary search tree.
 *
 * \param 	tree 	The tree to insert the new node into
 * \param 	element 	The new node's data
 */
void bst_add(bst* tree, void* element) {
	// Creating the new node
	bst_node* node = malloc(sizeof(bst_node)); // Allocating a node
	node->data = malloc(tree->data_size); // Allocating data
	memcpy(node->data, element, tree->data_size); // Setting data
	node->left = NULL;
	node->right = NULL;

	if(tree->root)
		bst_add_rec(tree, tree->root, node); // Start recursion
	else {
		tree->root = node; // Setting the new node as the tree's root
	}
}

static void bst_add_rec(bst* tree, bst_node* current, bst_node* new) {
	if(tree->compare(current->data, new->data) >= 0) // current >= new
		if(!current->left) // current->left == NULL
			current->left = new;
		else 
			bst_add_rec(tree, current->left, new);
	else // current < new
		if(!current->right)
			current->right = new;
		else
			bst_add_rec(tree, current->right, new);
}



/**
 * \brief 	Computes the size of a tree
 *
 * \param 	tree 	Tree to compute
 *
 * \return 	The number of elements in the tree
 */
int bst_size(bst* tree) {
	if(tree->root)
		return bst_size_rec(tree->root);
	else
		return 0;
}

static int bst_size_rec(bst_node* current) {
	if(!current) // current == NULL
		return 0;
	return 1 + bst_size_rec(current->left) + bst_size_rec(current->right);
}


/**
 * \brief 	Searches a tree
 *
 * Searches tree for element. A node is considered a result whenever bst#compare
 * returns 0.
 *
 * \param 	tree 	The tree to search
 * \param 	element 	The element to search for
 *
 * \return 	A pointer to the node element was found, @c NULL if element
 * couldn't be found.
 *
 * \see 	bst#compare
 * \see 	orderFun
 * */
bst_node* bst_search(bst* tree, void* element) {
	if(tree->root)
		return bst_search_rec(tree, tree->root, element);
	return NULL;
}

static bst_node* bst_search_rec(bst* tree, bst_node* current, void* element) {
	if(!current)
		return NULL;
	if(tree->compare(current->data, element) > 0) // current > element
		bst_search_rec(tree, current->left, element);
	else if(tree->compare(current->data, element) < 0)  // current < element
		bst_search_rec(tree, current->right, element);
	else
		return current;
}


/**
 * \brief 	Iterates over a tree
 *
 * Applies a function to every element in the tree.
 *
 * \param 	tree 	The tree to iterate over
 * \param 	function 	The function to apply. Cannont be @c NULL.
 */
void bst_iter(bst* tree, iterFun function) {
	if(tree->root)
		bst_iter_rec(tree->root, function);
}

static void bst_iter_rec(bst_node* current, iterFun function) {
	if(!current)
		return;
	bst_iter_rec(current->left, function);
	function(current->data);
	bst_iter_rec(current->right, function);
}

/**
 * \brief 	Removes an element
 *
 * Removes a node (using bst#free to dynamically deallocate the node's data if
 * needed), and restores the tree structure. If there are several occurences of
 * element in tree, only the lowest-depth one will be removed.
 * 
 * \param 	tree 	Tree to remove element
 * \param 	element 	Element to remove
 */
void bst_remove(bst* tree, void* element) {
	
}
