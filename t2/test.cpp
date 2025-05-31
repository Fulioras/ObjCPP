#include "MerkleContainer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace MyMerkle;

void print_tree_info(std::ostream& out, const MerkleTree& tree, const std::string& label) {
    out << "== " << label << " ==\n";
    out << "Root hash: " << tree.get_root_hash() << '\n';
    out << "Tree height: " << tree.get_height() << '\n';
    out << "Tree size: " << tree.get_size() << "\n\n";
}

int main() {
    std::ofstream out("logfile.txt");
    if (!out.is_open()) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    try {
        std::vector<std::string> inputs = {
            "Alpha", "Bravo", "Charlie", "Delta", "Echo",
            "Foxtrot", "Golf", "Hotel", "India", "Juliet"
        };

        MerkleContainer container;

        for (size_t i = 0; i < inputs.size(); ++i) {
            container.add(inputs[i]);
            out << "[Added]: " << inputs[i] << '\n';

            if ((i + 1) % 3 == 0) {
                auto snapshot = container.get_tree();
                print_tree_info(out, snapshot, "Snapshot after " + std::to_string(i + 1) + " additions");
            }
        }

        out << "Container empty? " << (container.empty() ? "Yes" : "No") << '\n';
        out << "Total elements added: " << container.get_size() << "\n\n";

        std::string finalTreeHash = container.get_tree().get_root_hash();
        out << "Final tree root hash: " << finalTreeHash << '\n';

        out << "Total MerkleNode instances alive: ";// << MerkleNode::get_count() << "\n";

    } catch (const MerkleExcept& e) {
        out << "MerkleExcept: " << e.what() << '\n';
    } catch (const std::exception& e) {
        out << "Standard exception: " << e.what() << '\n';
    } catch (...) {
        out << "Unknown exception occurred.\n";
    }

    out << "Test completed. Remaining MerkleNode count: ";// << MerkleNode::get_count() << '\n';
    out.close();
    return 0;
}
