// Author: Augustinas Bickaitis, 2025

// An append-only MerkleContainer, with a functionality to 
//				make snapshot tree (MerkleTree)
	

// NEED TO HAVE OPEN SSL LIBRARY INSTALLED!!

// Basic use is demonstrated on main.cpp

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

//~~~~~~~~~~HASHING~~~~~~~~~~~~
	std::string sha256(const std::string& );// Hashing function from OpenSSl

//~~~~~~~~~~EXCEPTION~~~~~~
	class MerkleExcept : public std::runtime_error {
    public:
        explicit MerkleExcept(const std::string& msg);
    };
//~~~~~~~~~~~~~NODE~~~~~~~~~~
class MerkleNode;
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
	    int get_height() const; // 0 if only root node
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