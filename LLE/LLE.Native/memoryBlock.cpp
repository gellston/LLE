#include "memoryBlock.h"


#pragma region IMPL
namespace lleapi {
	namespace v1 {
		class impl_memoryBlock {
		public:

			std::vector<uint8_t> buffer;

		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::memoryBlock::memoryBlock(const std::size_t& size) : impl(new impl_memoryBlock()) {


	try {

		if (size <= 0) {
			throw std::exception("Invalid block size");
		}

		this->impl->buffer.resize(size);


	}
	catch (...) {
		throw;
	}
}
#pragma endregion


#pragma region Destructor
lleapi::v1::memoryBlock::~memoryBlock() {

}
#pragma endregion


#pragma region Public Functions
uint8_t * lleapi::v1::memoryBlock::data() {

	if (this->impl->buffer.size() <= 0) {
		throw std::exception("Invalid buffer size");
	}


	return this->impl->buffer.data();
}


std::size_t lleapi::v1::memoryBlock::size() {
	return this->impl->buffer.size();
}
#pragma endregion


#pragma region Static Functions
lleapi::v1::memoryBlock_ptr lleapi::v1::memoryBlock::create(const std::size_t& size) {

	try {
		auto raw_pointer = new lleapi::v1::memoryBlock(size);
		return std::shared_ptr<lleapi::v1::memoryBlock>(raw_pointer);
	}
	catch (...) {
		throw;
	}
}
#pragma endregion

