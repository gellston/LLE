#include "lle/image.h"
#include "lle/memoryPool.h"


#pragma region Impl
namespace lleapi {
	namespace v1 {
		class impl_image {
		public:
			uint32_t stride = 0;
			std::vector<uint8_t> buffer;
			uint32_t width = 0;
			uint32_t height = 0;
			uint32_t channel = 0;

			memoryToken_ptr token;
		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::image::image(const uint32_t& width, const uint32_t& height, const uint32_t& channel, lleapi::v1::memoryPool_ptr pool) : impl(new impl_image()) {
	try {

		auto total_size = (std::size_t)width * (std::size_t)height * (std::size_t)channel;
		auto stride = (uint32_t)width * (uint32_t)channel;

		if (stride == 0 || total_size == 0) {
			throw std::exception("Invalid size");
		}


		this->impl->token = pool->acquire(total_size);
		this->impl->stride = stride;
		this->impl->width = width;
		this->impl->height = height;
		this->impl->channel = channel;


	}
	catch (...) {
		throw;
	}
}

lleapi::v1::image::image(const uint32_t& width, const uint32_t& height, const uint32_t& channel) : impl(new impl_image()) {
	try {

		auto total_size = (std::size_t)width * (std::size_t)height * (std::size_t)channel;
		auto stride = (uint32_t)width * (uint32_t)channel;

		if (stride == 0 || total_size == 0) {
			throw std::exception("Invalid size");
		}


		this->impl->buffer.resize(total_size);
		this->impl->stride = stride;
		this->impl->token = std::shared_ptr<memoryToken>(new memoryToken(total_size, total_size, this->impl->buffer.data()));
		this->impl->width = width;
		this->impl->height = height;
		this->impl->channel = channel;

	}
	catch (...) {
		throw;
	}
}
#pragma endregion


#pragma region Destructor
lleapi::v1::image::~image() {

}
#pragma endregion


#pragma region Public Functions
uint32_t lleapi::v1::image::width() {
	return this->impl->width;
}

uint32_t lleapi::v1::image::height() {
	return this->impl->height;
}

uint32_t lleapi::v1::image::channel() {
	return this->impl->channel;
}

uint32_t lleapi::v1::image::stride() {
	return this->impl->stride;
}

uint8_t* lleapi::v1::image::data() {
	return this->impl->token->data();
}

std::size_t lleapi::v1::image::size() {
	return this->impl->token->size();
}
#pragma endregion

#pragma region Static Functions
lleapi::v1::image_ptr lleapi::v1::image::create(const uint32_t& width, const uint32_t& height, const uint32_t& channel) {
	try {
		return std::shared_ptr<lleapi::v1::image>(new image(width, height, channel));
	}
	catch (...) {
		throw;
	}
}
lleapi::v1::image_ptr lleapi::v1::image::create(const uint32_t& width, const uint32_t& height, const uint32_t& channel, memoryPool_ptr pool) {
	try {
		return std::shared_ptr<lleapi::v1::image>(new image(width, height, channel, pool));
	}
	catch (...) {
		throw;
	}
}
#pragma endregion

