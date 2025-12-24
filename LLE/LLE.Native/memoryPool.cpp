
#include "memoryPool.h"
#include "memoryBlock.h"


#include <unordered_set>
#include <map>
#include <set>
#include <mutex>

#pragma region IMPL
namespace lleapi {
	namespace v1 {
		class impl_memoryPool {
		public:

			std::set<std::size_t> bins;
			std::map<std::size_t, std::vector<memoryBlock_ptr>> active;
			std::map<std::size_t, std::vector<memoryBlock_ptr>> inactive;
			std::size_t unit_size = 64;

			double efficient_rate = 0.9;

			std::mutex mtx_alloc;
		};
	}
}
#pragma endregion



#pragma region Constructor
lleapi::v1::memoryPool::memoryPool(std::initializer_list<std::size_t> bins , std::size_t unit_size, double efficient_rate) : impl(new impl_memoryPool()) {

	if (unit_size <= 0) {
		throw std::exception("Invalid unit size");
	}

	if (bins.size() <= 0) {
		throw std::exception("Invalid bin size");
	}

	if (efficient_rate <= 0 && efficient_rate > 1.0) {
		throw std::exception("Invalid efficient rate");
	}

	this->impl->unit_size = unit_size;
	this->impl->efficient_rate = efficient_rate;

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
	return size % unitSize == 0 ? size : size + unitSize;
}

lleapi::v1::memoryToken_ptr lleapi::v1::memoryPool::findToken(std::size_t size) {
	try {
		auto aligned_size = this->computeAlignSize(size);
		auto& memory_list = this->impl->inactive[aligned_size];
		if (memory_list.size() == 0) {
			memory_list.push_back(lleapi::v1::memoryBlock::create(aligned_size));
		}

		auto& memory_block = memory_list[0];
		this->impl->active[aligned_size].push_back(memory_block);
		memory_list.erase(memory_list.begin());


		auto actual_size = memory_block->size();
		auto buffer = memory_block->data();

		auto raw_token = new lleapi::v1::memoryToken(actual_size, size, buffer, [this, memory_block, aligned_size] {
			std::scoped_lock lock(this->impl->mtx_alloc);

			this->impl->inactive[aligned_size].push_back(memory_block);
			auto iterator = std::remove_if(this->impl->active[aligned_size].begin(), this->impl->active[aligned_size].end(), [&](const memoryBlock_ptr& element) {
				return element == memory_block;
			});

			this->impl->active[aligned_size].erase(iterator);
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
			return this->findToken(size);
		}


		for (auto& bin : this->impl->bins) {
			//bin의 사이즈가 요청한 aligned된 사이즈보다 클경우 이 bin사이즈로 사용할지를 결정. 쩝... 어렵구먼..
			//만약 사용자가 설정한 효율 비율?보다 높다면 그대로 리턴
			if (bin > aligned_size) {
				double percent = (double)aligned_size / (double)bin;
				if (percent >= 0.9) {
					return this->findToken(bin);
				}
			}
		}


		//만약 효율 비율보다 떨어진다면 그 사이즈를 bin에 등록
		auto memory_block = lleapi::v1::memoryBlock::create(aligned_size);
		this->impl->inactive[aligned_size].push_back(memory_block);
		this->impl->bins.insert(aligned_size);
		return this->findToken(aligned_size);
	}
	catch (...) {
		throw;
	}
}


void lleapi::v1::memoryPool::reset() {
	try {

		std::scoped_lock lock(this->impl->mtx_alloc);

		this->impl->active.clear();
		this->impl->inactive.clear();

		for (auto& bin : this->impl->bins) {
			auto aligned_size = this->computeAlignSize(bin);
			auto memory_block = lleapi::v1::memoryBlock::create(aligned_size);
			this->impl->inactive[aligned_size].push_back(memory_block);
			this->impl->bins.insert(aligned_size);
		}

	}
	catch (...) {
		throw;
	}
}


double lleapi::v1::memoryPool::efficientRate() {
	return this->impl->efficient_rate;
}

void lleapi::v1::memoryPool::efficientRate(double rate) {
	if (rate >= 1.0) this->impl->efficient_rate = 1.0;
	else if (rate <= 0.5) this->impl->efficient_rate = 0.5;

	this->impl->efficient_rate = 0.5;
}
#pragma endregion


#pragma region Static Functions
lleapi::v1::memoryPool_ptr lleapi::v1::memoryPool::create(std::initializer_list<std::size_t> bins, const std::size_t& unit_size, double efficient_rate) {
	try {
		auto raw_pointer = new lleapi::v1::memoryPool(bins, unit_size, efficient_rate);
		return std::shared_ptr<lleapi::v1::memoryPool>(raw_pointer);
	}
	catch (...) {
		throw;
	}
}
#pragma endregion


