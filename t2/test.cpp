#include "MerkleTree.h"
#include <iostream>
#include <cassert>
#include <fstream>

using namespace MyMerkle;

int main(){

	std::cout << "TEST START\n";
	std::ofstream logfile("logfile.txt");

	if(!logfile){
		std::cerr << "logfile null\n";
		return 1;
	}

	logfile << "~~~~~Testing adding to a tree:\n";

	{
		logfile << "~Creating tree with one node\n";
		MerkleTree* tree = new MerkleTree("vienas");
		std::string base = "AA";
		logfile << "~Adding 100 diffrent nodes\n";
		for(int i = 0; i < 10; i++){
			base[0]++;
			base[1] = 'A';
			for(int j = 0; j < 10; j++){
				base[1]++;
				tree->add(base);
			}
		}

		logfile << "~Node addition complete\n";
		logfile << "Total nodes in MerkleNode instances: " << MerkleNode::get_count() << '\n';
		delete tree;
	}
	assert(MerkleNode::get_count() == 0);
	logfile << "MerkleNode instances out of bounds: " << MerkleNode::get_count() << '\n';
	logfile << "~~~~~Addition testing - PASSED\n\n";


	logfile << "~~~~~Testing get_static() functionality:\n";
	{
		logfile << "~Initializing tree and adding 3 elements\n";
		MerkleTree* tree = new MerkleTree();

		tree->add("labas");
		tree->add("geras");
		tree->add("super");
		logfile << "~Making a static tree from the original\n";
		auto stat = tree->get_static();
			//sukonstruotas stat atrodys taip:		  	  1233)
			//										   12)	  33)
			//										  1) 2)  3) 3)

		// sukuriam rankiniu budu
		std::string vienasDu = sha256(sha256("labas")+sha256("geras"));
		std::string trysTrys = sha256(sha256("super")+sha256("super"));
		std::string manualHash = sha256(vienasDu+trysTrys);
		logfile << "~Comparing manually created root hash with the static tree root hash\n";
		assert(manualHash == stat->get_hash());
		logfile << "Both hashes are the same (correct behaviour)\n";
		
		logfile << manualHash << "<-- manual hash\n";
		logfile << stat->get_hash() << "<-- static tree hash\n";
		

		logfile << "Instance count before: " << MerkleNode::get_count() << '\n';
		delete tree;
	}
	assert(MerkleNode::get_count() == 0);
	logfile << "Instance count out of bounds: "<< MerkleNode::get_count() << '\n';
	logfile << "~~~~~get_static() functionality - PASSED\n\n";

	logfile << "\nSUCCESS\n";

	logfile.close();
	std::cout << "TEST ENDED\n";
	return 0;
}