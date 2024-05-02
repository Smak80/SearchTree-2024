#include <iostream>
#include "tree.h"
#include "queue.h"

using namespace std;

enum bypass
{
	prefix, infix, postfix, wide
};

queue<node*> wide_bypass(node* tree)
{
	queue<node*> q;
	enqueue(q, tree);
	int count = 1;
	auto h = get_tree_height(tree);
	auto curr = q.first;
	while (count < (1 << h) - 1)
	{
		if (curr->data) {
			enqueue(q, curr->data->left);
			enqueue(q, curr->data->right);
		}
		else
		{
			enqueue<node*>(q, nullptr);
			enqueue<node*>(q, nullptr);
		}
		count += 2;
		curr = curr->next;
	}
	return q;
}

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
		case wide:
		{
			auto q = wide_bypass(root);
			auto curr = q.first;
			auto h = get_tree_height(root);
			auto tree_elem_count = (1 << h) - 1;
			for (int l = 1; l <= h; l++)
			{
				int elem_on_row = 0;
				auto space_count = (1 << (h - l)) - 1;
				while (elem_on_row < tree_elem_count) {
					for (int i = 0; i < space_count; i++)
					{
						cout << " ";
						elem_on_row++;
					}
					if (curr->data) cout << curr->data->val; else cout << "=";
					elem_on_row++;
					for (int i = 0; i < space_count; i++)
					{
						cout << " ";
						elem_on_row++;
					}
					if (elem_on_row < tree_elem_count) {
						cout << " ";
						elem_on_row++;
					} else
					{
						cout << "\n";
					}
					curr = curr->next;
				}
			}
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
	int a[] = { 2, 7, 3, 9, 1, 3, 3, 5, 6, 6 };
	tree t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		add(t, a[i]);
	}
	show_tree(t, wide);
	cout << endl;
	show_tree(t, infix);
	remove(t, 3);
	show_tree(t, prefix);
	remove(t, 10);
	show_tree(t, postfix);
	drop(t);
	show_tree(t, infix);
}