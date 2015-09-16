/**
 * \file 	bst.h
 * \author 	Richard Degenne
 * \date 	16-09-15
 *
 * \brief 	Header file for the bst module
 *
 * This file defines the structure and interface of a binary search tree
 * library.
 */

#ifndef _BST_H
#define _BST_H

#include <stdlib.h>
#include <stdio.h>


/**
 * \brief 	Binary relation
 * 
 * To work, the dataset of a BST must be <i>ordered</i>. This function type
 * must return an @c int : a positive number if the first argument is <i>greater
 * than</i> the second argument, and a negative number otherwise. A zero (0)
 * return value will be considered an equality, thus it will lead to the right
 * sub-tree while adding or removing nodes, and will also be considered a valid
 * result for a search.
 *
 * \see bst_add
 * \see bst_remove
 * \see bst_search
 */
typedef int (*orderFun)(void *, void *);


/**
 * \brief 	Free function
 *
 * This function type allows the user to specify a way to dynamically
 * deallocate memory when a tree node is destroyed, to avoid memory leaks.
 */
typedef void (*freeFun)(void *);


/**
 * \brief 	Iteration function
 *
 * This function type is used when iterating over BST's elements.
 *
 * \see 	bst_iter
 */
typedef void (*iterFun)(void *);

/**
 * \brief 	Node structure
 *
 * This describes the structure of a node. The tree_node#data type is @c void*
 * to be as generic as possible. See the bst structure for further details about
 * how is implemented the tree data type.
 *
 * \see 	bst
 */
typedef struct _tree_node {
	void* data; 	/**< The node's data */
	_tree_node left; 	/**< The node's left sub-tree */
	_tree_node right; 	/**< The node's right sub-tree */
} tree_node;


/**
 * \brief 	Binary search tree structure
 *
 * This is the structure of a binary search tree.
 *
 * \warning 	Do not manipulate the structure directly. Use the functions of the
 * bst interface instead.
 */
typedef struct {
	size_t data_size; /**< Size of an element */
	tree_node* root; /**< Root of the bst */
	orderFun compare; /**< Binary relation of the tree dataset */
	freeFun free; /**< Dynamic deallocation of tree data */
} bst;


/**
 * \brief 	Initializes a tree
 *
 * Use this function to initialize the tree attributes. Make sure you respect
 * the following conditions since some @c asserts are used to check the
 * parameters relevance.
 *
 * \param 	tree 	The tree to initialize
 * \param 	data_size 	The size of an element; must be > 0.
 * \param 	orderFun 	The tree's binary relation; cannot be @c NULL.
 * \param 	freeFun 	The tree's data freeing function; can be NULL.
 *
 * \warning 	Breaking any of the above conditions will cause the program to
 * abort!
 */
void bst_new(bst* tree, size_t data_size, orderFun compare, freeFun free);


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
void bst_destroy(bst* tree);


/**
 * \brief 	Adds a new node to a tree
 *
 * Calling this function dynamically allocates a new node and inserts it into
 * the binary search tree.
 *
 * \param 	tree 	The tree to insert the new node into
 * \param 	element 	The new node's data
 */
void bst_add(bst* tree, void* element);


/**
 * \brief 	Computes the size of a tree
 *
 * \param 	tree 	Tree to compute
 *
 * \return 	The number of elements in the tree
 */
int bst_size(bst* tree);


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
tree_node* bst_search(bst* tree, void* element);


/**
 * \brief 	Iterates over a tree
 *
 * Applies a function to every element in the tree.
 *
 * \param 	tree 	The tree to iterate over
 * \param 	function 	The function to apply. Cannont be @c NULL.
 */
void bst_iter(bst* tree, iterFun function);


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
void bst_remove(bst* tree, void* element);


#endif
