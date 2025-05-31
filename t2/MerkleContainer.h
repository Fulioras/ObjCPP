// Author: Augustinas Bickaitis, 2025

// An append-only MerkleContainer, with a functionality to 
//				create static trees by returning a root MerkleNode
	

// NEED TO HAVE OPEN SSL LIBRARY INSTALLED!!


/*
//~~~~~~~~~~~~~BASIC USE~~~~~~~~~~~

using namespace MyMerkle;

MerkleContainer* tree = new MerkleContainer(); // Creates an empty append-only Merkle container

tree->add("string1");  // Adds hash value of "string1" to the tree
tree->add("string2");  // Adds hash value of "string2"

MerkleTree static1 = tree->get_tree(); 
// .get_tree() returns a MerkleTree representing a view-only snapshot of the current state

tree->add("string3");  // Append more data

MerkleTree static2 = tree->get_tree(); 
// static1 and static2 have different root hashes due to the new data

//~~~~~~~~~~~~~~CHECKING~~~~~~~~~~

tree->empty();       // Returns true if the container has no elements
tree->get_size();    // Returns number of added elements (leaf count)

static1.get_height(); // Returns height of the tree
static1.get_root_hash(); // Returns root hash of the tree

//~~~~~~~~~~~TRAVERSING STATIC TREE~~~~~~~

auto root = static1.get_root(); // Returns a shared_ptr<MerkleNode> to the root

if (root->get_height() > 0) {
    auto left = root->get_left();  // Returns shared_ptr<MerkleNode> to left child
    auto right = root->get_right(); // Returns shared_ptr<MerkleNode> to right child
    // Throws MerkleExcept if called on a leaf node (height == 0)
}

delete tree; // Don't forget to free memory.
*/
#include <memory> //shared and unique ptr
#include <iomanip> // hex function for line formating
#include <cmath>  // for pow()
#include <stack>
#include <string>
#ifndef OPEN_SSL
#define OPEN_SSL
#include "openssl/sha.h" // hash function
#endif //OPEN_SSL

#ifndef MERKLE_TREE
#define MERKLE_TREE

namespace MyMerkle{

	std::string sha256(const std::string& );// Hashing function from OpenSSl

//~~~~~~~~~~EXCEPTION~~~~~~
	class MerkleExcept : public std::runtime_error {
    public:
        explicit MerkleExcept(const std::string& msg);
    };
//~~~~~~~~~~~~~NODE~~~~~~~~~~
class MerkleNode {
	public:
		MerkleNode(const std::string& data); // creates a 0 height node from an input
		MerkleNode(std::shared_ptr<MerkleNode> l, std::shared_ptr<MerkleNode> r); //creates new node based on its left and right nodes
		
		MerkleNode(const MerkleNode& other);
		MerkleNode(const std::shared_ptr<MerkleNode>&); //Deep-copy

		~MerkleNode();
		
	    MerkleNode& operator=(const MerkleNode&);
	    bool operator==(const MerkleNode& other);
	    bool operator!=(const MerkleNode& other);

		std::string get_hash() const;
		std::shared_ptr<MerkleNode> get_left() const;
		std::shared_ptr<MerkleNode> get_right() const;
		int get_height() const; // 0 if node has no children
		static int get_count();
		//std::shared_ptr<MerkleNode> clone() const;


	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};
//~~~~~~~~~~~~~~Tree~~~~~~~~
class MerkleTree {
	public:
		MerkleTree() = delete;
	    MerkleTree(std::shared_ptr<MerkleNode> root, int numLeaves);

	    MerkleTree(const MerkleTree& other);
		MerkleTree(MerkleTree&& other) noexcept;

	    ~MerkleTree();

		MerkleTree& operator=(const MerkleTree& other);
		MerkleTree& operator=(MerkleTree&& other) noexcept;

		bool operator==(const MerkleTree& other);
		bool operator!=(const MerkleTree& other);
	    
	    std::string get_root_hash() const;
	    int get_height() const;
	    int get_size() const;

	private:
	    class Impl;
	    std::unique_ptr<Impl> pImpl;
};
//~~~~~~~~~~~~~~Container~~~~~~~
	class MerkleContainer{
	public:
		MerkleContainer();
		MerkleContainer(const std::string&);

		MerkleContainer(const MerkleContainer& other);
		MerkleContainer(MerkleContainer&& other) noexcept;	
		
		~MerkleContainer();


		MerkleContainer& operator=(const MerkleContainer& other);
	    MerkleContainer& operator=(MerkleContainer&& other) noexcept;
    	bool operator==(const MerkleContainer& other);	
		bool operator!=(const MerkleContainer& other);

		void add(const std::string&);
		int get_size() const; // how many added elements
		bool empty() const; // true if empty
		MerkleTree get_tree() const; 	// returns a pointer to the root node
		
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}// namespace MyMerkle

#endif //MERKLE_TREE