#pragma once
#include <iostream>
#include <iterator>
#include <memory>

template<typename T, class Comparator = std::less<T>, class Allocator = std::allocator<T>>
class BinarySearchTree {

private:

	class Node {
	public:
		Node* parent;
		Node* left;
		Node* right;
		T data;

		bool isFake = false;

		Node(T value = T(), Node* p = nullptr, Node* l = nullptr, Node* r = nullptr) : parent(p), left(l), right(r), data(value) {}

		/*bool operator==(Node* other) {
			return (data == other->data);
		}*/
	};
	class iterator {
		friend class BinarySearchTree<T>;
	protected:
		Node* _node;
		explicit iterator(Node* nd) : _node(nd) {};
		Node* node() {
			return _node;
		}
		iterator parent() const {
			return iterator(_node->parent);
		}
		iterator left() const {
			return iterator(_node->left);
		}
		iterator right() const {
			return iterator(_node->right);
		}

		bool isFake() const {
			return _node == nullptr || _node->isFake;
		}
	public:
		const T& operator*() {
			return _node->data;
		}
		iterator& step_up() {
			_node = _node->parent;
			return *this;
		}
		iterator& step_left() {
			_node = _node->left;
			return *this;
		}
		iterator& step_right() {
			_node = _node->right;
			return *this;
		}

		iterator& operator++() {
			if (isFake()) {
				step_left();
				return *this;
			}
			if (!right().isFake()) {
				step_right();
				while (!left().isFake()) step_left();
				return *this;
			}
			iterator prev = *this;
			step_up();
			while (!isFake() && (right() == prev)) {
				prev = *this;
				step_up();
			}
			return *this;
		}

		iterator& operator--() {
			if (isFake()) {
				step_right();
				return *this;
			}
			if (!left().isFake()) {
				step_left();
				while (!right().isFake()) step_right();
				return *this;
			}
			iterator prev = *this;
			step_up();
			while (!isFake() && (left() == prev)) {
				prev = *this;
				step_up();
			}
			return *this;
		}

		iterator operator++(int) {
			iterator iter(*this);
			++this;
			return iter;
		}
		iterator operator--(int) {
			iterator iter(*this);
			--this;
			return iter;
		}

		iterator operator+(int a) const {
			iterator tmp = iterator(_node);
			for (int i = 0; i < a; i++) ++tmp;
			return tmp;
		}

		iterator operator-(int a) const {
			iterator tmp = iterator(_node);
			for (int i = 0; i < a; i++) --tmp;
			return tmp;
		}



		friend bool operator== (const iterator& it1, const iterator& it2)
		{
			return it1._node == it2._node;
		}

		friend bool operator!= (const iterator& it1, const iterator& it2)
		{
			return it1._node != it2._node;
		}

		iterator GetMin() {
			iterator temp = *this;
			while (!temp.left().isFake())
				temp = temp.left();
			return temp;
		}
		iterator GetMax() {
			iterator temp = *this;
			while (!temp.right().isFake())
				temp = temp.right();
			return temp;
		}

		void setParent(const iterator& other) {
			_node->parent = other._node;
		}
		void setRight(const iterator& other) {
			_node->right = other._node;
		}
		void setLeft(const iterator& other) {
			_node->left = other._node;
		}
		bool isRight() const {
			return parent().right() == *this;
		}
		bool isLeft() const {
			return parent().left() == *this;
		}

	};

	Comparator comparator = Comparator();
	using Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
	Alloc alloc;

	//Node* root;
	Node* fake_root;
	size_t _size = 0;

