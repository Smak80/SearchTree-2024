#include "tree.h"
#include <cmath>

bool add(node*& root, int value);
node*& get(node*& root, int value);
bool remove(node*& root, int value);
void move_node(node* rem_el, node* root, node* parent, bool right);
void drop(node*& root);
int get_tree_height(const node* root);

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

/// Функция удаления элемента из списка
/// @param root коренвой элемент, определяющий дерево или поддерево
/// @param value значение, которое необходимо удалить
/// @return true, если значение было удалено и false в противном случае
bool remove(node*& root, int value)
{
	// Если корневой элемент отсутствует, удалять нечего, выходим. 
	if (!root) return false;
	// Если в корневой элемент содержит удаляемое значение
	if (root->val == value)
	{
		// Сам корневой элемент удалять нельзя, потому что в этом случае
		// структура дерева сильно исказится (будет большой дисбаланс)
		// Поэтому нужно найти элемент, как можно ниже
		if (!root->left && !root->right)
		{
			// Однако, если этот элемент - лист его можно удалить безболезненно, что и делаем
			delete root;
			root = nullptr;
		}
		else {
			// Начинаем искать самый большой элемент слева от удаляемого или самый маленький справа
			// от удаляемого
			// Для начала определим какое из поддеревьев выше и запомним полученное значение
			auto right_dir = get_tree_height(root->left) > get_tree_height(root->right);
			// Если левое поддерево выше, пойдем в ЛЕВОМ поддереве направо (right_dir = true)
			// Если правое поддерево выше, пойдем в ПРАВОМ поддереве налево (right_dir = false)
			auto& sub_tree = right_dir ? root->left : root->right;
			// Находим элемент в поддереве, из которого будем переносить значение в тот узел, где
			// сейчас находится удаляемое значение
			move_node(root, sub_tree, root, right_dir);
			// После выполнения функции в root, где раньше лежало удаляемое значение
			// будет находиться значение из одного из нижележащих узлов
			// Производим балансировку дерева, в том числе корректируем высоту корневого узла,
			// поскольку он мог измениться после удаления
			balance(root);
		}
		// Сообщаем о том, что элемент был удален из дерева
		return true;
	}
	// Если удаляемое значение не было еще найдено, определяем направление,
	// в котором его можно будет искать 
	auto& sub = value < root->val ? root->left : root->right;
	// И вызываем функцию удаления рекурсивно
	bool r = remove(sub, value);
	// Если удаление произведено (r = true), выполняем балансировку и коррекцию высоты
	if (r) balance(root);
	return r;
}

/// Функция перемещения элемента дерева
/// @param rem_el Ссылка на узел, куда будет перемещаться значение из реально удаляемого элемента
/// @param root Ссылка на корень, определяющий дерево или поддерево
/// @param parent Ссылка на узел, который является родительским для корневого элемента
/// @param right Направление движения по поддерево (true - вправо, если двигаемся в левом поддереве и false - влево, если двигаемся в правом поддереве)
void move_node(node* rem_el, node* root, node* parent, bool right)
{
	// Проверяем есть ли узел правее данного, если двигаемся вправо
	// или левее данного, если двигаемся влево
	if (right && !root->right || !right && !root->left)
	{
		// Дошли до максимального или минимального элемента в поддереве
		// У самого правого (левого) элемента могу быть дочерни элемента слева (справа)
		// Переносим ссылку на них в родительский узел относительно удаляемого (root) узла
		if (right) parent->right = root->left;
		else parent->left = root->right;
		// Переносим значения из удаляемого узла в тот узел, где находятся удаляемые значения
		rem_el->val = root->val;
		// Не забываем и про количество, которое также хранится в структуре
		rem_el->count = root->count;
		// Удаляем узел из дерева
		delete root;
		return;
	}
	// Если до самого правого (левого) узла еще не дошли, вызываем move_node рекурсивно
	move_node(rem_el, right ? root->right : root->left, root, right);
	// Проводим балансировку в дереве, поскольку высоты могли измениться после удаления.
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