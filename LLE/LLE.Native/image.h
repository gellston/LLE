#pragma once

#ifndef LLE_IMAGE
#define LLE_IMAGE

#include <memory>

#include "lle_api.h"

namespace lleapi {
	namespace v1 {
		class impl_image;
		class image;
		using image_ptr = std::shared_ptr<image>;
		class image {
		private:
			std::unique_ptr<impl_image> instance;
		public:

#pragma region Constructor
			/// <summary>
			/// 생성자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API image();
#pragma endregion

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
			LLE_NATIVE_API unsigned int width();

			/// <summary>
			/// 이미지 높이
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API unsigned int height();

			/// <summary>
			/// 이미지 채널
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API unsigned int channel();

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
			LLE_NATIVE_API unsigned int stride();

			/// <summary>
			/// 이미지 객체가 valid한지 체크하는 함수 
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API bool valid();
	
#pragma endregion



		};
	}
}


#endif