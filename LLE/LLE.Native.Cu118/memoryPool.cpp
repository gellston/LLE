
#include "lle/memoryPool.h"
#include "lle/memoryBlock.h"


#include <map>
#include <set>
#include <mutex>
#include <algorithm>
#include <stdexcept>

#pragma region IMPL
namespace lleapi {
	namespace v1 {
		class impl_memoryPool {
		public:

			std::set<std::size_t> bins;
			std::map<std::size_t, std::vector<memoryBlock_ptr>> active;
			std::map<std::size_t, std::vector<memoryBlock_ptr>> inactive;
			std::size_t unit_size = 64;

			double minRequestedToBinRatio = 0.9;

			std::mutex mtx_alloc;
		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::memoryPool::memoryPool(std::initializer_list<std::size_t> bins , std::size_t unit_size, double minRequestedToBinRatio) : impl(new impl_memoryPool()) {

	if (unit_size == 0) {
		throw std::runtime_error("Invalid unit size");
	}

	if (bins.size() == 0) {
		throw std::runtime_error("Invalid bin size");
	}

	if (minRequestedToBinRatio < 0.5 || minRequestedToBinRatio > 1.0) {
		throw std::runtime_error("Invalid efficient rate");
	}

	this->impl->unit_size = unit_size;
	this->impl->minRequestedToBinRatio = minRequestedToBinRatio;

	for (auto& bin : bins) {
		auto aligned_size = this->computeAlignSize(bin);
		auto memory_block = lleapi::v1::memoryBlock::create(aligned_size);
		this->impl->inactive[aligned_size].push_back(memory_block);
		this->impl->bins.insert(aligned_size);
	}
}
#pragma endregion

#pragma region Destructor
lleapi::v1::memoryPool::~memoryPool() {

}
#pragma endregion

#pragma region Private Functions

std::size_t lleapi::v1::memoryPool::computeAlignSize(std::size_t size) {
	auto& unitSize = this->impl->unit_size;
	return (size % unitSize == 0) ? size : (size + (unitSize - (size % unitSize)));
}

lleapi::v1::memoryToken_ptr lleapi::v1::memoryPool::findToken(std::size_t requested_size, std::size_t aligned_size) {
	try {
		auto& memory_list = this->impl->inactive[aligned_size];
		if (memory_list.size() == 0) {
			memory_list.push_back(lleapi::v1::memoryBlock::create(aligned_size));
		}

		auto memory_block = memory_list.back();
		memory_list.pop_back();                  
		this->impl->active[aligned_size].push_back(memory_block);


		auto actual_size = memory_block->size();
		auto buffer = memory_block->data();

		auto weak_impl = std::weak_ptr<impl_memoryPool>(this->impl);
		auto raw_token = new lleapi::v1::memoryToken(actual_size, requested_size, buffer, [weak_impl, memory_block, aligned_size] {

			if (auto impl = weak_impl.lock()) {
				std::scoped_lock lock(impl->mtx_alloc);
				impl->inactive[aligned_size].push_back(memory_block);
				auto& vec = impl->active[aligned_size];
				vec.erase(std::remove(vec.begin(), vec.end(), memory_block), vec.end());
			}
		});

		auto token = std::shared_ptr<lleapi::v1::memoryToken>(raw_token);
		return token;
	}
	catch (...) {
		throw;
	}
}
#pragma endregion



#pragma region Public Functions
lleapi::v1::memoryToken_ptr lleapi::v1::memoryPool::acquire(const std::size_t & size) {
	try {

		std::scoped_lock lock(this->impl->mtx_alloc);

		auto aligned_size = this->computeAlignSize(size);

		//aligned된 사이즈가 정확하게 bin에 있는경우 존재할 경우
		if (this->impl->bins.count(aligned_size) > 0) {
			return this->findToken(size, aligned_size);
		}


		for (auto& bin : this->impl->bins) {
			//bin의 사이즈가 요청한 aligned된 사이즈보다 클경우 이 bin사이즈로 사용할지를 결정. 쩝... 어렵구먼..
			//만약 사용자가 설정한 효율 비율?보다 높다면 그대로 리턴
			if (bin > aligned_size) {
				double percent = (double)aligned_size / (double)bin;
				if (percent >= this->impl->minRequestedToBinRatio) {
					return this->findToken(size, bin);
				}
			}
		}


		//만약 효율 비율보다 떨어진다면 그 사이즈를 bin에 등록
		auto memory_block = lleapi::v1::memoryBlock::create(aligned_size);
		this->impl->inactive[aligned_size].push_back(memory_block);
		this->impl->bins.insert(aligned_size);
		return this->findToken(size, aligned_size);
	}
	catch (...) {
		throw;
	}
}


void lleapi::v1::memoryPool::reset() {
	try {

		std::scoped_lock lock(this->impl->mtx_alloc);

		for (auto& keypair : this->impl->active) {
			if (!keypair.second.empty()) {
				throw std::runtime_error("Need to return memory token");
			}
		}

		this->impl->inactive.clear();

		for (auto& bin : this->impl->bins) {
			auto aligned_size = this->computeAlignSize(bin);
			auto memory_block = lleapi::v1::memoryBlock::create(aligned_size);
			this->impl->inactive[aligned_size].push_back(memory_block);
		}

	}
	catch (...) {
		throw;
	}
}


double lleapi::v1::memoryPool::minRequestedToBinRatio() {
	return this->impl->minRequestedToBinRatio;
}

void lleapi::v1::memoryPool::minRequestedToBinRatio(double rate) {

	this->impl->minRequestedToBinRatio = std::clamp(rate, 0.5, 1.0);;
}
#pragma endregion


#pragma region Static Functions
lleapi::v1::memoryPool_ptr lleapi::v1::memoryPool::create(std::initializer_list<std::size_t> bins, const std::size_t& unit_size, double minRequestedToBinRatio) {
	try {
		auto raw_pointer = new lleapi::v1::memoryPool(bins, unit_size, minRequestedToBinRatio);
		return std::shared_ptr<lleapi::v1::memoryPool>(raw_pointer);
	}
	catch (...) {
		throw;
	}
}
#pragma endregion


