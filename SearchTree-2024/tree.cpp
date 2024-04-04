#include "tree.h"

bool add(node*& root, int value);
node*& get(node*& root, int value);
void remove(node*& curr, int value);
void drop(node*& root);
int get_tree_height(const node* root);

bool add(tree& t, int value)
{
	return add(t.root, value);
}

node* get(tree t, int value)
{
	return get(t.root, value);
}

void remove(tree& t, int value)
{
	remove(t.root, value);
}

void remove(node*& root, int value)
{
	if (!root) return;
	auto& to_del = get(root, value);
	if (!to_del) return;
	auto curr = to_del;
	const auto lh = get_tree_height(curr->left);
	const auto rh = get_tree_height(curr->right);
	auto parent = curr;
	if (lh == 0 && rh == 0)
	{
		delete to_del;
		to_del = nullptr;
		return;
	}
	if (lh > rh)
	{
		curr = curr->left;
		while (curr->right) {
			parent = curr;
			curr = curr->right;
		}
		parent->right = curr->left;
	} else
	{
		curr = curr->right;
		while (curr->left)
		{
			parent = curr;
			curr = curr->left;
		}
		parent->left = curr->right;
	}
	to_del->val = curr->val;
	delete curr;
}

void drop(tree& t)
{
	drop(t.root);
}

void drop(node*& root)
{
	if (root->left) drop(root->left);
	if (root->right) drop(root->right);
	delete root;
	root = nullptr;
}

node*& get(node*& root, int value)
{
	if (!root || root->val == value) return root;
	return get(value < root->val ? root->left : root->right, value);
}

bool add(node*& root, int value)
{
	if (!root) {
		node* new_node = new node;
		new_node->val = value;
		root = new_node;
		return true;
	}
	if (root->val == value)
	{
		root->count++;
		return false;
	}
	const bool res = add(value < root->val ? root->left : root->right, value);
	root->height = get_tree_height(root);
	return res;
}

int get_tree_height(const node* root)
{
	if (!root) return 0;
	const auto lh = root->left ? root->left->height : 0;
	const auto rh = root->right ? root->right->height : 0;
	return 1 + (lh > rh ? lh : rh);
}