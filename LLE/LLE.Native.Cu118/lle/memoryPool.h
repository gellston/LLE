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
			std::shared_ptr<impl_memoryPool> impl;
#pragma endregion

#pragma region Constructor
			/// <summary>
			/// Constructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API memoryPool(std::initializer_list<std::size_t> bins = { 3145728, 786432, 196608 }, std::size_t unit_size = 64, double minRequestedToBinRatio = 0.9);
#pragma endregion

#pragma region Private Functions
			/// <summary>
			/// compute align size
			/// </summary>
			/// <param name="size"></param>
			/// <param name="unit_size"></param>
			/// <returns></returns>
			std::size_t computeAlignSize(std::size_t size);


			/// <summary>
			/// find memory block from internal inactive memory table
			/// </summary>
			/// <param name="aligned_size"></param>
			/// <returns></returns>
			memoryToken_ptr findToken(std::size_t requested_size, std::size_t aligned_size);
#pragma endregion


		public:
#pragma region Destructor
			/// <summary>
			/// Destructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~memoryPool();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// Acquire memory block (memory token) from inactive memory table
			/// </summary>
			/// <param name="size"></param>
			/// <returns></returns>
			LLE_NATIVE_API memoryToken_ptr acquire(const std::size_t& size);

			/// <summary>
			/// Init all memory pool
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void reset();


			/// <summary>
			/// Return current min requested to bin ratio
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API double minRequestedToBinRatio();

			/// <summary>
			/// Set current min requested to bin ratio
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void minRequestedToBinRatio(double rate);

#pragma endregion

#pragma region Static Functions
			/// <summary>
			/// Create memory pool class
			/// </summary>
			/// <param name="bins"></param>
			/// <returns></returns>
			LLE_NATIVE_API static memoryPool_ptr create(std::initializer_list<std::size_t> bins = { 3145728, 786432, 196608 }, const std::size_t& unit_size = 64, double minRequestedToBinRatio = 0.9);
#pragma endregion


		};
	}
}


#endif