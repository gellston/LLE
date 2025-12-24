#pragma once

#ifndef LLE_INFERENCE
#define LLE_INFERENCE

#include <memory>

#include "lle_api.h"

namespace lleapi {
	namespace v1 {
		class impl_lle;
		class lle {
		private:
			std::unique_ptr<impl_lle> impl;
		public:

#pragma region Constructor
			/// <summary>
			/// 생성자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API lle();
#pragma endregion

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
			LLE_NATIVE_API void setup();


			/// <summary>
			/// 내부 인스턴스를 정리
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void shutdown();

#pragma endregion


			
		};
	}
}


#endif