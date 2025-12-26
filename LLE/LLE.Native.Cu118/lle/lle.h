#pragma once

#ifndef LLE_INFERENCE
#define LLE_INFERENCE

#include <memory>
#include <string>

#include "lle_api.h"
#include "image.h"

namespace lleapi {
	namespace v1 {

		enum dlType {
			zeroDCE
		};

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
			/// Constructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API lle(memoryPool_ptr pool);

			LLE_NATIVE_API lle();
#pragma endregion
		public:



#pragma region Destructor
			/// <summary>
			/// Destructor
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API ~lle();
#pragma endregion

#pragma region Public Functions
			/// <summary>
			/// Setup internal modoel and instances
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void setup(const std::string & path, device _device);


			LLE_NATIVE_API void setup(dlType delType, device _device);

			/// <summary>
			/// Cleaup internal model and instances
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API void shutdown();



			/// <summary>
			/// Load image file from path and predict
			/// </summary>
			/// <param name="path"></param>
			/// <returns></returns>
			LLE_NATIVE_API image_ptr predict(const std::string & path);



			/// <summary>
			/// Predict from image
			/// </summary>
			/// <param name="path"></param>
			/// <returns></returns>
			LLE_NATIVE_API image_ptr predict(image_ptr image);

#pragma endregion

#pragma region Static Functions
			/// <summary>
			/// Create LLE class
			/// </summary>
			/// <returns></returns>
			LLE_NATIVE_API static lle_ptr create();


			/// <summary>
			/// Create LLE class with memory pool
			/// </summary>
			/// <param name="pool"></param>
			/// <returns></returns>
			LLE_NATIVE_API static lle_ptr create(memoryPool_ptr pool);
#pragma endregion


			
		};
	}
}


#endif