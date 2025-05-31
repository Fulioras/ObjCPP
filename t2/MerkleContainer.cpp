#include "MerkleContainer.h"
#include <iostream>


namespace MyMerkle{

//~~~~~~~~~~~~~~~~hashing implementation

	std::string sha256(const std::string& inputStr){
		unsigned char hash[SHA256_DIGEST_LENGTH];
		const unsigned char* data = (const unsigned char*)inputStr.c_str();
		SHA256(data, inputStr.size(), hash); // "Do your magic, Magicman!"

	    std::stringstream ss;
	    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	    {
	        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	        //Turns bynary output to a readable hexadecimal format
	    }
	    return ss.str();
	}

//~~~~~~~~~~~~~~~~~Exception implementation
	MerkleExcept::MerkleExcept(const std::string& msg)
       							 : std::runtime_error(msg) {}

//~~~~~~~~~~~~~~~~~node implementation

class MerkleNode {
private:
    static int counter;
    std::string hash;
    std::shared_ptr<MerkleNode> left;
    std::shared_ptr<MerkleNode> right;
    int height = 0;

public:
    // Leaf constructor
    MerkleNode(const std::string& data)
        : hash(sha256(data)), left(nullptr), right(nullptr), height(0) {
        ++counter;
    }

    // Internal node constructor
    MerkleNode(std::shared_ptr<MerkleNode> l, std::shared_ptr<MerkleNode> r)
        : left(std::move(l)), right(std::move(r)) {
        if (!left || !right) {
            throw MerkleExcept("Cannot create internal MerkleNode with null children.");
        }
        hash = sha256(left->get_hash() + right->get_hash());
        height = std::max(left->get_height(), right->get_height()) + 1;
        ++counter;
    }

    // Copy constructor from shared_ptr
    MerkleNode(const std::shared_ptr<MerkleNode>& other) {
        if (!other) throw std::invalid_argument("Can't copy from null node.");

        hash = other->get_hash();
        height = other->get_height();
        if (height > 0) {
            left = std::make_shared<MerkleNode>(other->get_left());
            right = std::make_shared<MerkleNode>(other->get_right());
        }
        ++counter;
    }

    // Copy constructor
    MerkleNode(const MerkleNode& other)
        : hash(other.hash), height(other.height) {
        left = other.left ? std::make_shared<MerkleNode>(*other.left) : nullptr;
        right = other.right ? std::make_shared<MerkleNode>(*other.right) : nullptr;
        ++counter;
    }

    // Destructor
    ~MerkleNode() {
        --counter;
    }

    // Assignment operator
    MerkleNode& operator=(const MerkleNode& other) {
        if (this != &other) {
            hash = other.hash;
            height = other.height;
            left = other.left ? std::make_shared<MerkleNode>(*other.left) : nullptr;
            right = other.right ? std::make_shared<MerkleNode>(*other.right) : nullptr;
        }
        return *this;
    }

    // Equality operators
    bool operator==(const MerkleNode& other) const {
        return hash == other.hash && height == other.height;
    }

    bool operator!=(const MerkleNode& other) const {
        return !(*this == other);
    }

    // Accessors
    std::string get_hash() const {
        return hash;
    }

    std::shared_ptr<MerkleNode> get_left() const {
        return left;
    }

    std::shared_ptr<MerkleNode> get_right() const {
        return right;
    }

    int get_height() const {
        return height;
    }

    static int get_count() {
        return counter;
    }
};

// Initialize static member
int MerkleNode::counter = 0;
//~~~~~~~~~~~~~~~~Tree implementation
	class MerkleTree::Impl {
	private:
		friend class MerkleTree;
	    std::shared_ptr<MerkleNode> root;
	    int leafCount;

	public:
	    Impl(std::shared_ptr<MerkleNode> rootNode, int numLeaves)
	        					: root(std::move(rootNode)), leafCount(numLeaves) {
	        if (!root) {
	            throw std::invalid_argument("Cannot construct MerkleTree with null root.");
	        }	
	    }																
	    Impl(const Impl& other): root(other.root ? std::make_shared<MerkleNode>(*other.root) : nullptr),
						          leafCount(other.leafCount) {}

	};

	// Constructor
	MerkleTree::MerkleTree(std::shared_ptr<MerkleNode> root, int numLeaves)
	    						: pImpl(std::make_unique<Impl>(std::move(root), numLeaves)) {}

	MerkleTree::MerkleTree(const MerkleTree& other)
							    : pImpl(std::make_unique<Impl>(*other.pImpl)) {}
	MerkleTree::MerkleTree(MerkleTree&& other) noexcept = default;

	// Destructor
	MerkleTree::~MerkleTree() = default;

	// Operators
	bool MerkleTree::operator==(const MerkleTree& other) {
	    return get_root_hash() == other.get_root_hash() && get_height() == other.get_height();
	}

	bool MerkleTree::operator!=(const MerkleTree& other) {
	    return !(*this == other);
	}

	MerkleTree& MerkleTree::operator=(const MerkleTree& other) {
	    if (this != &other) {
	        pImpl = std::make_unique<Impl>(*other.pImpl);
	    }
	    return *this;
	}

