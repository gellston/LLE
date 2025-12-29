

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

System::IntPtr LLEAPI::V1::MemoryPool::Handle::get() {
	return System::IntPtr(this->instance);
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


#pragma region Static Functoins
LLEAPI::V1::MemoryPool^ LLEAPI::V1::MemoryPool::Create() {
	try {

		return  LLEAPI::V1::MemoryPool::Create(gcnew array<std::size_t>{ 3145728, 786432, 196608 }, 64, 0.9);

	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

LLEAPI::V1::MemoryPool^ LLEAPI::V1::MemoryPool::Create(array<std::size_t>^ bins, std::size_t unitSize, double minRequestedToBinRatio) {
	try {

		if (bins == nullptr)
			bins = gcnew array<std::size_t>{ 3145728, 786432, 196608 };

		std::vector<size_t> v;
		v.reserve(bins->Length);
		for each (auto x in bins)
		{
			if (x <= 0) throw gcnew System::ArgumentOutOfRangeException("bins");
			v.push_back(static_cast<size_t>(x));
		}

		auto nativePool = lleapi::v1::memoryPool::create(v, unitSize, minRequestedToBinRatio);

		return gcnew LLEAPI::V1::MemoryPool(nativePool);
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}
#pragma endregion




