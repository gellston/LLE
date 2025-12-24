
#include "lle.h"


#pragma region IMPL
namespace lleapi {
	namespace v1 {
		class impl_lle {
		public:

		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::lle::lle() : impl(new impl_lle()){

}
#pragma endregion

#pragma region Destructor
lleapi::v1::lle::~lle() {

}
#pragma endregion


#pragma region Public Functions
void lleapi::v1::lle::setup() {

}

void lleapi::v1::lle::shutdown() {

}
#pragma endregion

