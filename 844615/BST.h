/**
 * Matricola : 844615 
 * Nome : Daniel
 * Cognome : Mateus Cardona
*/


#ifndef BST_H 
#define BST_H

#include<iostream>
#include "unique_contraint_violation.h"
#include "subtree_root_not_found.h"
#include "subtree_extraction_failed.h"
#include "sequence_to_bst_injection_error.h"

/**
 * @brief Binary Search Tree (BST) template class
 * 
 * This class implements a Binary Search Tree data structure.
 * The BST maintains an in-order arrangement of elements
 * 
 * @tparam T The type of values stored in the BST
 * @tparam C The comparison functor type for ordering elements
 * @tparam E The equality functor type for comparing elements
*/
template <typename T, typename C, typename E>
class BST {

public:
    typedef T value_type;       ///< Type of values stored in the BST
    typedef C compare_type;     ///< Type of the comparison functor
    typedef E equals_type;      ///< Type of the equality functor
    typedef unsigned int size_type; ///< Type used for size and indexing

private: 
    /**
     * @brief Node structure for the BST
     * 
     * This struct represents a single node in the Binary Search Tree.
    */
    struct node {
        value_type value;  ///< The value stored in the node
        node *parent;      ///< Pointer to the parent node
        node *left;        ///< Pointer to the left child node
        node *right;       ///< Pointer to the right child node

        /**
         * @brief Default constructor
         * 
         * Initializes a node with null pointers.
         */
        node() : parent(nullptr), left(nullptr), right(nullptr) {}

        /**
         * @brief Constructor with value
         * 
         * Initializes a node with a given value and null pointers.
         * 
         * @param v The value to be stored in the node
         */
        explicit node(const value_type &v) : value(v), parent(nullptr), left(nullptr), right(nullptr) {}
    };

    node *_root;                ///< Pointer to the root node of the BST
    size_type _size;            ///< Number of nodes currently in the BST
    compare_type _compare;      ///< Functor for comparing elements
    equals_type _equals;        ///< Functor for checking equality of elements

public: 
    /**
     * @brief Default constructor
     * 
     * Initializes an empty Binary Search Tree.
     * 
     * @post _root == nullptr, _size == 0
    */
    BST() : _root(nullptr), _size(0) {}


    /**
     * @brief Constructs a BST from a sequence of data
     * 
     * @tparam I The iterator type
     * @param b Iterator to the beginning of the sequence of data
     * @param e Iterator to the end of the sequence of data
     * 
     * 
     * @post If successful, a BST is constructed containing all elements from the range.
     *       If an exception is thrown during construction, the BST remains empty.
     * 
     * @throw sequence_to_bst_injection_error If an error occurs during element insertion
     * @throw May also throw exceptions from the add() method or element conversion
     * 
     * @note In case of any exception, the constructor ensures no memory is leaked
     *       by clearing any partially constructed tree before rethrowing.
     */
    template<typename I>
    BST(I b, I e) : _root(nullptr), _size(0) {
        try {
            while (b != e) {
                add(static_cast<value_type>(*b));
                ++b;
            }
        }
        catch (...) {
            clear(_root);
            _root = nullptr;
            _size = 0;
            throw sequence_to_bst_injection_error("Failed to construct BST from external data sequence: Error occurred while processing elements.");
        }
    }

    /**
     * @brief Copy constructor
     * @param other The BST to be copied
     * @throws May throw exceptions for bad-allocation
     */
    BST(const BST &other) : _root(nullptr), _size(0) {
        BST<value_type, compare_type, equals_type> tmp; 
        
        try {
            tmp.copySubTree(other._root);
        } catch(...) {
            tmp.clear(_root);
            tmp._root = nullptr;
            tmp._size = 0;
            throw;
        }

        swap(tmp);
    }

    /**
     * @brief Equal Operator
     * @param other The BST to be copied
     * @return A reference to *this
     * @throws May throw exceptions for bad-allocation
     */
    BST &operator=(const BST &other) {
        if(this != &other) {
            BST tmp(other);
            this->swap(tmp);
        }

        return *this;
    }

