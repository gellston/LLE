#pragma once

#ifndef LLE_IMAGE
#define LLE_IMAGE

#include <memory>
#include <string>

#include "lle_api.h"

namespace lleapi {
	namespace v1 {

		enum class colorType {
			gray,
			color
		};

		class memoryPool;
		using memoryPool_ptr = std::shared_ptr<memoryPool>;
		class impl_image;
		class image;
		using image_ptr = std::shared_ptr<image>;
		class image {
		private:
#pragma region Private Property
			std::unique_ptr<impl_image> impl;
#pragma endregion

#pragma region Constructor
			/// <summary>
			/// 생성자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API image(const uint32_t & width, const uint32_t& height, const uint32_t& channel);
			LLE_NATIVE_API image(const uint32_t& width, const uint32_t& height, const uint32_t& channel, memoryPool_ptr pool);
#pragma endregion

#pragma region Private Functions
			std::weak_ptr<memoryPool> pool();
#pragma endregion


		public:

#pragma region Destructor
			/// <summary>
			/// 파괴자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~image();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// 이미지 너비
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t width();

			/// <summary>
			/// 이미지 높이
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t height();

			/// <summary>
			/// 이미지 채널
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t channel();

			/// <summary>
			/// 이미지 포인터의 시작주소 
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API unsigned char* data();

			/// <summary>
			/// 이미지 전체 픽셀 크기
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API std::size_t size();

			/// <summary>
			/// stride 크기 
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t stride();

	
			
#pragma endregion

#pragma region Static Functions
			LLE_NATIVE_API static image_ptr create(const uint32_t& width, const uint32_t& height, const uint32_t& channel);
			LLE_NATIVE_API static image_ptr create(const uint32_t& width, const uint32_t& height, const uint32_t& channel, memoryPool_ptr pool);
			LLE_NATIVE_API static image_ptr resize(image_ptr & image, const uint32_t& width, const uint32_t& height);
			LLE_NATIVE_API static image_ptr imread(const std::string& path, colorType colorType);
			LLE_NATIVE_API static image_ptr imread(const std::string& path, colorType colorType, memoryPool_ptr pool);
			LLE_NATIVE_API static void imwrite(const std::string& path, image_ptr image);
#pragma endregion


		};
	}
}


#endif