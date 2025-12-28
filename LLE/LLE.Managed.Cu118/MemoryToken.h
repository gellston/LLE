#pragma once


//C++
#include <memory>
namespace lleapi {
	namespace v1 {
		class memoryToken;
		using memoryToken_ptr = std::shared_ptr<memoryToken>;
	}
}

//C++/CLI


//C#
namespace LLEAPI {
	namespace V1 {
		ref class MemoryToken {
#pragma region Private Property
		private:
			lleapi::v1::memoryToken_ptr* instance = nullptr;
			bool disposed = false;
#pragma endregion

#pragma region Private Functions
			void Cleanup(bool disposing);
#pragma endregion

#pragma region Constructor
		internal:
			MemoryToken(lleapi::v1::memoryToken_ptr memoryToken);
#pragma endregion

		public:
#pragma region Destructor
			~MemoryToken();
#pragma endregion

#pragma region Finalizer
			!MemoryToken();
#pragma endregion

#pragma region Public Property
			property std::size_t ActualSize {
				std::size_t get();
			}

			property std::size_t Size {
				std::size_t get();
			}

			property System::IntPtr Data {
				System::IntPtr get();
			}
#pragma endregion

		};
	}
}