	void fake_root_initialize() {
		fake_root = alloc.allocate(1);

		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->parent));
		fake_root->parent = nullptr;
		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->right));
		fake_root->right = nullptr;
		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->left));
		fake_root->left = nullptr;

		fake_root->isFake = true;

	}

	void fake_root_delete() {
		std::allocator_traits<Alloc>::destroy(alloc, fake_root->parent);
		std::allocator_traits<Alloc>::destroy(alloc, fake_root->right);
		std::allocator_traits<Alloc>::destroy(alloc, fake_root->left);
		std::allocator_traits<Alloc>::deallocate(alloc, fake_root, 1);

	}

	Node* newNode(const T& value, Node* parent, Node* left, Node* right) {
		Node* newnode = alloc.allocate(1);

		std::allocator_traits<Alloc>::construct(alloc, &(newnode->parent));
		newnode->parent = parent;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->left));
		newnode->left = left;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->right));
		newnode->right = right;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->data));
		newnode->data = value;
		newnode->isFake = false;

		return newnode;
	}
	void clearNode(Node* node) {

		std::allocator_traits<Alloc>::destroy(alloc, &(node->data));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->parent));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->right));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->left));
		std::allocator_traits<Alloc>::deallocate(alloc, node, 1);

	}




	Node* copyByRecursion(Node* original, const Node* original_fake)
	{

		Node* lsb;
		if (original->left != original_fake) {
			lsb = copyByRecursion(original->left, original_fake);
		}
		else {
			lsb = fake_root;
		}

		Node* rsb;
		if (original->right != original_fake) {
			rsb = copyByRecursion(original->right, original_fake);
		}
		else {
			rsb = fake_root;
		}

		Node* current = newNode(original->data, nullptr, lsb, rsb);
		if (original->right != original_fake) current->right->parent = current;
		if (original->left != original_fake) current->left->parent = current;
		return current;
	}

	static bool checkByRecursion(Node* node1, Node* node2) {
		if (node1->isFake && node2->isFake) {
			return true;
		}

		if (node1->isFake || node2->isFake) {
			return false;
		}

		return (node1->data == node2->data) && checkByRecursion(node1->left, node2->left) && checkByRecursion(node1->right, node2->right);
	}
	void swapWithMostLeft(iterator node) {
		iterator toSwap = node.left().GetMax();

		//case 1
		if (node.left() == toSwap) {
			toSwap.setParent(node.parent());
			node.setLeft(toSwap.left());

			if (!toSwap.left().isFake()) {
				toSwap.left().setParent(node);
			}
			else {
				if (fake_root->left == toSwap.node()) {
					fake_root->left = node.node();
				}
			}
			toSwap.setLeft(node);

			node.right().setParent(toSwap);
			toSwap.setRight(node.right());
			node.node()->right = fake_root;

			if (!node.parent().isFake()) {
				if (node.isRight()) {
					node.parent().setRight(toSwap);
				}
				else {
					node.parent().setLeft(toSwap);
				}
			}
			else {
				if (fake_root->parent == node.node()) {
					fake_root->parent = toSwap.node();
				}
			}
			node.setParent(toSwap);
			return;

		}
		//case 2
		toSwap.parent().setRight(node);

		if (node.parent().isFake()) {
			fake_root->parent = toSwap.node();
		}
		else {
			if (node.isRight()) {
				node.parent().setRight(toSwap);
			}
			else {
				node.setLeft(toSwap);
			}
		}
		node.right().setParent(toSwap);
		toSwap.setRight(node.right());
		node.node()->right = fake_root;

		iterator temp = node.left();

		if (!toSwap.left().isFake()) {
			toSwap.left().setParent(node);
		}
		node.setLeft(toSwap.left());

		temp.setParent(toSwap);
		toSwap.setLeft(temp);

		iterator temp1 = node.parent();

		node.setParent(toSwap.parent());
		toSwap.setParent(temp1);
	}

	void printNode(const Node* current, int width = 0) const {
		std::string spaces = "";
		for (int i = 0; i < width; ++i) spaces += "  ";
		if (current == fake_root) {
			std::cout << spaces << "Fake\n";
			return;
		}
		printNode(current->right, width + 3);
		std::cout << spaces << current->data << std::endl;
		printNode(current->left, width + 3);
	}
