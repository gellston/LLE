#pragma once


//C++
#include <memory>
namespace lleapi {
	namespace v1 {
		class memoryPool;
		using memoryPool_ptr = std::shared_ptr<memoryPool>;
	}
}
#include <lle/memoryPool.h>


//C++/CLI
#include "MemoryToken.h"

//C#

namespace LLEAPI {
	namespace V1 {
		ref class MemoryPool {
		private:
#pragma region Private Property
			lleapi::v1::memoryPool_ptr* instance = nullptr;
			bool disposed = false;
#pragma endregion

#pragma region Private Functions
			void Cleanup(bool disposing);
#pragma endregion


		internal:
#pragma region Constructor
			MemoryPool(lleapi::v1::memoryPool_ptr memoryPool);
#pragma endregion

		public:
#pragma region Destructor
			~MemoryPool();
#pragma endregion

#pragma region Finalizer
			!MemoryPool();
#pragma endregion


#pragma region Public Property
			property double MinRequestedToBinRatio {
				double get();
				void set(double value);
			}
#pragma endregion

#pragma region Public Functions
			MemoryToken^ Acquire(std::size_t size);

			void Reset();
#pragma endregion



		};
	}
}