#include <iostream>
#include "BSTree.h"	


int main() {
	BinarySearchTree<int> bt;

	bt.add(5);
	bt.add(-5);
	bt.add(0);
	bt.add(31);
	bt.add(15);
	bt.add(6);
	

	bt.PrintTree();
	std::cout << bt.size();
	return 1;
}