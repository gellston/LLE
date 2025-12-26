#pragma once

#ifndef LLE_MEMORY_BLOCK
#define LLE_MEMORY_BLOCK

#include <memory>
#include <functional>

#include "lle_api.h"

namespace lleapi {
	namespace v1 {

		class impl_memoryBlock;
		class memoryBlock;
		using memoryBlock_ptr = std::shared_ptr<memoryBlock>;
		class memoryBlock {
		private:

#pragma region Friend Class
			friend class memoryPool;
#pragma endregion


#pragma region Private Property
			std::unique_ptr<impl_memoryBlock> impl;
#pragma endregion


#pragma region Constructor
			/// <summary>
			/// Constructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API memoryBlock(const std::size_t& size);
#pragma endregion
			

		public:
			
#pragma region Destructor
			/// <summary>
			/// Destructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~memoryBlock();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// memory block size
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API std::size_t size();

			/// <summary>
			/// memory block start address 
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint8_t* data();
#pragma endregion

#pragma region Static Functions
			/// <summary>
			/// create memory block
			/// </summary>
			/// <param name="size"></param>
			/// <returns></returns>
			LLE_NATIVE_API static memoryBlock_ptr create(const std::size_t& size);
#pragma endregion




		};
	}
}


#endif