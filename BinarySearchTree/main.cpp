#include "..\BinarySearchTree\BSTree.h"
#include <set>
#include <functional>
#include <memory_resource>
#include <iterator>
#include <vector>
#include <chrono>

class Elem {
	long data;
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
    setlocale(LC_ALL, "RUS");
    {
        std::multiset<Elem> multiSet;
        auto start = std::chrono::steady_clock::now();
        const long n = 10000000; // Количество элементов
        for (long i = 0; i < n; ++i) {
            multiSet.insert(Elem(i));
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Вставка " << n << " элементов в std::multiset: " << elapsed_seconds.count() << "s\n";
    }
    
    {
        BinarySearchTree<Elem> bst;
        auto start = std::chrono::steady_clock::now();
        const long n = 10000000; // Количество элементов
        for (long i = 0; i < n; ++i) {
            bst.insert(Elem(i));
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Вставка " << n << " элементов в ikdr::BinarySearchTree: " << elapsed_seconds.count() << "s\n";

    }


    return 0;
}


void testWithPrint() {
    
}