    /**
     * @brief Destructor
     * Frees all memory associated with the BST and sets its size to 0.
     */
    ~BST() {
        clear(_root);
        _root = nullptr;
        _size = 0;
    }

    /**
     * @brief Adds a new value to the Binary Search Tree
     * 
     * This method inserts a new value into the BST.
     * If the value already exists in the tree, a unique_constraint_violation exception is thrown.
     * 
     * @param value The value to be inserted into the BST
     * 
     * @pre The value to be added must not already exist in the BST.
     *  
     * @post If the value was not present in the BST:
     *       - A new node containing the value is added to the BST in-order.
     *       - The size of the BST is increased by 1.
     *       - The BST property is maintained (all left descendants <= node < all right descendants).
     * 
     * @throw unique_constraint_violation If the value already exists in the BST
     * @throw std::bad_alloc If memory allocation for the new node fails
     * 
     * @note This method may throw other exceptions if the comparison functor throws
     */
    void add(const value_type &value) {

		bool valueAlreadyExists = exists(value);
		if( valueAlreadyExists ){
			throw unique_contraint_violation("Insertion failed: The value is already present in the tree. BST does not allow duplicate values."); 
		}
		
		if(_root == nullptr) { 
			_root = new node(value);
			_size++;

			return;
		}

        try{
            placeNodeInDepth(_root, value),
            _size++;
        }catch(...){
            throw;
            // don´t increase _size when fail
        }
	}

    /**
     * @brief Creates a new BST representing a subtree of the current BST
     *
     * @param value The value of the root node of the subtree to be extracted
     * @return A new BST representing the extracted subtree
     *
     * @throws subtree_root_not_found If the specified value is not found in the BST
     * @throws subtree_extraction_failed If the subtree extraction process fails
     *
     * @note The returned BST is a deep copy of the subtree, not a reference to the original nodes
     * @note The new BST uses the same comparison and equality functors as the original BST
     */
    BST subtree(const value_type &value) const {
        const node* newRoot = search(_root, value);
        if(newRoot == nullptr) {
            throw subtree_root_not_found("Subtree root not found: The specified value does not exist in the BST.");
        }
        
        BST<value_type, compare_type, equals_type> subtree; 

        try {
            subtree.copySubTree(newRoot);
        } catch(...) {
            subtree.clear(subtree._root);
            subtree._root = nullptr;
            subtree._size = 0;
            throw subtree_extraction_failed("Unable to extract subtree: The operation to create a subtree from the given root has failed."); 
        }

        return subtree;
    }

    /**
     * @brief Returns the number of elements in the BST
     * @return The number of elements in the BST
    */
    unsigned int getSize() const { return _size; }

    /**
     * @brief Checks if a given value exists in the BST
     *
     * @param value The value to search for in the BST
     * @return true if the value is found in the BST, false otherwise
     */
    bool exists(const value_type& value) const { 
        return search(_root, value) != nullptr; 
    }

private: 
    /**
     * @brief Places a new node in the Binary Search Tree
     * 
     * This private helper method finds the correct position for a new value in the tree
     * and inserts it as a new leaf, maintaining the BST property.
     * 
     * @param current Pointer to the current node in the recursion process
     * @param value The value to be inserted into the tree
     * 
     * @pre The value must not already exist in the BST (this should be checked by the calling function).
     * 
     * @post If the insertion is successful:
     *       - A new node containing the value is added to the BST in-order.
     *       - The BST property is maintained (all left descendants <= node < all right descendants).
     *       - The parent pointers are correctly set.
     * 
     * @note Memory allocation:
     * - Memory is dynamically allocated only at the end of the recursion,
     *   when the new node is actually created.
     * - If the function fails before reaching a leaf (e.g., due to an
     *   exception thrown by _compare), no memory is allocated on the heap.
     * 
     * @note Exception handling:
     * - This function does not directly handle exceptions.
     * - Any thrown exceptions (e.g., from _compare or memory allocation)
     *   are propagated to the caller.
     * 
     * @throw May throw std::bad_alloc if memory allocation for the new node fails.
     *        May throw exceptions from the comparison functor (_compare).
     * 
     */
    void placeNodeInDepth(node* current, const value_type& value) {
        bool followsLeftBranch = _compare(value, current->value);
        
        if (followsLeftBranch) {
            if (current->left == nullptr) {
                current->left = new node(value);
                current->left->parent = current;
                return;
            }
            placeNodeInDepth(current->left, value);
        } else {
            if (current->right == nullptr) {
                current->right = new node(value);
                current->right->parent = current;
                return;
            }
            placeNodeInDepth(current->right, value);
        }
    }

