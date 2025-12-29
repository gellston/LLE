#pragma once

#include <memory>


#include "Image.h"
#include "MemoryPool.h"

namespace lleapi {
	namespace v1 {
		class lle;
		using lle_ptr = std::shared_ptr<lle>;
	}
}


namespace LLEAPI {
	namespace V1 {
		enum class DlType {
			ZeroDCE
		};

		enum class Device {
			Cpu,
			Cuda
		};

		ref class LLE {
		private:
#pragma region Private Property
			bool disposed = false;
			lleapi::v1::lle_ptr* instance = nullptr;
#pragma endregion

		internal:
#pragma region Constructor
			LLE(lleapi::v1::lle_ptr lle);
#pragma endregion

		public:
#pragma region Destructor
			~LLE();
#pragma endregion

#pragma region Finalizer
			!LLE();
#pragma endregion

#pragma region Private Functions
			void Cleanup(bool disposing);
#pragma endregion


#pragma region Public Functions
			void Setup(System::String^ path, Device device);
			void Setup(DlType dlType, Device device);
			void Shutdown();
			Image^ Predict(System::String^ path);
			Image^ Predict(Image^ image);
#pragma endregion

#pragma region Static Functions
			static LLE^ Create();
			static LLE^ Create(MemoryPool^ pool);
#pragma endregion




		};
	}
}