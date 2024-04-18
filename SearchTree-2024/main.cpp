#include <iostream>
#include "tree.h"

using namespace std;

enum bypass
{
	prefix, infix, postfix
};

void show_tree(node* root, bypass type)
{
	if (!root) return;
	switch(type)
	{
		case prefix:
		{
			cout << root->val << "(" << root->count << ") ";
			show_tree(root->left, type);
			show_tree(root->right, type);
			break;
		}
		case infix:
		{
			show_tree(root->left, type);
			cout << root->val << "(" << root->count << ") ";
			show_tree(root->right, type);
			break;
		}
		case postfix:
		{
			show_tree(root->left, type);
			show_tree(root->right, type);
			cout << root->val << "(" << root->count << ") ";
			break;
		}
	}
}

void show_tree(tree t, bypass type)
{
	show_tree(t.root, type);
	cout << endl;
}

void main()
{
	setlocale(LC_ALL, "");
	int a[] = { 2, 7, 3, 10, 1, 3, 3, 5, 6, 6 };
	tree t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		add(t, a[i]);
	}
	show_tree(t, infix);
	remove(t, 3);
	show_tree(t, prefix);
	remove(t, 10);
	show_tree(t, postfix);
	drop(t);
	show_tree(t, infix);
}