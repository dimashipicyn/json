#include "btree.h"

#include <stdlib.h>
#include <stdio.h>

t_btree* btree_create_node(void* value)
{
	t_btree	*btree_node;

	btree_node = calloc(1, sizeof(t_btree));
	if (btree_node == NULL)
		return (NULL);
	btree_node->left = NULL;
	btree_node->right = NULL;
	btree_node->value = value;
	return (btree_node);
}

t_btree* btree_insert(t_btree **root, void* value, int (*cmp)(void*, void*))
{
	if (*root == NULL) {
		*root = btree_create_node(value);
		return *root;
	}
	if ((*cmp)((*root)->value, value) < 0)
		return btree_insert(&(*root)->right, value, cmp);
	else if ((*cmp)((*root)->value, value) > 0)
		return btree_insert(&(*root)->left, value, cmp);
	
	(*root)->value = value;
	return (*root);
}

t_btree* btree_find(t_btree *root, void *value, int (*cmp)(void*, void*))
{
	if (root == NULL || (*cmp)(root->value, value) == 0) 
		return (root);
	if ((*cmp)(root->value, value) > 0)
		return btree_find(root->left, value, cmp);
	else
		return btree_find(root->right, value, cmp);
}

void	btree_apply_suffix(t_btree *root, void (*applyf)(void*))
{
	if (root == NULL)
		return;
	btree_apply_suffix(root->left, applyf);
	btree_apply_suffix(root->right, applyf);
	(*applyf)(root->value);
}

void	btree_apply_prefix(t_btree *root, void (*applyf)(void*))
{
	if (root == NULL)
		return;
	(*applyf)(root->value);
	btree_apply_prefix(root->left, applyf);
	btree_apply_prefix(root->right, applyf);
}

void	btree_apply_infix(t_btree *root, void (*applyf)(void*))
{
	if (root == NULL)
		return;
	btree_apply_infix(root->left, applyf);
	(*applyf)(root->value);
	btree_apply_infix(root->right, applyf);
}

t_btree* btree_erase(t_btree* root, void* value, int (cmp)(void*, void*), void (*del)())
{
	if (root == NULL) {
		return NULL;
	}

	if (cmp(root->value, value) < 0) {
		root->right = btree_erase(root->right, value, cmp, del);
	}
	else if (cmp(root->value, value) > 0) {
		root->left = btree_erase(root->left, value, cmp, del);
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			return NULL;
		}
		if (root->left == NULL) {
			return root->right;
		}
		if (root->right == NULL) {
			return root->left;
		}
		// note
	}
}

void btree_clear(t_btree** root)
{
	if (*root == NULL) {
		return;
	}
	btree_clear(&(*root)->left);
	btree_clear(&(*root)->right);
	if ((*root)->left == NULL && (*root)->right == NULL) {
		free(*root);
		*root = NULL;
	}
}