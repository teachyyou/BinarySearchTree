#include <iostream>
#include "BSTree.h"	


int main() {
	BinarySearchTree<int> bt;
	BinarySearchTree<int> bt2 = { 8, 3, 10, 4 ,-1, -11, 7, 0 };

	bt.add(5);
	bt.add(-5);
	bt.add(0);
	bt.add(31);
	bt.add(15);
	bt.add(6);

	bt.erase(5);


	std::cout << bt.size();
	bt.PrintTree();
	return 1;
}