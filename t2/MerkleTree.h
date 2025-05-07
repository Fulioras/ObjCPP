// Author: Augustinas Bickaitis, 2025

// An append-only MerkleTree, with a functionality to 
//				create view-only static snapshots of the tree
	

// NEED TO HAVE OPEN SSL LIBRARY INSTALLED!!


/*
//~~~~~~~~~~~~~BASIC USE~~~~~~~~~~~

MerkleTree* tree = new MerkleTree(); 		//creates empty tree
tree.add("string");		//adds hash value of "string" to tree
tree.add("string2");	//adds hash value of "string2" to tree

auto static1 = tree->get_static(); 
						// .get_static() returns 
						//the root node of a view-only tree
tree.add("string3");
MerkleNode static2 = tree.get_static();
						//static1 and static2 
						//have diffrent root hash strings
}
//~~~~~~~~~~~~~~CHECKING~~~~~~~~~~
tree.empty(); 	//returns true if empty
tree.get_size; 	//returns the number of added elements

static1.get_height; //returns node height in tree
static1.get_hash(); //retruns nodes hash string

//~~~~~~~~~~~TRAVERSING STATIC TREE~~~~~~~
static1.get_left()
static1.get_right() 
		//returns a shared_ptr<MerkleNode>, throws exeption 
		//	if left/right don't exist (nodes height == 0)

*/
#include <memory> //shared and unique ptr
#include <iomanip> // hex function for line formating
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
	    explicit MerkleExcept(const std::string& msg) : runtime_error(msg){}
	};

//~~~~~~~~~~~~~NODE~~~~~~~~~~
	class MerkleNode {
	public:
		MerkleNode(const std::string& ); // creates a 0 height node from an input
		MerkleNode(std::shared_ptr<MerkleNode>, std::shared_ptr<MerkleNode>); //creates new node based on its left and right nodes
		
		~MerkleNode();
		std::string get_hash() const;
		std::shared_ptr<MerkleNode> get_left() const;
		std::shared_ptr<MerkleNode> get_right() const;
		int get_height() const; // 0 if node has no children
		static int get_count(); //static;

	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

//~~~~~~~~~~~~~~TREE~~~~~~~
	class MerkleTree{
	public:
		MerkleTree();
		MerkleTree(const std::string&);
		~MerkleTree();
		void add(const std::string&);
		int get_size() const; // how many added elements
		bool empty() const; // true if empty
		std::shared_ptr<MerkleNode> get_static(); 	// returns a pointer to the root node
		
	private:
		class Impl;
		std::unique_ptr<Impl> pImpl;
	};

}// namespace MyMerkle

#endif //MERKLE_TREE