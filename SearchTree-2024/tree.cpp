#include "tree.h"
#include <cmath>

bool add(node*& root, int value);
node*& get(node*& root, int value);
bool remove(node*& root, int value);
void move_node(node* rem_el, node* root, node* parent, bool right);
void drop(node*& root);

void small_left_rotate(node*& root);
void small_right_rotate(node*& root);
void large_left_rotate(node*& root);
void large_right_rotate(node*& root);

void balance(node*& root);

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

/// ������� �������� �������� �� ������
/// @param root �������� �������, ������������ ������ ��� ���������
/// @param value ��������, ������� ���������� �������
/// @return true, ���� �������� ���� ������� � false � ��������� ������
bool remove(node*& root, int value)
{
	// ���� �������� ������� �����������, ������� ������, �������. 
	if (!root) return false;
	// ���� � �������� ������� �������� ��������� ��������
	if (root->val == value)
	{
		// ��� �������� ������� ������� ������, ������ ��� � ���� ������
		// ��������� ������ ������ ��������� (����� ������� ���������)
		// ������� ����� ����� �������, ��� ����� ����
		if (!root->left && !root->right)
		{
			// ������, ���� ���� ������� - ���� ��� ����� ������� �������������, ��� � ������
			delete root;
			root = nullptr;
		}
		else {
			// �������� ������ ����� ������� ������� ����� �� ���������� ��� ����� ��������� ������
			// �� ����������
			// ��� ������ ��������� ����� �� ����������� ���� � �������� ���������� ��������
			auto right_dir = get_tree_height(root->left) > get_tree_height(root->right);
			// ���� ����� ��������� ����, ������ � ����� ��������� ������� (right_dir = true)
			// ���� ������ ��������� ����, ������ � ������ ��������� ������ (right_dir = false)
			auto& sub_tree = right_dir ? root->left : root->right;
			// ������� ������� � ���������, �� �������� ����� ���������� �������� � ��� ����, ���
			// ������ ��������� ��������� ��������
			move_node(root, sub_tree, root, right_dir);
			// ����� ���������� ������� � root, ��� ������ ������ ��������� ��������
			// ����� ���������� �������� �� ������ �� ����������� �����
			// ���������� ������������ ������, � ��� ����� ������������ ������ ��������� ����,
			// ��������� �� ��� ���������� ����� ��������
			balance(root);
		}
		// �������� � ���, ��� ������� ��� ������ �� ������
		return true;
	}
	// ���� ��������� �������� �� ���� ��� �������, ���������� �����������,
	// � ������� ��� ����� ����� ������ 
	auto& sub = value < root->val ? root->left : root->right;
	// � �������� ������� �������� ����������
	bool r = remove(sub, value);
	// ���� �������� ����������� (r = true), ��������� ������������ � ��������� ������
	if (r) balance(root);
	return r;
}

/// ������� ����������� �������� ������
/// @param rem_el ������ �� ����, ���� ����� ������������ �������� �� ������� ���������� ��������
/// @param root ������ �� ������, ������������ ������ ��� ���������
/// @param parent ������ �� ����, ������� �������� ������������ ��� ��������� ��������
/// @param right ����������� �������� �� ��������� (true - ������, ���� ��������� � ����� ��������� � false - �����, ���� ��������� � ������ ���������)
void move_node(node* rem_el, node* root, node* parent, bool right)
{
	// ��������� ���� �� ���� ������ �������, ���� ��������� ������
	// ��� ����� �������, ���� ��������� �����
	if (right && !root->right || !right && !root->left)
	{
		// ����� �� ������������� ��� ������������ �������� � ���������
		// � ������ ������� (������) �������� ���� ���� ������� �������� ����� (������)
		// ��������� ������ �� ��� � ������������ ���� ������������ ���������� (root) ����
		if (right) parent->right = root->left;
		else parent->left = root->right;
		// ��������� �������� �� ���������� ���� � ��� ����, ��� ��������� ��������� ��������
		rem_el->val = root->val;
		// �� �������� � ��� ����������, ������� ����� �������� � ���������
		rem_el->count = root->count;
		// ������� ���� �� ������
		delete root;
		return;
	}
	// ���� �� ������ ������� (������) ���� ��� �� �����, �������� move_node ����������
	move_node(rem_el, right ? root->right : root->left, root, right);
	// �������� ������������ � ������, ��������� ������ ����� ���������� ����� ��������.
	balance(root);
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
	balance(root);
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

void small_left_rotate(node*& root)
{
	node* new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;
	root->height = get_tree_height(root);
	root = new_root;
	root->height = get_tree_height(root);
}

void small_right_rotate(node*& root)
{
	node* new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;
	root->height = get_tree_height(root);
	root = new_root;
	root->height = get_tree_height(root);
}

void large_left_rotate(node*& root)
{
	small_right_rotate(root->right);
	small_left_rotate(root);
}

void large_right_rotate(node*& root)
{
	small_left_rotate(root->left);
	small_right_rotate(root);
}

void balance(node*& root)
{
	auto lh = get_tree_height(root->left);
	auto rh = get_tree_height(root->right);
	if (abs(lh - rh) >= 2)
	{
		if (lh > rh)
		{
			if (get_tree_height(root->left->left) > get_tree_height(root->left->right))
				small_right_rotate(root);
			else
				large_right_rotate(root);
		} else
		{
			if (get_tree_height(root->right->right) > get_tree_height(root->right->left))
				small_left_rotate(root);
			else
				large_left_rotate(root);
		}
	} else
	{
		root->height = get_tree_height(root);
	}
}