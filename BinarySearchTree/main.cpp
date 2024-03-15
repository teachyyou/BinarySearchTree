#include "..\BinarySearchTree\BSTree.h"
#include <set>
#include <functional>
#include <memory_resource>
#include <iterator>
#include <vector>

class Elem {
	int data;
	uint32_t watch;
	static size_t elem_counter;
	void check() const { if (watch != 0xDEADBEEF) throw new std::exception("Повреждение памяти!! (Обращение к неинициализированному экземпляру класса Elem)"); }
public:
	Elem(const Elem& el) = delete;
	Elem& operator=(const Elem& el) = delete;
	explicit Elem(int value) : data(value), watch(0xDEADBEEF) { ++elem_counter; }
	Elem(Elem&& e) noexcept { e.check(); data = e.data; watch = e.watch; ++elem_counter; }
	Elem& operator=(Elem&& e) noexcept { check(); e.check(); data = e.data; watch = e.watch; }
	bool operator<(const Elem& e) const { check(); e.check(); return data < e.data; }
	~Elem() { check(); watch = 0; --elem_counter; }
	static size_t count() { return elem_counter; }
};
size_t Elem::elem_counter = 0;

int main() {

	BinarySearchTree<std::string> T1{ "abc", "cde", "123", "AAAAAAAA" };
	std::vector<std::string> check1{ "123", "AAAAAAAA", "abc", "cde" };
	for (std::string s : T1) {
		std::cout << s << std::endl;
	}
	T1.PrintTree();

	//BinarySearchTree<Elem> tree;
	
	return 1;
}