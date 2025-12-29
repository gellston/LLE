#pragma once


//C++
#include <memory>
namespace lleapi {
	namespace v1 {
		class image;
		using image_ptr = std::shared_ptr<image>;
	}
}

//C++/CLI
#include "MemoryPool.h"


//C#
namespace LLEAPI {
	namespace V1 {

		enum class ColorType {
			Gray,
			Color
		};

		ref class Image {
#pragma region Private Property
		private:
			lleapi::v1::image_ptr* instance = nullptr;
			bool disposed = false;
#pragma endregion

#pragma region Private Functions
			void Cleanup(bool disposing);
#pragma endregion

#pragma region Constructor
		internal:
			Image(lleapi::v1::image_ptr image);
#pragma endregion

		public:
#pragma region Destructor
			~Image();
#pragma endregion

#pragma region Finalizer
			!Image();
#pragma endregion

#pragma region Public Property
			property uint32_t Width {
				uint32_t get();
			}

			property uint32_t Height {
				uint32_t get();
			}

			property uint32_t Channel {
				uint32_t get();
			}

			property std::size_t Size {
				std::size_t get();
			}

			property uint32_t Stride {
				uint32_t get();
			}

			property System::IntPtr Data {
				System::IntPtr  get();
			}

			property System::IntPtr Handle {
				System::IntPtr get();
			}
#pragma endregion

#pragma region Static Functions
			static Image^ Create(uint32_t width, uint32_t height, uint32_t channel);
			static Image^ Create(uint32_t width, uint32_t height, uint32_t channel, MemoryPool^ pool);
			static Image^ Resize(Image^ image, uint32_t width, uint32_t height);
			static Image^ Imread(System::String^ path, ColorType colorType);
			static Image^ Imread(System::String^ path, ColorType colorType, MemoryPool^ pool);
			static void Imwrite(System::String^ path, Image^ image);
#pragma endregion

		};
	}
}