    /**
     * @brief Clears a subtree 
     *
     * @note This method uses post-order traversal to ensure child nodes
     *       are deleted before their parent nodes.
     */
    void clear(node* current) {
        if (current == nullptr) return;
        
        clear(current->left);
        clear(current->right);
        
        delete current;
        current = nullptr;
    }

   /**
    * @brief Private utility method to copy a subtree into the current BST
    *
    * This method manages the process of copying a subtree, replacing the current
    * state of the BST with a copy of the provided subtree. It uses a temporary BST
    * for the copy process to ensure strong exception safety.
    *
    * @param subtree_root Const pointer to the root of the subtree to be copied
    *
    * @pre The caller is responsible for ensuring that 'subtree_root' is a valid
    *      pointer to a node in a BST or nullptr. No preliminary checks are performed.
    *
    * @note This is a private utility method. Logical correctness and appropriateness
    *       of the call are the responsibility of the calling method.
    * 
    * @throw Propagates any exceptions thrown during the copy process (e.g., std::bad_alloc)
    *
    */
    void copySubTree(const node *n){
        BST tmp; 
        
        try {
            copySubTreeHelper(n, tmp);
            swap(tmp); 
        } catch (...) {
            tmp.clear(tmp._root);
            tmp._root = nullptr;
            tmp._size = 0;
            throw;
        }
    }

    /**
    * @brief Private helper method to recursively copy nodes from one BST to another
    *
    * This method performs the actual copying of nodes from the source subtree
    * to the destination BST. It uses a pre-order traversal to maintain the
    * structure of the original subtree.
    *
    * @param current Const pointer to the current node in the source subtree
    * @param tmp Reference to the destination BST
    *
    * @note This is a private helper method called by copySubTree. No additional
    *       checks on the validity of inputs are performed.
    *
    * @throw May throw exceptions from the BST's add method (e.g., std::bad_alloc)
    */
    void copySubTreeHelper(const node *n, BST &tmp) {
        if (n == nullptr) return;

        tmp.add(n->value);
        copySubTreeHelper(n->left, tmp);
        copySubTreeHelper(n->right,tmp);
    }

    /**
     * @brief Searches for a node with a specific value in the BST
     *
     * @param current Pointer to the current node in the search process
     * @param value The value to search for
     * @return Pointer to the node containing the value if found, nullptr otherwise
     *
     */
    const node* search(const node* current, const value_type &value) const {
        if (current == nullptr) return nullptr;
        if (_equals(current->value, value)) return current;
        if (_compare(value, current->value)) return search(current->left, value);
        return search(current->right, value);
    }

    /**
     * @brief Finds the leftmost (smallest) node in a subtree
     *
     * @param n Pointer to the root of the subtree to search
     * @param first Pointer to the current candidate for the smallest node
     * @return Pointer to the leftmost (smallest) node in the subtree
     *
     * @note This method assumes that the BST property is maintained
     * @note If the subtree is empty (n is nullptr), it returns the last known smallest node
     */
    const node* getFirstNode(const node* n, const node *first) const {
        if (n == nullptr) { 
            return first; 
        } else {
            return getFirstNode(n->left, n);
        }
    }

    /**
     * @brief Swaps the contents of this BST with another BST
     *
     * @param other The BST to swap contents with
     */
    void swap(BST& other) {
        std::swap(_root, other._root);
        std::swap(_size, other._size);
    }


public:
    class const_iterator {

