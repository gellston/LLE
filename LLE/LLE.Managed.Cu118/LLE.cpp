
#include <lle/image.h>
#include <lle/lle.h>
#include <msclr/marshal_cppstd.h>


#include "LLE.h"


#pragma region Constructor
LLEAPI::V1::LLE::LLE(lleapi::v1::lle_ptr lle) {
	this->instance = new lleapi::v1::lle_ptr(lle);
}
#pragma endregion

#pragma region Destructor
LLEAPI::V1::LLE::~LLE() {
	this->Cleanup(true);
}
#pragma endregion

#pragma region Finalizer
LLEAPI::V1::LLE::!LLE() {
	this->Cleanup(false);
}
#pragma endregion

#pragma region Private Functions
void LLEAPI::V1::LLE::Cleanup(bool disposing) {
	if (this->disposed == true) return;


	//Managed Resource Cleanup
	if (disposing) {

	}

	delete this->instance;
	this->instance = nullptr;

	this->disposed = true;
}
#pragma endregion


#pragma region Public Functions
void LLEAPI::V1::LLE::Setup(System::String^ path, Device device) {
	try {

		auto nativePath = msclr::interop::marshal_as <std::string>(path);
		auto nativeDevice = safe_cast<lleapi::v1::device>(device);
		this->instance->get()->setup(nativePath, nativeDevice);
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

void LLEAPI::V1::LLE::Setup(LLEAPI::V1::DlType dlType, LLEAPI::V1::Device device) {
	try {
		auto nativeType = safe_cast<lleapi::v1::dlType>(dlType);
		auto nativeDevice = safe_cast<lleapi::v1::device>(device);
		this->instance->get()->setup(nativeType, nativeDevice);
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

void LLEAPI::V1::LLE::Shutdown() {
	try {
		this->instance->get()->shutdown();
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

LLEAPI::V1::Image^ LLEAPI::V1::LLE::Predict(System::String^ path) {
	try {
		auto nativePath = msclr::interop::marshal_as<std::string>(path);
		auto nativeImage = this->instance->get()->predict(nativePath);
		auto managedImage = gcnew LLEAPI::V1::Image(nativeImage);
		return managedImage;
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

LLEAPI::V1::Image^ LLEAPI::V1::LLE::Predict(LLEAPI::V1::Image^ image) {
	try {
		auto nativeInputImage = (lleapi::v1::image_ptr*)image->Handle.ToPointer();
		auto nativeOutputImage = this->instance->get()->predict(*nativeInputImage);
		return gcnew LLEAPI::V1::Image(nativeOutputImage);
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}
#pragma endregion

#pragma region Static Functions
LLEAPI::V1::LLE^ LLEAPI::V1::LLE::Create() {
	try {
		auto nativeLLE = lleapi::v1::lle::create();

		auto managedLLE = gcnew LLEAPI::V1::LLE(nativeLLE);
		return managedLLE;
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}

LLEAPI::V1::LLE^ LLEAPI::V1::LLE::Create(MemoryPool^ pool) {
	try {

		auto nativePool = (lleapi::v1::memoryPool_ptr *)pool->Handle.ToPointer();
		auto nativeLLE = lleapi::v1::lle::create(*nativePool);
		auto managedLLE = gcnew LLEAPI::V1::LLE(nativeLLE);
		return managedLLE;
	}
	catch (std::exception ex) {
		throw gcnew System::Exception(msclr::interop::marshal_as<System::String^>(ex.what()));
	}
}
#pragma endregion


