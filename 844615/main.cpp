#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "BST.h"

/**
 * @brief Struttura che rappresenta una persona con nome, età e altezza.
 */
struct Person {
    std::string name; ///< Nome della persona
    int age; ///< Età della persona
    double height; ///< Altezza della persona

    Person(std::string n, int a, double h) : name(n), age(a), height(h) {}

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age && height == other.height;
    }
};

/**
 * @brief Funtore per confrontare due persone.
 */
struct PersonCompare {
    bool operator()(const Person& a, const Person& b) const {
        if (a.age != b.age) return a.age < b.age;
        if (a.height != b.height) return a.height < b.height;
        return a.name < b.name;
    }
};

/**
 * @brief Funtore per verificare l'uguaglianza tra due persone.
 */
struct PersonEqual {
    bool operator()(const Person& a, const Person& b) const {
        return a.name == b.name && a.age == b.age && a.height == b.height;
    }
};

/**
 * @brief Struttura che rappresenta un punto nello spazio 2D.
 */
struct Point {
    int x, y; ///< Coordinate del punto
    Point(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << "," << p.y << ")";
}

/**
 * @brief Funtore per confrontare due interi.
 */
struct IntCompare {
    bool operator()(const int& a, const int& b) const { return a < b; }
};

/**
 * @brief Funtore per verificare l'uguaglianza tra due interi.
 */
struct IntEqual {
    bool operator()(const int& a, const int& b) const { return a == b; }
};

/**
 * @brief Funtore per confrontare due stringhe in base alla loro lunghezza.
 */
struct StringCompare {
    bool operator()(const std::string& a, const std::string& b) const { return a.length() < b.length(); }
};

/**
 * @brief Funtore per verificare l'uguaglianza tra due stringhe.
 */
struct StringEqual {
    bool operator()(const std::string& a, const std::string& b) const { return a == b; }
};

/**
 * @brief Funtore per confrontare due punti in base alla coordinata x.
 */
struct PointCompare {
    bool operator()(const Point& a, const Point& b) const { return a.x < b.x; }
};

/**
 * @brief Funtore per verificare l'uguaglianza tra due punti.
 */
struct PointEqual {
    bool operator()(const Point& a, const Point& b) const { return a == b; }
};

/**
 * @brief Funtore per verificare se un numero è pari.
 */
struct IsEven {
    bool operator()(const int& x) const {
        return x % 2 == 0;
    }
};

/**
 * @brief Funtore per verificare se un numero è maggiore di cinque.
 */
struct GreaterThanFive {
    bool operator()(const int& x) const {
        return x > 5;
    }
};

// Funzioni di test
void test_constructors() {
    std::cout << "Testing constructors..." << std::endl;

    BST<int, IntCompare, IntEqual> bst1;
    assert(bst1.getSize() == 0);

    std::vector<int> vec = {5, 3, 7, 2, 4, 6, 8};
    BST<int, IntCompare, IntEqual> bst2(vec.begin(), vec.end());
    assert(bst2.getSize() == 7);

    BST<int, IntCompare, IntEqual> bst3(bst2);
    assert(bst3.getSize() == 7);

    std::vector<std::string> vec_str = {"apple", "banana", "cherry"};
    BST<std::string, StringCompare, StringEqual> bst4(vec_str.begin(), vec_str.end());
    assert(bst4.getSize() == 3);

    std::cout << "Constructor tests passed!" << std::endl;
}

void test_assignment_operator() {
    std::cout << "Testing assignment operator..." << std::endl;

    BST<int, IntCompare, IntEqual> bst1;
    bst1.add(5);
    bst1.add(3);
    bst1.add(7);

    BST<int, IntCompare, IntEqual> bst2;
    bst2 = bst1;

    assert(bst2.getSize() == 3);
    assert(bst2.exists(5));
    assert(bst2.exists(3));
    assert(bst2.exists(7));

    BST<Point, PointCompare, PointEqual> bst3;
    bst3.add(Point(1, 1));
    bst3.add(Point(2, 2));

    BST<Point, PointCompare, PointEqual> bst4;
    bst4 = bst3;

    assert(bst4.getSize() == 2);
    assert(bst4.exists(Point(1, 1)));
    assert(bst4.exists(Point(2, 2)));

    std::cout << "Assignment operator test passed!" << std::endl;
}

void test_add() {
    std::cout << "Testing add method..." << std::endl;

    BST<int, IntCompare, IntEqual> bst;
    bst.add(5);
    bst.add(3);
    bst.add(7);
    bst.add(8);
    bst.add(9);
    bst.add(4);
    bst.add(6);

    assert(bst.getSize() == 7);
    assert(bst.exists(5));
    assert(bst.exists(3));
    assert(bst.exists(7));
    assert(bst.exists(8));
    assert(bst.exists(9));
    assert(bst.exists(4));
    assert(bst.exists(6));

    // Test aggiunta di un elemento già presente
    try {
        bst.add(5);
        assert(false); // Dovrebbe lanciare un'eccezione
    } catch (const unique_contraint_violation& e) {
        // Eccezione attesa
    }

    BST<std::string, StringCompare, StringEqual> bst_str;
    bst_str.add("hello");
    bst_str.add("world");
    bst_str.add("openai");

    assert(bst_str.getSize() == 3);
    assert(bst_str.exists("hello"));
    assert(bst_str.exists("world"));
    assert(bst_str.exists("openai"));

    std::cout << "Add method test passed!" << std::endl;
}

void test_subtree() {
    std::cout << "Testing subtree method..." << std::endl;

    BST<int, IntCompare, IntEqual> bst;
    bst.add(5);
    bst.add(3);
    bst.add(7);
    bst.add(2);
    bst.add(4);

    BST<int, IntCompare, IntEqual> subtree = bst.subtree(3);

    assert(subtree.getSize() == 3);
    assert(subtree.exists(3));
    assert(subtree.exists(2));
    assert(subtree.exists(4));

    // Test subtree con valore non presente
    try {
        BST<int, IntCompare, IntEqual> invalid_subtree = bst.subtree(6);
        assert(false); // Dovrebbe lanciare un'eccezione
    } catch (const subtree_root_not_found& e) {
        // Eccezione attesa
    }

    BST<Point, PointCompare, PointEqual> bst_point;
    bst_point.add(Point(3, 3));
    bst_point.add(Point(1, 1));
    bst_point.add(Point(5, 5));
    bst_point.add(Point(2, 2));

    BST<Point, PointCompare, PointEqual> subtree_point = bst_point.subtree(Point(3, 3));
    assert(subtree_point.getSize() == 4);
    assert(subtree_point.exists(Point(3, 3)));
    assert(subtree_point.exists(Point(5, 5)));

    std::cout << "Subtree method test passed!" << std::endl;
}

void test_iterators() {
    std::cout << "Testing iterators..." << std::endl;

    BST<int, IntCompare, IntEqual> bst;
    bst.add(5);
    bst.add(3);
    bst.add(7);
    bst.add(2);
    bst.add(4);
    bst.add(6);
    bst.add(8);

    std::vector<int> expected = {2, 3, 4, 5, 6, 7, 8};
    std::vector<int> result;

    for (BST<int, IntCompare, IntEqual>::const_iterator it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    assert(result == expected);

    BST<std::string, StringCompare, StringEqual> bst_str;
    bst_str.add("cat");
    bst_str.add("elephant");
    bst_str.add("bear");
    bst_str.add("zebra");
    bst_str.add("giraffe");

    std::vector<std::string> expected_order = {"cat", "bear", "zebra", "giraffe", "elephant"};
    std::vector<std::string> actual_order;

    for (BST<std::string, StringCompare, StringEqual>::const_iterator it = bst_str.begin(); it != bst_str.end(); ++it) {
        actual_order.push_back(*it);
    }

    assert(actual_order == expected_order);

    std::cout << "Iterator test passed!" << std::endl;
}

template<typename T, typename C, typename E>
struct BSTCompare {
    bool operator()(const BST<T, C, E>& a, const BST<T, C, E>& b) const {
        if (a.getSize() != b.getSize()) {
            return a.getSize() < b.getSize();
        }
        typename BST<T, C, E>::const_iterator it_a = a.begin();
        typename BST<T, C, E>::const_iterator it_b = b.begin();
        while (it_a != a.end() && it_b != b.end()) {
            if (*it_a != *it_b) {
                return *it_a < *it_b;
            }
            ++it_a;
            ++it_b;
        }
        return false;
    }
};

template<typename T, typename C, typename E>
struct BSTEqual {
    bool operator()(const BST<T, C, E>& a, const BST<T, C, E>& b) const {
        if (a.getSize() != b.getSize()) {
            return false;
        }
        typename BST<T, C, E>::const_iterator it_a = a.begin();
        typename BST<T, C, E>::const_iterator it_b = b.begin();
        while (it_a != a.end() && it_b != b.end()) {
            if (*it_a != *it_b) {
                return false;
            }
            ++it_a;
            ++it_b;
        }
        return true;
    }
};

void test_bst_of_bst() {
    std::cout << "Testing BST of BST..." << std::endl;

    typedef BST<int, IntCompare, IntEqual> InnerBST;
    typedef BST<InnerBST, BSTCompare<int, IntCompare, IntEqual>, BSTEqual<int, IntCompare, IntEqual>> OuterBST;

    OuterBST bst_of_bst;

    InnerBST bst1;
    bst1.add(1);
    bst1.add(2);
    bst1.add(3);

    InnerBST bst2;
    bst2.add(4);
    bst2.add(5);

    InnerBST bst3;
    bst3.add(6);
    bst3.add(7);
    bst3.add(8);

    InnerBST bst4;
    bst4.add(1);
    bst4.add(2);
    bst4.add(3);

    bst_of_bst.add(bst1);
    bst_of_bst.add(bst2);
    bst_of_bst.add(bst3);

    assert(bst_of_bst.getSize() == 3);
    assert(bst_of_bst.exists(bst1));
    assert(bst_of_bst.exists(bst2));
    assert(bst_of_bst.exists(bst3));
    assert(bst_of_bst.exists(bst4));

    OuterBST subtree = bst_of_bst.subtree(bst2);
    assert(subtree.getSize() == 1);
    assert(subtree.exists(bst2));

    std::cout << "BST of BST test passed!" << std::endl;
}

void test_printIF() {
    std::cout << "Testing printIF function..." << std::endl;

    BST<int, IntCompare, IntEqual> bst;
    for (int i = 1; i <= 10; ++i) {
        bst.add(i);
    }

    std::cout << "Original BST: ";
    std::cout << bst << std::endl;

    std::cout << "Even numbers: ";
    printIF(bst, IsEven());
    std::cout << std::endl;

    std::cout << "Numbers greater than 5: ";
    printIF(bst, GreaterThanFive());
    std::cout << std::endl;

    std::cout << "printIF test passed!" << std::endl;
}

void test_complex_person() {
    std::cout << "Testing complex person comparison..." << std::endl;

    BST<Person, PersonCompare, PersonEqual> bst_person;
    bst_person.add(Person("Alice", 30, 165.5));
    bst_person.add(Person("Bob", 25, 180.0));
    bst_person.add(Person("Charlie", 30, 170.0));
    bst_person.add(Person("David", 25, 175.0));

    std::vector<std::string> expected_names = {"David", "Bob", "Alice", "Charlie"};
    std::vector<std::string> actual_names;

    for (BST<Person, PersonCompare, PersonEqual>::const_iterator it = bst_person.begin(); it != bst_person.end(); ++it) {
        actual_names.push_back(it->name);
    }

    assert(actual_names == expected_names);
    std::cout << "Complex person comparison test passed!" << std::endl;
}

struct PointDistanceCompare {
    bool operator()(const Point& a, const Point& b) const {
        return (a.x * a.x + a.y * a.y) < (b.x * b.x + b.y * b.y);
    }
};

struct PointComplexEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

void test_complex_point() {
    std::cout << "Testing complex point comparison..." << std::endl;

    BST<Point, PointDistanceCompare, PointComplexEqual> bst_point;
    bst_point.add(Point(3, 4));
    bst_point.add(Point(1, 1));
    bst_point.add(Point(0, 5));
    bst_point.add(Point(2, 2));

    std::vector<Point> expected_order;
    expected_order.push_back(Point(1, 1));
    expected_order.push_back(Point(2, 2));
    expected_order.push_back(Point(3, 4));
    expected_order.push_back(Point(0, 5));

    std::vector<Point> actual_order;

    for (BST<Point, PointDistanceCompare, PointComplexEqual>::const_iterator it = bst_point.begin(); it != bst_point.end(); ++it) {
        actual_order.push_back(*it);
    }

    assert(actual_order == expected_order);
    std::cout << "Complex point comparison test passed!" << std::endl;
}

int main() {
    test_constructors();
    test_assignment_operator();
    test_add();
    test_subtree();
    test_iterators();
    test_bst_of_bst();
    test_printIF();
    test_complex_person();
    test_complex_point();

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
