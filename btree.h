#ifndef BTREE_H
#define BTREE_H

typedef struct		s_btree
{
	void* value;
	struct s_btree	*left;
	struct s_btree	*right;
}					t_btree;

t_btree*	btree_create_node(void* value);
t_btree*	btree_insert(t_btree **root, void* value, int (*cmp)());
t_btree*	btree_find(t_btree *root, void *value, int (*cmp)());
t_btree*	btree_erase(t_btree* root, void* value, int (*cmp)(), void (*del)());
void		btree_apply_suffix(t_btree *root, void (*applyf)(void*));
void		btree_apply_prefix(t_btree *root, void (*applyf)(void*));
void		btree_apply_infix(t_btree *root, void (*applyf)(void*));
void		btree_clear(t_btree** root);

#endif
