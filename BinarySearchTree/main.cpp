#include <iostream>
#include "BSTree.h"	


int main() {
	char carr[] = "abc", carr2[] = "def";
	BinarySearchTree<char> v0;
	BinarySearchTree<char> v1(carr, carr + 3);

	v0.clear();
	std::pair<BinarySearchTree<char>::iterator, bool> pib = v0.insert('d');
	pib = v0.insert('d');
	v0.insert(v0.begin(), 'e');
	v0.insert(carr, carr + 3);
	v0.insert(carr2, carr2 + 3);
	v0.clear();
	v0 = { 'd','a','e','b','f','c'};

	v0.PrintTree();
	//v0.swapWithMostLeft(v0.find('a'));
	v0.erase(v0.find('a'));
	std::cout << "\n\n\n\n";
	v0.PrintTree();

	//v0.erase(v0.find('a'));
	//v0.PrintTree();
	//daebfc
	//std::cout << *(v0.find('a'));
	//for (char c : v0) std::cout << c << std::endl;
	//std::cout << v0.find('d').parent().isFake() << std::endl;
	//std::cout << v0.find('d').left().isFake() << std::endl;
	//std::cout << v0.find('d').right().isFake() << std::endl;
	//v0.erase(v0.begin()+1);
	//std::cout << v0.size() << std::endl;

	//v0.erase(v0.begin(), ++v0.begin());
	//v0.erase('x');
	//v0.erase_leaf(v0.find('c'));
	//v0.erase('d');
	//v0.erase(v0.find('f'));
	//v0.erase(v0.find('e'));
	//v0.PrintTree();
	return 1;
}