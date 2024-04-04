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
			cout << root->val << " ";
			show_tree(root->left, type);
			show_tree(root->right, type);
			break;
		}
		case infix:
		{
			show_tree(root->left, type);
			cout << root->val << " ";
			show_tree(root->right, type);
			break;
		}
		case postfix:
		{
			show_tree(root->left, type);
			show_tree(root->right, type);
			cout << root->val << " ";
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
	int a[] = { 2, 7, 3, 10, 1, 5, 12, 4 };
	size_t sz = 8;
	tree t;
	for (int i = 0; i< sz; i++)
	{
		add(t, a[i]);
	}
	show_tree(t, infix);
	remove(t, 7);
	show_tree(t, prefix);
	remove(t, 4);
	show_tree(t, postfix);
	drop(t);
	show_tree(t, infix);
}