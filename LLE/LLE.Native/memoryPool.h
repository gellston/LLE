#pragma once

#ifndef LLE_MEMORY_POOL
#define LLE_MEMORY_POOL

#include <memory>


#include "lle_api.h"
#include "memoryToken.h"

namespace lleapi {
	namespace v1 {

		class memoryPool;
		using memoryPool_ptr = std::shared_ptr<memoryPool>;

		class impl_memoryPool;
		class memoryPool {
		private:

#pragma region Private Property
			std::unique_ptr<impl_memoryPool> impl;
#pragma endregion

#pragma region Constructor
			/// <summary>
			/// 생성자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API memoryPool(std::initializer_list<std::size_t> bins = { 3145728, 786432, 196608 }, std::size_t unit_size = 64, double efficient_rate = 0.9);
#pragma endregion

#pragma region Private Functions
			/// <summary>
			/// Align Size를 계산하기 위한 함수
			/// </summary>
			/// <param name="size"></param>
			/// <param name="unit_size"></param>
			/// <returns></returns>
			std::size_t computeAlignSize(std::size_t size);


			/// <summary>
			/// 메모리블록을 찾는 함수. 만약 없다면 생성
			/// </summary>
			/// <param name="aligned_size"></param>
			/// <returns></returns>
			memoryToken_ptr findToken(std::size_t size);
#pragma endregion


		public:
#pragma region Destructor
			/// <summary>
			/// 파괴자
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~memoryPool();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// 메모리 풀로부터 메모리 블록을 가져오는 함수
			/// </summary>
			/// <param name="size"></param>
			/// <returns></returns>
			LLE_NATIVE_API memoryToken_ptr acquire(const std::size_t& size);

			/// <summary>
			/// 메모리풀 bin을 전부 초기화하는 함수
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void reset();


			/// <summary>
			/// 현재 설정된 EfficientRate를 리턴
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API double efficientRate();

			/// <summary>
			/// EfficientRate를 설정하는 함수
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void efficientRate(double rate);

#pragma endregion

#pragma region Static Functions
			/// <summary>
			/// 메모리풀 객체를 생성하는 함수
			/// </summary>
			/// <param name="bins"></param>
			/// <returns></returns>
			LLE_NATIVE_API static memoryPool_ptr create(std::initializer_list<std::size_t> bins = { 3145728, 786432, 196608 }, const std::size_t& unit_size = 64, double efficient_rate = 0.9);
#pragma endregion


		};
	}
}


#endif