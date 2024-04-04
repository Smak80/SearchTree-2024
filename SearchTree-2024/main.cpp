#include <iostream>
#include "tree.h"

using namespace std;

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
	remove(t, 7);
	remove(t, 4);
	drop(t);

}