#include "MerkleContainer.h"
#include <iostream>

using namespace MyMerkle;

int main() {
    try {
        std::cout << "Initializing MerkleContainer...\n";
        MerkleContainer container;

        // Adding elements to the container
        std::cout << "\n--- Adding Leaves ---\n";
        container.add("One");
        container.add("Two");
        container.add("Three");

        std::cout << "\nCreating MerkleTree from current container (3 values)...\n";
        MerkleTree tree1 = container.get_tree();
        std::cout << "Tree1 Root Hash: " << tree1.get_root_hash() << '\n';
        std::cout << "Tree1 Height: " << tree1.get_height() << '\n';

        // Adding more elements
        container.add("Four");
        container.add("Five");

        std::cout << "\nCreating MerkleTree from updated container (5 values)...\n";
        MerkleTree tree2 = container.get_tree();
        std::cout << "Tree2 Root Hash: " << tree2.get_root_hash() << '\n';
        std::cout << "Tree2 Height: " << tree2.get_height() << '\n';

        // Copy tree2 to tree3 using copy constructor
        std::cout << "\nCopying tree2 into tree3...\n";
        MerkleTree tree3 = tree2;

        std::cout << "Tree3 Root Hash: " << tree3.get_root_hash() << '\n';
        std::cout << "Tree3 Height: " << tree3.get_height() << '\n';

        if (tree3.get_root_hash() == tree2.get_root_hash()) {
            std::cout << "GOOD. Tree3 is a valid deep copy of Tree2\n";
        } else {
            std::cout << "BAD. Tree3 and Tree2 do not match!\n";
        }

        std::cout << "\n--- Hash Comparison ---\n";
        if (tree1.get_root_hash() == tree2.get_root_hash()) {
            std::cout << "BAD Tree1 and Tree2 have the same root hash (unexpected)\n";
        } else {
            std::cout << "GOOD. Tree1 and Tree2 have different root hashes (as expected)\n";
        }

        //std::cout << "\nMerkleNode instances in memory: " << MerkleNode::get_count() << '\n';

    } catch (const MerkleExcept& e) {
        std::cerr << "MerkleExcept: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Unknown error occurred.\n";
    }

    //std::cout << "\nRemaining MerkleNode instances: " << MerkleNode::get_count() << '\n';
    std::cout << "GOOD. Demo completed successfully.\n";

    return 0;
}