		const node *_node;

        /**
         * @brief Advances the iterator to the next node in the in-order traversal of the BST
         * 
         * This private method moves the iterator to the next node in the in-order traversal
         * sequence of the Binary Search Tree. It implements the logic for finding the
         * in-order successor of the current node.
         * 
         * @pre The iterator is pointing to a valid node in the BST or to the end (nullptr)
         * 
         * @post The iterator points to the next node in the in-order traversal, or to
         *       nullptr if the end of the traversal has been reached
         * 
         * @note This method handles all cases of BST traversal, including:
         *       - Moving to the right child and then all the way left
         *       - Moving up the tree when there's no right child
         *       - Handling the case when we've reached the end of the traversal
        */
        void goNext() {
            if (_node == nullptr) {

                return; 
            }
            
            if (_node->right != nullptr) {

                _node = _node->right;

                while (_node->left != nullptr) {
                    _node = _node->left;
                }

            } else {
                const node* parent = _node->parent;

                while (parent != nullptr && _node == parent->right) {
                    _node = parent;
                    parent = parent->parent;
                }

                _node = parent;
            }
        }

	public:
        typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;


		const_iterator() : _node(nullptr) {}
		const_iterator(const const_iterator &other) : _node(other._node) {}

		const_iterator &operator=(const const_iterator &other) {
			_node = other._node;
			return *this;
		}

		~const_iterator() {}

		reference operator*() const { return _node->value; }
		pointer operator->() const { return &(_node->value);}

		const_iterator operator++(int) {
			const_iterator tmp(*this);
			goNext();

			return tmp;
		}

		const_iterator &operator++() {
			goNext();

			return *this;
		}

		bool operator==(const const_iterator &other) const {
			return (_node == other._node);
		}

		bool operator!=(const const_iterator &other) const {
			return (_node != other._node);
		}

	private:

		friend class BST;
		const_iterator(const node *node) : _node(node) {}
	};

	const_iterator begin() const { 
		return const_iterator(getFirstNode(_root, nullptr)); 
	}

	const_iterator end() const { 
		return const_iterator(nullptr);
	}
};


/**
 * @brief Overloaded stream insertion operator for BST
 *
 * This global function allows a BST to be printed to an output stream.
 * It prints all elements of the BST in order, separated by spaces.
 *
 * @tparam T The type of elements stored in the BST
 * @tparam C The comparison functor type used by the BST
 * @tparam E The equality functor type used by the BST
 * @param os The output stream to write to
 * @param bst The BST to be printed
 * @return Reference to the output stream after writing
 *
 * @note The elements are printed using their own operator<< implementation
 * @note This function assumes that the BST's iterator traverses the tree in-order
 */
template <typename T, typename C, typename E>
std::ostream &operator<<(std::ostream &os, const BST<T,C,E> &bst) {
    if (bst.getSize() == 0) return os << "Empty BST";
    
    typename BST<T,C,E>::const_iterator b = bst.begin();
    typename BST<T,C,E>::const_iterator e = bst.end();
    
    while(b != e) {
        os << *b << " ";
        ++b;
    }

    return os;
}

/**
 * @brief Prints selected elements of a BST based on a given policy
 *
 * This function traverses the BST and prints only those elements that satisfy
 * the condition specified by the policy functor.
 *
 * @tparam T The type of elements stored in the BST
 * @tparam C The comparison functor type used by the BST
 * @tparam E The equality functor type used by the BST
 * @tparam P The type of the policy functor
 * @param bst The BST whose elements are to be conditionally printed
 * @param policy A functor that decides whether an element should be printed
 */

template<typename T, typename C, typename E, typename P>
void printIF(const BST<T, C, E> &bst, const P& policy) {
    typename BST<T,C,E>::const_iterator b = bst.begin();
    typename BST<T,C,E>::const_iterator e = bst.end();

    while (b != e) {
        if (policy(*b)) std::cout << *b << " ";
        ++b;
    }
}

#endif
