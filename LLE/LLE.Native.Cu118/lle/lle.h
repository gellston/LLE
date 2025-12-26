#pragma once

#ifndef LLE_INFERENCE
#define LLE_INFERENCE

#include <memory>
#include <string>

#include "lle_api.h"
#include "image.h"

namespace lleapi {
	namespace v1 {

		enum device{
			cpu,
			cuda
		};

		class memoryPool;
		using memoryPool_ptr = std::shared_ptr<memoryPool>;

		class lle;
		using lle_ptr = std::shared_ptr<lle>;
		class impl_lle;
		class lle {
		private:
#pragma region Private Property
			std::unique_ptr<impl_lle> impl;
#pragma endregion

#pragma region Constructor
			/// <summary>
			/// 생성자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API lle(memoryPool_ptr pool);

			LLE_NATIVE_API lle();
#pragma endregion
		public:



#pragma region Destructor
			/// <summary>
			/// 파괴자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~lle();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// 내부 변수들과 인스턴스들을 초기화 및 셋팅
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void setup(const std::string & path, device _device);


			/// <summary>
			/// 내부 인스턴스를 정리
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void shutdown();



			/// <summary>
			/// 이미지 파일을 불러와 예측한 결과를 리턴
			/// </summary>
			/// <param name="path"></param>
			/// <returns></returns>
			LLE_NATIVE_API image_ptr predict(const std::string & path);



			/// <summary>
			/// 입력 이미지를 넣어서 예측
			/// </summary>
			/// <param name="path"></param>
			/// <returns></returns>
			LLE_NATIVE_API image_ptr predict(image_ptr image);

#pragma endregion

#pragma region Static Functions
			/// <summary>
			/// LLE 객체 생성
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API static lle_ptr create();

			LLE_NATIVE_API static lle_ptr create(memoryPool_ptr pool);
#pragma endregion


			
		};
	}
}


#endif