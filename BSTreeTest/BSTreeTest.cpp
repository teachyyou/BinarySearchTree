#include "CppUnitTest.h"
#include "..\BinarySearchTree\BSTree.h"
#include <set>
#include <functional>
#include <memory_resource>
#include <iterator>
#include <vector>

//������������ ��������� <set>, ���������� �� ����� �The C++ Standard Template Library� P.J. Plauger, Alexander A. Stepanov,
//    Meng Lee, David R. Musser. ������� ��������������, � ������� �� ��������� �����. 
   // 
   // ��� ����� ����� ���� ������������ ��� ������������ ��������� ������ ������, ����������� �� �������� � ������������ ����������� STL,
   // ������ �� ����� ������������ ��� ��� ������������ ������� set �� ������ RBT, ��� � �� ������ AVL.
   // 
   // ������� ����� � ����, ������ ������ �������� ����� � �������� ���������� ���������� std::set, ��� ��� ����� ��� ����, ������ �� 
   // ��� ���������� ������ ���������, �� ���� �� ���.

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TreeTest
{
	TEST_CLASS(BaseTests)
	{
	public:

		template<typename T> using Myal = std::allocator<T>;
		template<typename T> using Mypred = std::less<T>;

		//  ��� ����, ����� ��������� ������������ ������ �� ��������� ����������� (std::set ��� Binary_Tree_Search)
		//    ������ ���� ����������������� ���� �� ��������� �����:
		//template<typename T> using ContainerTemplate = std::set<T, Mypred<T>, Myal<T>>;
		template<typename T> using ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;

		TEST_METHOD(TreeSizeTest)
		{
			//  ������������ ������� ������ ��� ������ ��������� �������������
			ContainerTemplate<int> T1;
			Assert::AreEqual(T1.size(), ContainerTemplate<int>::size_type(0), L"������ ������ ����� ��������� ������!");
			ContainerTemplate<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			Assert::AreEqual(Tree.size(), ContainerTemplate<int>::size_type(9), L"������� ����������� ������ ������!");
			ContainerTemplate<int> Tree2(Tree);
			Assert::AreEqual(Tree.size(), Tree2.size(), L"������� ����������� ������ ����� �����������!");
		}

		TEST_METHOD(TreesEqualityTest)
		{
			//  ������������ ��������� == ��� ������
			ContainerTemplate<int> T1, T2;
			Assert::IsTrue(T1 == T2, L"������� ����������� ��������� ������ ��������!");
			ContainerTemplate<int> Tree = { 40,50,30,35,10,75,23,87,68 };
			ContainerTemplate<int> Tree2(Tree);
			Assert::IsTrue(Tree == Tree2, L"������ ��������� �������� �� ��������� ����� �����������!");
		

		}
		TEST_METHOD(TreeAssignmentTests)
		{
			//  ������������ ��������� == ��� ������
			ContainerTemplate<int> T1 = { 40,50,30,35,10,75,23,87,68 };
			ContainerTemplate<int> T2;
			T2 = T1;
			Assert::IsTrue(T1 == T2, L"������ � ��������� ������������!");
			T2.clear();
			T1 = T2;
			Assert::IsTrue(T1 == T2, L"������ � ��������� ������������ ������ ��������!");
			ContainerTemplate<int> T3 = { 40,50,30,35,10,75,23,87,68 };
			T1 = T3;
			T2 = std::move(T3);
			Assert::IsTrue(T1 == T2, L"������ ��� ������������ ��������� ������������!");
		}
		
	};
	TEST_CLASS(SetTests)
	{
		//  ����� ������������ ���������� std::set, �� ����� "The C++ Standard template library" ��������, ��������� � ��.
	public:

		template<typename T> using Myal = std::allocator<T>;
		template<typename T> using Mypred = std::less<T>;

		//  ��� ����, ����� ��������� ������������ ������ �� ��������� ����������� (std::set ��� Binary_Tree_Search)
		//    ������ ���� ����������������� ���� �� ��������� �����:
		//template<typename T> using ContainerTemplate = std::set<T, Mypred<T>, Myal<T>>;
		template<typename T> using ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;

		using Mycont = ContainerTemplate<char>;

		TEST_METHOD(SetSize)
		{
			Mycont v0;
			Myal<char> al = v0.get_allocator();
			Mypred<char> pred;
			Mycont v0a(pred), v0b(pred, al);
			Assert::IsTrue(v0.empty() && v0.size() == 0, L"�������� ������ ������� set");
			Assert::IsTrue(v0a.size() == 0 && v0a.get_allocator() == al, L"�������� ������ ��� ���������");
			Assert::IsTrue(v0b.size() == 0 && v0b.get_allocator() == al, L"�������� ������ ��� ���������");
		}

		TEST_METHOD(SetCreation)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal<char> al = v0.get_allocator();
			Mypred<char> pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"������� �������� set ��������");

			Mycont v2(carr, carr + 3, pred);
			Assert::IsTrue(v2.size() == 3 && *v2.begin() == 'a', L"������� �������� set ��������");

			Mycont v3(carr, carr + 3, pred, al);
			Assert::IsTrue(v3.size() == 3 && *v3.begin() == 'a', L"������� �������� set ��������");

			const Mycont v4(carr, carr + 3);
			v0 = v4;
			Assert::IsTrue(v1.size() == 3 && *v1.begin() == 'a', L"������� �������� �������� ������������ ��� set");
		}

		TEST_METHOD(SetIterators)
		{
			char ch('a'), carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal<char> al = v0.get_allocator();
			Mypred<char> pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Mycont v2(carr, carr + 3, pred);
			Mycont v3(carr, carr + 3, pred, al);
			const Mycont v4(carr, carr + 3);
			v0 = v4;

			Mycont::iterator p_it(v1.begin());
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			/*Assert::IsTrue(*p_it == 'a' && *--(p_it = v1.end()) == 'c', L"��������� end() �� ���������?");
			Assert::IsTrue(*p_cit == 'a' && *--(p_cit = v4.end()) == 'c', L"��������� ��� const iterator �� end() �� ���������?");
			//Assert::IsTrue(*p_rit == 'c' && *--(p_rit = v1.rend()) == 'a', L"Reverse iterator �� ���������?");
			Assert::IsTrue(*p_crit == 'c' && *--(p_crit = v4.rend()) == 'a', L"Const reverse iterator �� ���������?");*/
		}

		TEST_METHOD(SetInsertEraseTests)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Myal<char> al = v0.get_allocator();
			Mypred<char> pred;
			Mycont v0a(pred);

			Mycont v1(carr, carr + 3);
			Mycont v2(carr, carr + 3, pred);
			Mycont v3(carr, carr + 3, pred, al);
			const Mycont v4(carr, carr + 3);
			v0 = v4;

			Mycont::iterator p_it(v1.begin());
			Mycont::const_iterator p_cit(v4.begin());
			Mycont::reverse_iterator p_rit(v1.rbegin());
			Mycont::const_reverse_iterator p_crit(v4.rbegin());

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && pib.second);
			Assert::IsTrue(*--v0.end() == 'd');
			pib = v0.insert('d');
			Assert::IsTrue(*pib.first == 'd' && !pib.second);
			Assert::IsTrue(*v0.insert(v0.begin(), 'e') == 'e');
			v0.insert(carr, carr + 3);
			Assert::IsTrue(v0.size() == 5 && *v0.begin() == 'a');
			v0.insert(carr2, carr2 + 3);
			Assert::IsTrue(v0.size() == 6 && *--v0.end() == 'f');
			Assert::IsTrue(*v0.erase(v0.begin()) == 'b' && v0.size() == 5);
			Assert::IsTrue(*v0.erase(v0.begin(), ++v0.begin()) == 'c' && v0.size() == 4);
			Assert::IsTrue(v0.erase('x') == 0 && v0.erase('e') == 1);
		}

		TEST_METHOD(SetSwapAndCompTests)
		{
			char carr[] = "abc", carr2[] = "def";
			Mycont v0;
			Mycont v1(carr, carr + 3);

			v0.clear();
			std::pair<Mycont::iterator, bool> pib = v0.insert('d');
			pib = v0.insert('d');
			v0.insert(v0.begin(), 'e');
			v0.insert(carr, carr + 3);
			v0.insert(carr2, carr2 + 3);
			v0.erase(v0.begin());
			v0.erase(v0.begin(), ++v0.begin());
			v0.erase('x');
			v0.erase('e');
			v0.clear();
			Assert::IsTrue(v0.empty());
			v0.swap(v1);
			Assert::IsTrue(!v0.empty() && v1.empty());
			std::swap(v0, v1);
			Assert::IsTrue(v0.empty() && !v1.empty());
			Assert::IsTrue(v1 == v1 && v0 < v1, L"��������� �������� �����������!");
			Assert::IsTrue(v0 != v1 && v1 > v0, L"��������� �������� �����������!");
			Assert::IsTrue(v0 <= v1 && v1 >= v0, L"��������� �������� �����������!");
		}

		TEST_METHOD(SetComparatorTests)
		{
			Mycont v0;
			Assert::IsTrue(v0.key_comp()('a', 'c') && !v0.key_comp()('a', 'a'), L"������������ ����������!");
			Assert::IsTrue(v0.value_comp()('a', 'c') && !v0.value_comp()('a', 'a'), L"������������ ����������!");
		}

		TEST_METHOD(SetAlgTests)
		{
			char carr[] = "abc";
			const Mycont v4(carr, carr + 3);

			Assert::IsTrue(*v4.find('b') == 'b');
			Assert::IsTrue(v4.count('x') == 0 && v4.count('b') == 1);
			Assert::IsTrue(*v4.lower_bound('a') == 'a', L"����� lower_bound");
			Assert::IsTrue(*v4.upper_bound('a') == 'b', L"����� upper_bound");
			std::pair<Mycont::const_iterator, Mycont::const_iterator> pcc = v4.equal_range('a');
			Assert::IsTrue(*pcc.first == 'a' && *pcc.second == 'b', L"������ ������ equal_range");
		}
	};

	class Elem {
		int data;
		uint32_t watch;
		static size_t elem_counter;
		void check() const { Assert::IsTrue(watch == 0xDEADBEEF, L"����������� ������!! (��������� � ��������������������� ���������� ������ Elem)"); }
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

	TEST_CLASS(ComplexTypeTests)
	{
		///  ������� ���������� - ����� �� ������� ���������� ������
	public:

		template<typename T> using Myal = std::allocator<T>;
		template<typename T> using Mypred = std::less<T>;

		//  ��� ����, ����� ��������� ������������ ������ �� ��������� ����������� (std::set ��� Binary_Tree_Search)
		//    ������ ���� ����������������� ���� �� ��������� �����:
		//template<typename T> using ContainerTemplate = std::set<T, Mypred<T>, Myal<T>>;
		template<typename T> using ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;


		TEST_METHOD(StringTests)
		{
			return;
			ContainerTemplate<std::string> T1{ "abc", "cde", "123", "AAAAAAAA" };
			std::vector<std::string> check1{ "123", "AAAAAAAA", "abc", "cde" };
			

			Assert::IsTrue(T1.size() == 4, L"����������� ��������� ���������� ��������� ���������");
			Assert::IsTrue(std::equal(T1.begin(), T1.end(), check1.begin(), check1.end()), L"������������ ������� ��������� ���������");
			Assert::IsTrue(std::equal(T1.rbegin(), T1.rend(), check1.rbegin(), check1.rend()), L"������������ ������� ��������� ���������");

			for (const auto& str : check1)
				T1.erase(str);

			Assert::IsTrue(T1.size() == 0, L"��� �� ���� ����� �������� ���� ���������");
			for (const auto& elem : T1)
				Assert::Fail(L"���-�� ����� � ���� ����� �������� ���� ���������");

			T1.insert("test-1");
			T1.insert("test-4");
			T1.insert("test-2");
			
			std::vector<std::string> check2{ "test-1", "test-2", "test-4" };
			Assert::IsTrue(std::equal(T1.begin(), T1.end(), check2.begin(), check2.end()), L"������������ ������� ��������� ���������");
			Assert::IsTrue(std::equal(T1.rbegin(), T1.rend(), check2.rbegin(), check2.rend()), L"������������ ������� ��������� ���������");
		}

		TEST_METHOD(ElemTests)
		{
			size_t init_count = Elem::count();
			{
				ContainerTemplate<Elem> T1;
				Assert::IsTrue(Elem::count() - init_count == 0, L"������ ������ ��������� ������ Elem");

				for (const auto& elem : T1)
					Assert::Fail(L"���-�� ����� � ������ ����");

				T1.insert(Elem(40));
				T1.insert(Elem(75));
				T1.insert(Elem(50));
				T1.insert(Elem(23));
				T1.insert(Elem(87));
				Assert::IsTrue(Elem::count() - init_count == 5, L"������ ������ ��������� ������ Elem");
				T1.erase(Elem(75));
				Assert::IsTrue(Elem::count() - init_count == 4, L"����������� �������� �������� ��������� ������");
				T1.erase(Elem(100));
				Assert::IsTrue(Elem::count() - init_count == 4, L"����������� �������� �������� �������������� ���������");
			}
			Assert::IsTrue(Elem::count() - init_count == 0, L"������ ������!!");
		}
	};

} 