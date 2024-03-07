#include <iostream>
#include "BSTree.h"	


int main() {
	BinarySearchTree<int> bt;
	BinarySearchTree<int> bt2 = { -5, 0, 31, 15, 6, 9 };

	bt.add(-5);
	bt.add(0);
	bt.add(31);
	bt.add(15);
	bt.add(6);
	bt.add(9);


	std::cout << (bt == bt2) << std::endl;

	//std::cout << bt.compareTree(bt2);
	return 1;
}