
#include "lle/memoryToken.h"


#include <vector>
#include <stdexcept>


#pragma region IMPL
namespace lleapi {
	namespace v1 {
		class impl_memoryToken {
		public:
			std::size_t size = 0;
			std::size_t actual_size = 0;
			uint8_t* buffer = nullptr;
			std::function<void()> dealloc_callback;

		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::memoryToken::memoryToken(const std::size_t& actual_size, const std::size_t& size, uint8_t* buffer) : impl(new impl_memoryToken()) {
	if (size == 0) {
		throw std::runtime_error("Invalid block size");
	}

	this->impl->actual_size = actual_size;
	this->impl->size = size;
	this->impl->buffer = buffer;
	this->impl->dealloc_callback = [&] {

	};
}

lleapi::v1::memoryToken::memoryToken(const std::size_t& actual_size, const std::size_t& size, uint8_t* buffer, const std::function<void()>& dealloc_callback) : impl(new impl_memoryToken()) {
	if (size == 0) {
		throw std::runtime_error("Invalid block size");
	}

	this->impl->actual_size = actual_size;
	this->impl->size = size;
	this->impl->buffer = buffer;
	this->impl->dealloc_callback = dealloc_callback;
}
#pragma endregion


#pragma region Destructor
lleapi::v1::memoryToken::~memoryToken() {

	try {
		impl->dealloc_callback();
	}
	catch (...) {

	}

}
#pragma endregion


#pragma region Public Functions


uint8_t * lleapi::v1::memoryToken::data() {
	try {
		if (this->impl->size == 0) {
			throw std::runtime_error("Invalid block size");
		}

		return this->impl->buffer;
	}
	catch (...) {
		throw;
	}
}

std::size_t lleapi::v1::memoryToken::size() {
	try {

		return this->impl->size;
	}
	catch (...) {
		throw;
	}
}

std::size_t lleapi::v1::memoryToken::actualSize() {
	try {

		return this->impl->actual_size;
	}
	catch (...) {
		throw;
	}
}


#pragma endregion
