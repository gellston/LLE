#pragma once

#ifndef LLE_MEMORY_TOKEN
#define LLE_MEMORY_TOKEN

#include <memory>
#include <functional>

#include "lle_api.h"

namespace lleapi {
	namespace v1 {

		class impl_memoryToken;
		class memoryToken;
		using memoryToken_ptr = std::shared_ptr<memoryToken>;
		class memoryToken {
		private:

#pragma region Friend Class
			friend class memoryPool;
			friend class image;
#pragma endregion

#pragma region Private Property
			std::unique_ptr<impl_memoryToken> impl;
#pragma endregion

#pragma region Constructor
			/// <summary>
			/// Constructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API memoryToken(const std::size_t & actual_size, const std::size_t & size, uint8_t* buffer);

			LLE_NATIVE_API memoryToken(const std::size_t & actual_size, const std::size_t & size, uint8_t* buffer, const std::function<void()> & dealloc_callback);
#pragma endregion

		public:


#pragma region Destructor
			/// <summary>
			/// Destructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~memoryToken();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// memory block actual size
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API std::size_t actualSize();

			/// <summary>
			/// memory block¿¡ requested size
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API std::size_t size();

			/// <summary>
			/// memory block start address
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API uint8_t* data();
#pragma endregion



		};
	}
}


#endif