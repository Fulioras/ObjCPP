#include "MerkleContainer.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace MyMerkle;

void log(std::ofstream& out, const std::string& message) {
    std::cout << message << std::endl;
    out << message << std::endl;
}

int main() {
    std::ofstream logfile("logfile.txt");
    if (!logfile.is_open()) {
        std::cerr << "Failed to open logfile.txt" << std::endl;
        return 1;
    }

    log(logfile, "===== MerkleContainer Test Start =====");

    MerkleContainer container;

    log(logfile, "Adding elements to MerkleContainer...");
    std::vector<std::string> inputs = {"alpha", "beta", "gamma", "delta"};
    for (const auto& input : inputs) {
        container.add(input);
        log(logfile, "Added: " + input);
    }

    log(logfile, "Container size after additions: " + std::to_string(container.get_size()));
    log(logfile, "Is Container empty? " + std::string(container.empty() ? "Yes" : "No"));

    MerkleTree snapshot1 = container.get_tree();
    log(logfile, "Snapshot1 Root Hash: " + snapshot1.get_root_hash());
    log(logfile, "Snapshot1 Height: " + std::to_string(snapshot1.get_height()));

    log(logfile, "Snapshot1 node hashes by depth:");
    int maxDepth = snapshot1.get_height();
    for (int depth = 0; depth <= maxDepth; ++depth) {
        int count = 1 << depth; // 2^depth, can be replaced with pow(2, depth) if needed.
        log(logfile, "Depth " + std::to_string(depth) + ":");
        for (int i = 0; i < count; ++i) {
            try {
                std::string hash = snapshot1.get_hash(depth, i);
                log(logfile, "  Node " + std::to_string(i) + ": " + hash);
            } catch (const MerkleExcept& e) {
                std::cout << "  Node " << i << ": " << e.what() << "\n";
            }
        }
    }

    log(logfile, "\nAdding more elements...");
    container.add("epsilon");
    container.add("zeta");

    MerkleTree snapshot2 = container.get_tree();
    log(logfile, "Snapshot2 Root Hash: " + snapshot2.get_root_hash());
    log(logfile, "Snapshot2 Height: " + std::to_string(snapshot2.get_height()));

    if (snapshot1.get_root_hash() != snapshot2.get_root_hash()) {
        log(logfile, "PASS: Snapshots have different root hashes after modification.");
    } else {
        log(logfile, "FAIL: Snapshots should not have the same root hash.");
    }

    log(logfile, "\nAdding duplicates to check hash uniqueness...");
    container.add("alpha");
    MerkleTree snapshot3 = container.get_tree();
    log(logfile, "Snapshot3 Root Hash (with duplicate 'alpha'): " + snapshot3.get_root_hash());

    if (snapshot2.get_root_hash() != snapshot3.get_root_hash()) {
        log(logfile, "PASS: Duplicate data alters the tree as expected.");
    } else {
        log(logfile, "FAIL: Duplicate data should alter the root hash.");
    }

    log(logfile, "\n===== MerkleContainer Test Complete =====");
    logfile.close();
    return 0;
}