	MerkleTree& MerkleTree::operator=(MerkleTree&& other) noexcept {
	    if (this != &other) {
	        pImpl = std::move(other.pImpl);
	    }
	    return *this;
	}

	// Interaction
	std::string MerkleTree::get_root_hash() const {
	    return pImpl->root->get_hash();
	}

	int MerkleTree::get_height() const {
	    return pImpl->root->get_height();
	}

	int MerkleTree::get_size() const {
	    return pImpl->leafCount;
	}

//~~~~~~~~~~~~~~~~Container implementation
	class MerkleContainer::Impl{
	private:
		friend class MerkleContainer;
		std::stack<std::shared_ptr<MerkleNode>> roots; 	// Merkle trees of diffrent heights (smallest on top!)
		int leafCount = 0;

	public:
		Impl() = default;
		Impl(std::string data){
			add(data);
		}
		Impl(const Impl& other) : roots(other.roots), leafCount(other.leafCount) {}

		~Impl() = default;

		void add(const std::string& data){
			if(data.empty()){
				
				throw std::invalid_argument("Cannot create MerkleNode with empty data.");
			}
			roots.push(std::make_shared<MerkleNode>(data));
			leafCount++;

			// Count how many trailing 1s in binary representation of (leafCount - 1)
			for(int i = merge_count(); i > 0; i--){
				if (roots.size() < 2) {
					
				    throw std::underflow_error("Not enough nodes in the stack to merge.");
				}

				std::shared_ptr<MerkleNode> temp = roots.top();
				roots.pop();

				std::shared_ptr<MerkleNode> temp2 = roots.top();
				roots.pop();

				roots.push(std::make_shared<MerkleNode>(temp2, temp));
			}	
		}

		// makes a normal MerkleContainer(full binary tree) by filling all empty 
		//tree branches with the smallest tree inside the stack.
		std::shared_ptr<MerkleNode> get_tree(){
			if (roots.empty()) {
	        	throw MerkleExcept("Cannot get root hash of an empty Merkle Tree.");
	    	}
			
			std::stack<std::shared_ptr<MerkleNode>> copyRoots = roots;

			std::shared_ptr<MerkleNode> smallBranch = copyRoots.top();		
			copyRoots.pop();

			while(copyRoots.size() > 0){ // making full tree

				std::shared_ptr<MerkleNode> next = copyRoots.top();
				copyRoots.pop();

				while(smallBranch->get_height() < next->get_height()){
					smallBranch = std::make_shared<MerkleNode>(smallBranch, smallBranch);
				}	

				smallBranch = std::make_shared<MerkleNode>(next, smallBranch);
			}

			return smallBranch;
		}

		// Fills up all bits with '1' then adds 1
		int get_tree_size() const{
			int n = leafCount;
			if (n == 0)
		        return 1;

		    n--;         
		    n |= n >> 1;         
		    n |= n >> 2;
		    n |= n >> 4;
		    n |= n >> 8;
		    n |= n >> 16;
		    return n + 1;  
		}

	private:
		//checks by counting the '0' bits before the leading '1' bit
		int merge_count() const{
			int n = leafCount-1;
			int count = 0;

			while(n & 1){
				count++;
				n >>= 1;
			}

			return count;
		}
	};

	//Constructors
	MerkleContainer::MerkleContainer() 
					: pImpl(std::make_unique<Impl>()){}

	MerkleContainer::MerkleContainer(const std::string& data)
					: pImpl(std::make_unique<Impl>(data)){}

	MerkleContainer::MerkleContainer(const MerkleContainer& other)
    : pImpl(std::make_unique<Impl>(*other.pImpl)) {}

	MerkleContainer::MerkleContainer(MerkleContainer&& other) noexcept = default;

	// Destructor
	MerkleContainer::~MerkleContainer() = default;

	//Operators
	bool MerkleContainer::operator==(const MerkleContainer& other) {
	    return get_size() == other.get_size() &&
	           get_tree().get_root_hash() == other.get_tree().get_root_hash();
	}

	bool MerkleContainer::operator!=(const MerkleContainer& other) {
	    return !(*this == other);
	}

	MerkleContainer& MerkleContainer::operator=(const MerkleContainer& other) {
	    if (this != &other) {
	        pImpl = std::make_unique<Impl>(*other.pImpl);
	    }
	    return *this;
	}

	MerkleContainer& MerkleContainer::operator=(MerkleContainer&& other) noexcept {
	    if (this != &other) {
	        pImpl = std::move(other.pImpl);
	    }
	    return *this;
	}

	// Interaction
	void MerkleContainer::add(const std::string& data){
		pImpl->add(data);
	}

	int MerkleContainer::get_size() const{
		return pImpl->leafCount;
	}

	bool MerkleContainer::empty() const{
		return pImpl->roots.empty();
	}

	MerkleTree MerkleContainer::get_tree() const{
    	return MerkleTree(pImpl->get_tree(), pImpl->get_tree_size());
	}

} // namespace MyMerkle