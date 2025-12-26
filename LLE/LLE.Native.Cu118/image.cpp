#include "lle/image.h"
#include "lle/memoryPool.h"


#include <opencv2/opencv.hpp>
#include <stdexcept>


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

			std::weak_ptr<lleapi::v1::memoryPool> pool;
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
			throw std::runtime_error("Invalid size");
		}


		this->impl->token = pool->acquire(total_size);
		this->impl->stride = stride;
		this->impl->width = width;
		this->impl->height = height;
		this->impl->channel = channel;


		this->impl->pool = pool;

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
			throw std::runtime_error("Invalid size");
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

#pragma region Private Functions

std::weak_ptr<lleapi::v1::memoryPool>  lleapi::v1::image::pool() {
	return this->impl->pool;
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


lleapi::v1::image_ptr lleapi::v1::image::resize(image_ptr& image, const uint32_t& width, const uint32_t& height) {
	try {

		int cvMatType = CV_8UC3;

		switch (image->channel()) {
		case 1:
			cvMatType = CV_8UC1;
			break;

		case 3:
			cvMatType = CV_8UC3;
			break;

		default:
			throw std::runtime_error("Invalid image channel");
		}


		cv::Mat cvImage(cv::Size(image->width(), image->height()), cvMatType, image->data(), image->stride());
		cv::Mat resizeImage;

		cv::resize(cvImage, resizeImage, cv::Size(width, height));
		if (resizeImage.empty()) {
			throw std::runtime_error("Invalid result image");
		}

		// 메모리풀이 있을때에만
		if (!image->pool().expired()) {
			if (auto pool = image->pool().lock()) {
				auto result = lleapi::v1::image::create(resizeImage.cols, resizeImage.rows, resizeImage.channels(), pool);
				std::memcpy(result->data(), resizeImage.data, result->size());
				return result;
			}
		}

		auto result = lleapi::v1::image::create(resizeImage.cols, resizeImage.rows, resizeImage.channels());
		std::memcpy(result->data(), resizeImage.data, result->size());
		return result;
	}
	catch (...) {
		throw;
	}
}
lleapi::v1::image_ptr lleapi::v1::image::imread(const std::string & path, colorType colorType) {
	try {

		cv::ImreadModes mode = cv::IMREAD_COLOR;

		switch (colorType) {
		case colorType::color:
			mode = cv::IMREAD_COLOR;
			break;

		case colorType::gray:
			mode = cv::IMREAD_GRAYSCALE;
			break;
		}
		
		auto image = cv::imread(path, mode);

		if (image.empty()) {
			throw std::runtime_error("Invalid image");
		}

		auto result = lleapi::v1::image::create(image.cols, image.rows, image.channels());
		auto size = image.rows * image.step;
		std::memcpy(result->data(), image.data, size);

		return result;
	}
	catch (...) {
		throw;
	}
}

lleapi::v1::image_ptr lleapi::v1::image::imread(const std::string& path, colorType colorType, lleapi::v1::memoryPool_ptr pool) {
	try {

		cv::ImreadModes mode = cv::IMREAD_COLOR;

		switch (colorType) {
		case colorType::color:
			mode = cv::IMREAD_COLOR;
			break;

		case colorType::gray:
			mode = cv::IMREAD_GRAYSCALE;
			break;
		}

		auto image = cv::imread(path, mode);
		if (image.empty()) {
			throw std::runtime_error("Invalid image");
		}

		auto result = lleapi::v1::image::create(image.cols, image.rows, image.channels(), pool);
		auto size = image.rows * image.step;
		std::memcpy(result->data(), image.data, size);

		return result;
	}
	catch (...) {
		throw;
	}
}

void lleapi::v1::image::imwrite(const std::string& path, image_ptr image) {
	try {

		int matType = CV_8UC1;

		switch (image->channel()) {
		case 1:
			matType = CV_8UC1;
			break;

		case 3:
			matType = CV_8UC3;
			break;

		default:
			throw std::runtime_error("Invalid channel format");
			break;
		}

		cv::Mat cvImage(cv::Size(image->width(), image->height()), matType, image->data(), image->stride());
		cv::imwrite(path, cvImage);
		
	}
	catch (...) {
		throw;
	}
}
#pragma endregion

