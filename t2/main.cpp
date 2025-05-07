#include "MerkleTree.h"
#include <iostream>

using namespace MyMerkle;

int main(){

	try{
		MerkleTree* tree = new MerkleTree();

		tree->add("One");
		tree->add("Two");
		tree->add("Three");
		std::cout << "Creating static tree after adding 3 values\n";
		auto static1 = tree->get_static();
		tree->add("Four");
		tree->add("Five");
		std::cout << "Creating static tree after adding 5 values\n";
		auto static2 = tree->get_static();

		std::cout << "trees have diffrent root hashes";
		if(static1->get_hash() == static2->get_hash()){
			std::cout << "THIS WONT PRINT";
		}

		std::cout << "MerkleNode instances: " << MerkleNode::get_count() << '\n';
		delete tree;
		std::cout << "MerkleNode instances after deleting mutable tree: " << MerkleNode::get_count() << '\n';

	
	}
	catch(const MerkleExcept& e){
		std::cerr << "MerkleExept: " << e.what() << '\n';
	}
	catch(const std::exception& e){
		std::cerr << "standart error: " << e.what() << '\n';
	}
	catch(...){
		std::cerr << "Unknown error: \n";
	}

	std::cout << "Out of bounds MerkleNode instances: " << MerkleNode::get_count() << '\n';
	std::cout << "Demo over\n";
	return 0;
}