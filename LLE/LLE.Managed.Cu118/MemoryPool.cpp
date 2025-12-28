

//C++
#include <msclr/marshal_cppstd.h>

//C++/CLI
#include "MemoryPool.h"


//C#



#pragma region Constructor
LLEAPI::V1::MemoryPool::MemoryPool(lleapi::v1::memoryPool_ptr memoryPool) {
	this->instance = new lleapi::v1::memoryPool_ptr(memoryPool);
}
#pragma endregion

#pragma region Destructor
LLEAPI::V1::MemoryPool::~MemoryPool() {
	this->Cleanup(true);
}
#pragma endregion

#pragma region Finalizer
LLEAPI::V1::MemoryPool::!MemoryPool() {
	this->Cleanup(false);
}
#pragma endregion


#pragma region Private Functions
void LLEAPI::V1::MemoryPool::Cleanup(bool disposing) {
	if (this->disposed == true) return;


	//Managed Resource Cleanup
	if (disposing) {

	}

	delete this->instance;
	this->instance = nullptr;

	this->disposed = true;
}
#pragma endregion


#pragma region Public Property
double LLEAPI::V1::MemoryPool::MinRequestedToBinRatio::get() {
	return this->instance->get()->minRequestedToBinRatio();
}

void LLEAPI::V1::MemoryPool::MinRequestedToBinRatio::set(double value) {
	this->instance->get()->minRequestedToBinRatio(value);
}
#pragma endregion


#pragma region Public Functions
LLEAPI::V1::MemoryToken^ LLEAPI::V1::MemoryPool::Acquire(std::size_t size) {
	try {
		auto nativeMemoryToken = this->instance->get()->acquire(size);
		return gcnew LLEAPI::V1::MemoryToken(nativeMemoryToken);
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

void LLEAPI::V1::MemoryPool::Reset() {

	try {
		this->instance->get()->reset();
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}
#pragma endregion



