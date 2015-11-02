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


bst* bst_new(size_t data_size, orderFun compare, freeFun free_fun) {
	assert(data_size > 0);
	assert(compare);

	bst* tree = malloc(sizeof(bst));
	if(!tree) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	tree->data_size = data_size;
	tree->compare = compare;
	tree->free = free_fun;
	tree->root = NULL;

	return tree;
}


void bst_destroy(bst* tree) {
	assert(tree);
	if(tree->root)
		bst_destroy_rec(tree, tree->root);
	free(tree);
}

static void bst_destroy_rec(bst* tree, bst_node* current) {
	if(current->left)
		bst_destroy_rec(tree, current->left);
	if(current->right)
		bst_destroy_rec(tree, current->right);
	bst_destroy_node(tree, current);
}

static void bst_destroy_node(bst* tree, bst_node* node) {
	if(tree->free)
		tree->free(node->data);
	free(node->data);
	free(node);
}


void bst_add(bst* tree, void* element) {
	assert(tree);
	assert(element);

	// Creating the new node
	bst_node* node = bst_create_node(tree->data_size, element);

	if(tree->root)
		bst_add_rec(tree->compare, tree->root, node); // Start recursion
	else {
		tree->root = node; // Setting the new node as the tree's root
	}
}

static void bst_add_rec(orderFun compare, bst_node* current, bst_node* new) {
	if(compare(current->data, new->data) >= 0) // current >= new
		if(!current->left) // current->left == NULL
			current->left = new;
		else 
			bst_add_rec(compare, current->left, new);
	else // current < new
		if(!current->right)
			current->right = new;
		else
			bst_add_rec(compare, current->right, new);
}

static bst_node* bst_create_node(int data_size, void* element) {
	bst_node* node = malloc(sizeof(bst_node)); // Allocating a node
	if(!node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	node->data = malloc(data_size); // Allocating data
	if(!(node->data)) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(node->data, element, data_size); // Setting data
	node->left = NULL;
	node->right = NULL;

	return node;
}


int bst_size(bst* tree) {
	assert(tree);
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


bst_node* bst_search(bst* tree, void* element) {
	assert(tree);
	if(tree->root)
		return bst_search_rec(tree->compare, tree->root, element);
	return NULL;
}

static bst_node* bst_search_rec(orderFun compare, bst_node* current, void* element) {
	if(!current)
		return NULL;
	if(compare(current->data, element) > 0) // current > element
		bst_search_rec(compare, current->left, element);
	else if(compare(current->data, element) < 0)  // current < element
		bst_search_rec(compare, current->right, element);
	else
		return current;
}


void bst_iter(bst* tree, iterFun function) {
	assert(tree);
	assert(function);
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


void bst_remove(bst* tree, void* element) {
	assert(tree);
	if(tree->root)
		bst_remove_rec(tree, tree->root, element);
}

static bst_node* bst_remove_rec(bst* tree, bst_node* current, void* element) {
	if(!current)
		return NULL;
	if(tree->compare(current->data, element) > 0)
		current->left = bst_remove_rec(tree, current->left, element);
	else if(tree->compare(current->data, element) < 0)
		current->right = bst_remove_rec(tree, current->right, element);
	else {
		if(!current->left && !current->right) { // No children
			bst_destroy_node(tree, current);
			return NULL;
		}
		else if(!current->right) { // Only a left child
			bst_node* temp = current->left;
			bst_destroy_node(tree, current);
			return temp;
		}
		else if(!current->left) { // Only a right child
			bst_node* temp = current->right;
			bst_destroy_node(tree, current);
			return temp;
		}
		else { // Left and right children
			bst_node* successor = find_max(current->left);
			memcpy(current->data, successor->data, tree->data_size);
			current->left = bst_remove_rec(tree, current->left, successor->data);
		}
	}
	return current;
}

static bst_node* find_max(bst_node* current) {
	if(!current->right)
		return current;
	return find_max(current->right);
}
