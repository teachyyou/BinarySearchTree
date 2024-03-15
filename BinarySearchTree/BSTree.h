#pragma once
#include <iostream>
#include <iterator>
#include <memory>
#include <algorithm>

template<typename T, class Comparator = std::less<T>, class Allocator = std::allocator<T>>
class BinarySearchTree {

public:
	using key_type = T;
	using key_compare = Comparator;
	using value_type = T;
	using value_compare = Comparator;
	using allocator_type = Allocator;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;

private:

	class Node {
	public:
		Node* parent;
		Node* left;
		Node* right;
		value_type data;

		bool isFake = false;

		Node(const T& value, Node* p = nullptr, Node* l = nullptr, Node* r = nullptr) : parent(p), left(l), right(r), data(value) {}
		Node(T&& value, Node* p = nullptr, Node* l = nullptr, Node* r = nullptr) : parent(p), left(l), right(r), data(std::move(value)) {}

		bool operator==(const Node* other) const {
			return (data == other->data);
		}

	};
public:
	class iterator {
	public:
		friend class BinarySearchTree<value_type>;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = BinarySearchTree::value_type;
		using difference_type = BinarySearchTree::difference_type;
		using pointer = BinarySearchTree::const_pointer;
		using reference = BinarySearchTree::const_reference;
	protected:
		Node* _node;
		explicit iterator(Node* nd) : _node(nd) {};
		Node* &node() {
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
			if (!_node) {
				return true;
			}
			else if (_node->isFake) {
				return true;
			}
			return false;
		}
	public:
		const_reference operator*() {
			return _node->data;
		}
		pointer operator->() {
			return *_node;
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
			this->operator++();
			return iter;
		}
		iterator operator--(int) {
			iterator iter(*this);
			this->operator--();
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
		friend bool operator!=(const iterator& it1, const iterator& it2)
		{
			return it1._node != it2._node;
		}


		iterator GetMin() const {
			iterator temp = *this;
			while (!temp.left().isFake())
				temp = temp.left();
			return temp;
		}
		iterator GetMax() const {
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
private:
	Comparator comparator = Comparator();
	using Alloc = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
	Alloc alloc;


	//Node* root;
	Node* fake_root;
	size_type _size = 0;

	inline void fake_root_initialize() {
		fake_root = alloc.allocate(1);

		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->parent));
		fake_root->parent = fake_root;
		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->right));
		fake_root->right = fake_root;
		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->left));
		fake_root->left = fake_root;
		std::allocator_traits<Alloc>::construct(alloc, &(fake_root->isFake));
		fake_root->isFake = true;

	}

	inline void fake_root_delete() {
		std::allocator_traits<Alloc>::destroy(alloc, &(fake_root->parent));
		std::allocator_traits<Alloc>::destroy(alloc, &(fake_root->right));
		std::allocator_traits<Alloc>::destroy(alloc, &(fake_root->left));
		std::allocator_traits<Alloc>::destroy(alloc, &(fake_root->isFake));
		std::allocator_traits<Alloc>::deallocate(alloc, fake_root, 1);

	}
	inline Node* newNode(value_type&& value, Node* parent, Node* left, Node* right) {
		Node* newnode = alloc.allocate(1);

		std::allocator_traits<Alloc>::construct(alloc, &(newnode->parent));
		newnode->parent = parent;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->left));
		newnode->left = left;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->right));
		newnode->right = right;
		std::allocator_traits<Alloc>::construct(alloc, &(newnode->data), std::move(value));

		std::allocator_traits<Alloc>::construct(alloc, &(newnode->isFake));
		newnode->isFake = false;

		return newnode;
	}
	inline Node* newNode(const value_type& value, Node* parent, Node* left, Node* right) {
		return newNode(std::move(value_type(value)), parent, left, right);
	}
	void clearNode(Node* node) {

		std::allocator_traits<Alloc>::destroy(alloc, &(node->data));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->parent));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->right));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->left));
		std::allocator_traits<Alloc>::destroy(alloc, &(node->isFake));
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
	iterator swapWithMostLeft(iterator node) {
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
			return node;

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
		return node;
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
	using const_iterator = iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	friend bool operator==(const BinarySearchTree& first, const BinarySearchTree& second) {
		return (first._size == second._size) && checkByRecursion(first.fake_root->parent, second.fake_root->parent);
	}

	friend bool operator<(const BinarySearchTree& first, const BinarySearchTree& second) {
		iterator it1 = first.begin();
		iterator it2 = second.begin();
		while (it1 != first.end() && it2 != second.end() && *it1 == *it2) {
			++it1; ++it2;
		}

		if (it1 == first.end())
			return it2 != second.end();

		return it2 != second.end() && *it1 < *it2;
	}

	friend bool operator>(const BinarySearchTree& first, const BinarySearchTree& second) {
		return second < first;
	}

	friend bool operator>=(const BinarySearchTree& first, const BinarySearchTree& second) {
		return !(first < second);
	}

	friend bool operator<=(const BinarySearchTree& first, const BinarySearchTree& second) {
		return !(first > second);
	}

	friend bool operator!=(const BinarySearchTree& first, const BinarySearchTree& second) {
		return !(first == second);
	}

	iterator begin() const noexcept {
		return iterator(fake_root->left);
	}
	iterator end() const noexcept {
		return iterator(fake_root);
	}

	reverse_iterator rbegin() const	noexcept {
		return reverse_iterator(iterator(fake_root->right));
	}
	reverse_iterator rend() const noexcept {
		return reverse_iterator(iterator(fake_root));
	}

	allocator_type get_allocator() const noexcept {
		return alloc;
	}

	key_compare key_comp() const noexcept {
		return comparator;
	}
	value_compare value_comp() const noexcept {
		return comparator;
	}

	inline bool empty() const noexcept {
		return _size == 0;
	}


	template <class InputIterator>
	BinarySearchTree(InputIterator first, InputIterator last, Comparator comparator = Comparator(), allocator_type alloc = allocator_type()) : BinarySearchTree(comparator, alloc) {

		std::for_each(first, last, [this](T x) { insert(x); });
	}
	BinarySearchTree(Comparator _comparator = Comparator(), allocator_type _alloc = allocator_type()) : comparator(_comparator), alloc(_alloc), _size(0) {
		fake_root_initialize();

	}

	BinarySearchTree(std::initializer_list<value_type> list) : BinarySearchTree() {
		for (const T& x : list) insert(x);
	}

	BinarySearchTree(const BinarySearchTree& other) : BinarySearchTree() {
		_size = other._size;
		if (_size == 0) return;

		fake_root->parent = copyByRecursion(other.fake_root->parent, other.fake_root);
		fake_root->parent->parent = fake_root;

		fake_root->left = iterator(fake_root->parent).GetMin().node();
		fake_root->right = iterator(fake_root->parent).GetMax().node();
	}

	~BinarySearchTree() {
		clear();
		fake_root_delete();
	}

	const BinarySearchTree& operator=(const BinarySearchTree& other) {
		if (this == &other) return *this;
		BinarySearchTree<T> temp(other);
		std::swap(temp.fake_root, this->fake_root);
		std::swap(temp._size, this->_size);
		return *this;

	}

	const BinarySearchTree& operator=(BinarySearchTree&& other) {
		if (this == &other) return *this;
		std::swap(other.fake_root, this->fake_root);
		std::swap(other._size, this->_size);
		return *this;

	}

	std::pair<iterator, bool> insert(const_reference value) {
		return insert(std::move(T(value)));
	}

	//parent - root; right - most right; left - most left
	std::pair<iterator, bool> insert(value_type&& value) {
		if (iterator(fake_root->parent).isFake()) {
			Node* addedNode = newNode(std::move(value), fake_root, fake_root, fake_root);
			fake_root->parent = addedNode;
			fake_root->left = addedNode;
			fake_root->right = addedNode;
			++_size;
			return std::make_pair(iterator(addedNode), true);
		}
		else {
			iterator prev = iterator(fake_root->parent);
			iterator current = iterator(fake_root->parent);

			while (!current.isFake()) {
				prev = current;
				if (comparator(value, *current)) {
					current.step_left();
					continue;
				}
				if (comparator(*current, value)) {
					current.step_right();
					continue;
				}
				return std::make_pair(iterator(current), false);
			}
			Node* addedNode = newNode(std::move(value), prev.node(), fake_root, fake_root);
			++_size;
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
			return std::make_pair(iterator(addedNode), true);
		}
	}

	iterator insert(const_iterator pos, const_reference value) {
		iterator prev(pos);
		if (pos.isFake() || comparator(value, *pos)) {
			--prev;
			while (!prev.isFake() && comparator(value, *prev)) {
				pos = prev--;
			}
		}
		else {
			while (!pos.isFake() && !comparator(value, *pos)) {
				prev = pos++;
			}
		}

		if (pos == prev) {
			_size++;
			Node* addedNode = newNode(value, fake_root, fake_root, fake_root);
			fake_root->parent = fake_root->left = fake_root->right = addedNode;
			return iterator(addedNode);
		}
		if (!prev.isFake() && *prev == value) return prev;

		if (prev.isFake()) {
			Node* pnode = pos.node();
			pnode->left = newNode(value, pnode, fake_root, fake_root);
			++_size;
			fake_root->left = pnode;
			return iterator(fake_root->left);
		}

		if (prev.right().isFake()) {
			prev.node()->right = newNode(value, prev.node(), fake_root, fake_root);
			++_size;
			if (fake_root->right == prev.node()) {
				fake_root->right = prev.node()->right;
			}
			return iterator(prev.node()->right);
		}
		pos.node()->left = newNode(value, pos.node(), fake_root, fake_root);
		++_size;
		return iterator(pos.node()->left);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		while (first != last) insert(*first++);
	}

	void erase_leaf(iterator leaf) {
		if (leaf.isFake()) return;

		if (!leaf.parent().isFake()) {
			if (leaf.parent().right() == leaf) {
				leaf.parent().node()->right = fake_root;
			}
			else {
				leaf.parent().node()->left = fake_root;
			}
		}
		if (fake_root->parent == leaf.node()) {
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
		_size--;
		clearNode(leaf.node());
	}

	iterator erase(iterator it) {
		if (it.isFake()) return iterator(it);

		if (it.right().isFake() && it.left().isFake()) {
			iterator toReturn(it);
			++toReturn;
			erase_leaf(it);
			return toReturn;
		}

		if (it.right().isFake()) {
			if (it.parent().isFake()) {
				fake_root->parent = it.left().node();
				it.left().node()->parent = fake_root;
				fake_root->right = it.left().GetMax().node();
				_size--;
				clearNode(it.node());
				return iterator(fake_root->right);
			}
			else {
				iterator toReturn(it);
				++toReturn;
				it.left().node()->parent = it.parent().node();
				if (it.isRight()) {
					it.parent().node()->right = it.left().node();
					if (it.node() == fake_root->right) {
						fake_root->right = it.left().GetMax().node();
					}
				}
				else {
					it.parent().node()->left = it.left().node();
				}
				--_size;
				clearNode(it.node());
				return toReturn;

			}

		}
		if (it.left().isFake()) {
			if (it.parent().isFake()) {
				fake_root->parent = it.right().node();
				it.right().node()->parent = fake_root;
				fake_root->left = it.right().GetMin().node();
				--_size;
				clearNode(it.node());

				return iterator(fake_root->left);
			}
			else {
				iterator toReturn(it.right());
				it.right().node()->parent = it.parent().node();
				if (it.isRight()) {
					it.parent().node()->right = it.right().node();
				}
				else {
					it.parent().node()->left = it.right().node();
					if (it.node() == fake_root->left) {
						fake_root->left = it.right().GetMin().node();
					}
				}
				--_size;
				clearNode(it.node());
				return toReturn;
			}
		}

		swapWithMostLeft(it);
		return erase(it);

	}

	size_type erase(const_reference value) {
		iterator it = find(value);
		if (it.isFake()) return 0;
		erase(it);
		return 1;
	}

	iterator erase(const_iterator first, const_iterator last) {
		while (first != last)
			first = erase(first);
		return last;
	}

	iterator find(const_reference value) const {
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

	value_type min() const {
		return fake_root->left->data;
	}

	value_type max() const {
		return fake_root->right->data;
	}

	iterator lower_bound(const_reference value) const {
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

		return result;
	}

	iterator upper_bound(const_reference value) const {

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

		return result;
	}
	size_type count(const value_type& key) const {
		if (find(key) == end()) {
			return 0;
		}
		return 1;
	}

	std::pair<const_iterator, const_iterator> equal_range(const_reference value) const {
		const_iterator temp(fake_root->parent);
		const_iterator left(fake_root->parent);
		const_iterator right(fake_root->parent);
		while (!temp.isFake()) {
			if (!comparator(value, *temp)) {
				left = temp;
				temp.step_right();
			}
			else {
				right = temp;
				temp.step_left();
			}
		}
		return std::make_pair(left, right);

	}


	void PrintTree() const {
		//if (_size == 0) return;
		printNode(fake_root->parent);
		std::cout << "********************************************************\n";
	}

	size_type size() const { return _size; }

	void clear() {
		clearByRecur(fake_root->parent);
		_size = 0;
		fake_root->parent = fake_root;
		fake_root->left = fake_root;
		fake_root->right = fake_root;
	}

	void swap(BinarySearchTree& other) noexcept {
		std::swap(fake_root, other.fake_root);
		std::swap(_size, other._size);
	}

private:
	void clearByRecur(Node* node) {
		if (node != fake_root) {
			clearByRecur(node->left);
			clearByRecur(node->right);
			clearNode(node);
		}
	}
};