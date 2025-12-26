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
			/// Constructor
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
			/// Destructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~image();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// Image width
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t width();

			/// <summary>
			/// Image height
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t height();

			/// <summary>
			/// Image channel
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint32_t channel();

			/// <summary>
			/// Image buffer start pointer
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API unsigned char* data();

			/// <summary>
			/// Total image pixel size
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API std::size_t size();

			/// <summary>
			/// Image stride size
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