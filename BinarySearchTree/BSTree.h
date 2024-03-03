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
		iterator parent() {
			return iterator(_node->parent);
		}
		iterator left() {
			return iterator(_node->left);
		}
		iterator right() {
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

		iterator & operator++() {
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
			while (!isFake() && right() == prev) {
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
			while (!isFake() && left() == prev) {
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
				temp = temp.Left();
			return temp;
		}
		iterator GetMax() {
			iterator temp = *this;
			while (!temp.right().isFake())
				temp = temp.right();
			return temp;
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
public:

	//Итераторы
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
				if (fake_root->left == prev.node()) fake_root->left = addedNode;
			}
			else {
				prev.node()->right = addedNode;
				if (fake_root->right == prev.node()) fake_root->right = addedNode;
			}
		}
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

	void PrintTree() const {
		printNode(fake_root->parent);
		std::cout << "********************************************************\n";
	}

	size_t size() const { return _size; }

	
};