public:

	friend bool operator==(const BinarySearchTree& first, const BinarySearchTree& second) {
		return (first._size == second._size) && checkByRecursion(first.fake_root->parent, second.fake_root->parent);
	}

	friend bool operator!=(const BinarySearchTree& first, const BinarySearchTree& second) {
		return !(first==second);
	}
	
	iterator begin() const {
		return iterator(fake_root->left);
	}
	iterator end() const {
		return iterator(fake_root);
	}

	BinarySearchTree() {
		fake_root_initialize();
	}

	BinarySearchTree(std::initializer_list<T> list) : BinarySearchTree() {
		for (T x : list) add(x);
	}

	BinarySearchTree(const BinarySearchTree& other) : BinarySearchTree() {
		_size = other._size;
		if (_size == 0) return;

		fake_root->parent = copyByRecursion(other.fake_root->parent, other.fake_root);
		fake_root->parent->parent = fake_root;

		fake_root->left = iterator(fake_root->parent).GetMin().node();
		fake_root->right = iterator(fake_root->parent).GetMax().node();
	}

	const BinarySearchTree& operator=(const BinarySearchTree& other) {
		if (this == &other) return*this;
		BinarySearchTree temp(other);
		std::swap(temp.fake_root, this->fake_root);
		std::swap(temp._size, this->_size);

		return *this;

	}

	//parent - root; right - most right; left - most left
	void add(T value) {
		++_size;
		if (iterator(fake_root->parent).isFake()) {
			Node* addedNode = newNode(value, fake_root, fake_root, fake_root);
			fake_root->parent = addedNode;
			fake_root->left = addedNode;
			fake_root->right = addedNode;
		}
		else {
			iterator prev = iterator(fake_root->parent);
			iterator current = iterator(fake_root->parent);

			while (!current.isFake()) {
				prev = current;
				if (comparator(value, *current)) {
					current.step_left();
				}
				else if (comparator(*current, value)) {
					current.step_right();
				}
			}
			Node* addedNode = newNode(value, prev.node(), fake_root, fake_root);
			if (comparator(value, *prev)) {
				prev.node()->left = addedNode;
				if (fake_root->left == prev.node()) {
					fake_root->left = addedNode;
				}
			}
			else {
				prev.node()->right = addedNode;
				if (fake_root->right == prev.node()) {
					fake_root->right = addedNode;
				}
			}
		}
	}

	void erase_leaf(iterator leaf) {
		if (!leaf.parent().isFake()) {
			if (leaf.parent().right() == leaf) {
				leaf.parent().node()->right = fake_root;
			}
			else {
				leaf.parent().node()->left = fake_root;
			}
		}
		if (fake_root == leaf.node()) {
			fake_root->parent = fake_root;
			fake_root->left = fake_root;
			fake_root->right = fake_root;
		}
		else {
			if (fake_root->right == leaf.node()) {
				fake_root->right = leaf.parent().node();
			}
			else {
				if (fake_root->left == leaf.node()) {
					fake_root->left = leaf.parent().node();
				}
			}
		}

		clearNode(leaf.node());
	}

	

	void erase(iterator it) {
		if (it.isFake()) return;

		else if (it.right().isFake() && it.left().isFake()) {
			erase_leaf(it);
			return;
		}

		else if (it.right().isFake()) {
			if (it.parent().isFake()) {
				fake_root->parent = it.left().node();
				it.left().node()->parent = fake_root;
				fake_root->right = it.left().GetMax().node();
				clearNode(it.node());
				return;
			}
			else {
				it.left().setParent(it.parent());
				if (it.isRight()) {
					it.parent().setRight(it.left());
					if (it.node() == fake_root->right) {
						fake_root->right = it.left().GetMax().node();
					}
				}
				else {
					it.parent().setLeft(it.left());
				}
				--_size;
				clearNode(it.node());
				return;

			}

			if (it.left().isFake()) {
				if (it.parent().isFake()) {
					fake_root->parent = it.right().node();
					it.right().node()->parent = fake_root;
					fake_root->left = it.right().GetMin().node();
					clearNode(it.node());
					--_size;
					return;
				}
				else {
					it.right().setParent(it.parent());
					if (it.isRight()) {
						it.parent().setRight(it.right());
					}
					else {
						it.parent().setLeft(it.right());
						if (it.node() == fake_root->left) {
							fake_root->left = it.right().GetMin().node();
						}
					}
					--_size;
					clearNode(it.node());
					return;
				}
			}
		}

		swapWithMostLeft(it);
		erase(it);

	}

	iterator find(const T& value) const {
		iterator cur = iterator(fake_root->parent);

		while (!cur.isFake()) {
			if (comparator(value, *cur)) {
				cur = cur.left();
				continue;
			}
			if (comparator(*cur, value)) {
				cur = cur.right();
				continue;
			}
			break;
		}
		return cur;
	}

	T min() const {
		return fake_root->left->data;
	}

	T max() const {
		return fake_root->right->data;
	}

	T lowerBound(const T& value) const {
		iterator current(fake_root->parent);
		iterator result(fake_root->parent);;

		while (!current.isFake()) {
			if (!comparator(value, *current)) {
				result = current;
				current = current.right();
			}
			else
				current = current.left();
		}

		return *result;
	}

	T upper_bound(const T& value) const {

		iterator current(fake_root->parent);
		iterator result(fake_root->parent);;
		while (!current.isFake()) {

			if (comparator(value, *current)) {
				result = current;
				current = current.left();
			}
			else
				current = current.right();
		}

		return *result;
	}

	void erase(const T& value) {
		iterator iter = find(value);
		if (!iter.isFake()) erase(iter);
	}


	void PrintTree() const {
		printNode(fake_root->parent);
		std::cout << "********************************************************\n";
	}

	size_t size() const { return _size; }


};