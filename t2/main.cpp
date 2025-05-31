#include "MerkleContainer.h"
#include <iostream>

using namespace MyMerkle;

int main() {
    // Create a MerkleContainer
    MerkleContainer* tree = new MerkleContainer();

    // Add some values
    tree->add("apple");
    tree->add("banana");
    tree->add("cherry");

    std::cout << "Tree Size: " << tree->get_size() << std::endl;
    std::cout << "Is Empty? " << (tree->empty() ? "Yes" : "No") << std::endl;

    // Get a snapshot of the current tree
    MerkleTree snapshot1 = tree->get_tree();
    std::cout << "Snapshot1 Height: " << snapshot1.get_height() << std::endl;
    std::cout << "Snapshot1 Root Hash: " << snapshot1.get_root_hash() << std::endl;

    // Add more data
    tree->add("date");
    tree->add("elderberry");

    // New snapshot
    MerkleTree snapshot2 = tree->get_tree();
    std::cout << "\nAfter adding more items:\n";
    std::cout << "Snapshot2 Height: " << snapshot2.get_height() << std::endl;
    std::cout << "Snapshot2 Root Hash: " << snapshot2.get_root_hash() << std::endl;

    // Compare the root hashes to demonstrate tree evolution
    if (snapshot1.get_root_hash() != snapshot2.get_root_hash()) {
        std::cout << "\nRoot hashes are different, as expected." << std::endl;
    } else {
        std::cout << "\nUnexpected: root hashes are the same!" << std::endl;
    }

    delete tree;
    return 0;
}
