#include "MerkleTree.h"

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

//~~~~~~~~~~~~~~~~~node implementation

	class MerkleNode::Impl{
		private:
			static int counter;
			std::string hash;
			std::shared_ptr<MerkleNode> left;
			std::shared_ptr<MerkleNode> right;
			int height = 0;

		public:
			Impl(const std::string& data){
				counter++;
				left = nullptr;
				right = nullptr;
				hash = sha256(data);
			}

			Impl(std::shared_ptr<MerkleNode> l, std::shared_ptr<MerkleNode> r) : left(l), right(r){
				if (!l || !r) {
	        		throw MerkleExcept("Cannot create internal MerkleNode with null children.");
	    		}

				counter++;
				hash = sha256(left->get_hash() + right->get_hash());
		    	height = left->get_height() +1;
			}

			~Impl(){
				counter--;
			}

			std::string get_hash() const {
				return hash;
			}
			std::shared_ptr<MerkleNode> get_left() const{
				return left;
			}
			std::shared_ptr<MerkleNode> get_right() const{
				return right;
			}
			int get_height() const{
				return height;
			}
			static int get_counter(){
				return counter;
			}

			std::shared_ptr<MerkleNode> clone() const {
    			return std::make_shared<MerkleNode>(this->get_hash());
			}
	};

	int MerkleNode::Impl::counter = 0;

	MerkleNode::MerkleNode(const std::string& data) 
							: pImpl(std::make_unique<Impl>(data)){}

	MerkleNode::MerkleNode(std::shared_ptr<MerkleNode> l, std::shared_ptr<MerkleNode> r) 
							: pImpl(std::make_unique<Impl>(l, r)){}

	MerkleNode::~MerkleNode() = default;

	std::string MerkleNode::get_hash() const{
		return pImpl->get_hash();
	}

	std::shared_ptr<MerkleNode> MerkleNode::get_left() const{
		return pImpl->get_left();
	}

	std::shared_ptr<MerkleNode> MerkleNode::get_right() const{
		return pImpl->get_right();
	}

	int MerkleNode::get_height() const{
		return pImpl->get_height();
	}

	int MerkleNode::get_count() {
		return Impl::get_counter();
	}
//~~~~~~~~~~~~~~~~tree implementation

	class MerkleTree::Impl{
	private:
		std::stack<std::shared_ptr<MerkleNode>> roots; 	// Merkle trees of diffrent heights (smallest on top!)
		int leafCount = 0;

	public:
		Impl() = default;
		Impl(std::string data){
			add(data);
		}

		~Impl() = default;

		void add(const std::string& data){
			if(data.empty()){
				
				throw std::invalid_argument("Cannot create MerkleNode with empty data.");
			}
			roots.push(std::make_shared<MerkleNode>(data));
			leafCount++;

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

		int get_size() const{
			return leafCount;
		}
		bool empty() const{
			return roots.empty();
		}

		// makes a normal MerkleTree(full binary tree) by filling all empty 
		//tree branches with the smallest tree inside the stack.
		std::shared_ptr<MerkleNode> get_static(){
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

	private:
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

	MerkleTree::MerkleTree() 
					: pImpl(std::make_unique<Impl>()){}
	MerkleTree::MerkleTree(const std::string& data)
					: pImpl(std::make_unique<Impl>(data)){}

	MerkleTree::~MerkleTree() = default;

	void MerkleTree::add(const std::string& data){
		return pImpl->add(data);
	}

	int MerkleTree::get_size() const{
		return pImpl->get_size();
	}

	bool MerkleTree::empty() const{
		return pImpl->empty();
	}

	std::shared_ptr<MerkleNode> MerkleTree::get_static() { 
		return pImpl->get_static();
	}

} // namespace MyMerkle