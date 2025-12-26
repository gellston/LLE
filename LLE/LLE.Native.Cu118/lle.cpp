
#include "lle/lle.h"
#include "lle/memoryPool.h"

//Models
#include "models/zerodce.h"

#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>


#pragma region IMPL
namespace lleapi {
    namespace v1 {
        class impl_lle {
        public:
            std::unique_ptr<Ort::Env> env;
            Ort::SessionOptions so;
            std::unique_ptr<Ort::Session> session;
            bool initialized = false;

            std::weak_ptr<lleapi::v1::memoryPool> pool;

            int32_t model_w = 0;
            int32_t model_h = 0;

            impl_lle() {}

            std::mutex mtx_run;
        };
    }
}
#pragma endregion



#pragma region Constructor
lleapi::v1::lle::lle() : impl(new impl_lle()){

}


lleapi::v1::lle::lle(lleapi::v1::memoryPool_ptr pool) : impl(new impl_lle()) {
    this->impl->pool = pool;
}
#pragma endregion

#pragma region Destructor
lleapi::v1::lle::~lle() {
    try {
        this->shutdown();
    }
    catch (...) {

    }
}
#pragma endregion


#pragma region Public Functions
void lleapi::v1::lle::setup(const std::string & path, lleapi::v1::device _device) {

    std::scoped_lock lock(this->impl->mtx_run);

    if (impl->initialized) {
        throw std::runtime_error("lle::setup() called twice. Call shutdown() first.");
    }

    try {
        if (!impl->env) {
            impl->env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "LLE");
        }

        impl->so = Ort::SessionOptions{};
        impl->so.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
        impl->so.SetIntraOpNumThreads(1);
        impl->so.SetInterOpNumThreads(1);


        switch (_device) {
        case device::cpu:

            break;

        case device::cuda: {
            OrtCUDAProviderOptions options{};
            options.device_id = 0;
            options.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchExhaustive;
            options.arena_extend_strategy = 0;
            impl->so.AppendExecutionProvider_CUDA(options);


            break;
        }

        default:
            throw std::runtime_error("Unsupported device");
        }
        std::wstring wpath(path.begin(), path.end());
        impl->session = std::make_unique<Ort::Session>(*impl->env, wpath.c_str(), impl->so);


        {
            auto in_info = impl->session->GetInputTypeInfo(0).GetTensorTypeAndShapeInfo();
            auto shape = in_info.GetShape();
            if (shape.size() != 4) {
                throw std::runtime_error("Model input rank is not 4 (expected BCHW).");
            }

            const int64_t h = shape[2];
            const int64_t w = shape[3];

            if (h <= 0 || w <= 0) {
                throw std::runtime_error("Model input H/W is dynamic (<=0). Fixed input size model required.");
            }

            impl->model_h = static_cast<int32_t>(h);
            impl->model_w = static_cast<int32_t>(w);
        }

        impl->initialized = true;
    }
    catch (const Ort::Exception& e) {
        throw std::runtime_error(std::string("ONNX Runtime error: ") + e.what());
    }
}

void lleapi::v1::lle::setup(lleapi::v1::dlType delType, lleapi::v1::device _device) {

    std::scoped_lock lock(this->impl->mtx_run);

    if (impl->initialized) {
        throw std::runtime_error("lle::setup() called twice. Call shutdown() first.");
    }

    try {
        if (!impl->env) {
            impl->env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "LLE");
        }

        impl->so = Ort::SessionOptions{};
        impl->so.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
        impl->so.SetIntraOpNumThreads(1);
        impl->so.SetInterOpNumThreads(1);


        switch (_device) {
        case device::cpu:

            break;

        case device::cuda: {
            OrtCUDAProviderOptions options{};
            options.device_id = 0;
            options.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchExhaustive;
            options.arena_extend_strategy = 0;
            impl->so.AppendExecutionProvider_CUDA(options);


            break;
        }

        default:
            throw std::runtime_error("Unsupported device");
        }


        switch (delType) {
        case lleapi::v1::dlType::zeroDCE:
            impl->session = std::make_unique<Ort::Session>(*impl->env, zerodce, sizeof(zerodce), impl->so);
            break;
        }
        


        {
            auto in_info = impl->session->GetInputTypeInfo(0).GetTensorTypeAndShapeInfo();
            auto shape = in_info.GetShape();
            if (shape.size() != 4) {
                throw std::runtime_error("Model input rank is not 4 (expected BCHW).");
            }

            const int64_t h = shape[2];
            const int64_t w = shape[3];

            if (h <= 0 || w <= 0) {
                throw std::runtime_error("Model input H/W is dynamic (<=0). Fixed input size model required.");
            }

            impl->model_h = static_cast<int32_t>(h);
            impl->model_w = static_cast<int32_t>(w);
        }

        impl->initialized = true;
    }
    catch (const Ort::Exception& e) {
        throw std::runtime_error(std::string("ONNX Runtime error: ") + e.what());
    }
}

void lleapi::v1::lle::shutdown() {

    std::scoped_lock lock(this->impl->mtx_run);

    if (this->impl->initialized == false) {
        throw std::runtime_error("Its not initialized");
    }

    impl->session.reset();


    impl->initialized = false;
}


lleapi::v1::image_ptr lleapi::v1::lle::predict(lleapi::v1::image_ptr image) {

    std::scoped_lock lock(this->impl->mtx_run);

    try {
        if (!impl || !impl->initialized || !impl->session)
            throw std::runtime_error("lle::predict(): not initialized");

        if (!image)
            throw std::runtime_error("lle::predict(): image is null");

        if (image->channel() != 3)
            throw std::runtime_error("lle::predict(): only 3-channel supported");

        const int32_t modelW = impl->model_w;
        const int32_t modelH = impl->model_h;
        if (modelW <= 0 || modelH <= 0)
            throw std::runtime_error("lle::predict(): model input size not set (setup failed?)");

        const uint32_t W = image->width();
        const uint32_t H = image->height();

        const unsigned char* src = image->data();
        const uint32_t srcStride = image->stride();
        if (!src) throw std::runtime_error("lle::predict(): image->data() is null");

        auto resizeImage = lleapi::v1::image::resize(image, modelW, modelH);


        Ort::AllocatorWithDefaultOptions allocator;
        auto in_name_alloc = impl->session->GetInputNameAllocated(0, allocator);
        auto out_name_alloc = impl->session->GetOutputNameAllocated(0, allocator);
        const char* in_name = in_name_alloc.get();
        const char* out_name = out_name_alloc.get();

        
        std::vector<int64_t> in_shape = { 1, 3, (int64_t)modelH, (int64_t)modelW };
        std::vector<float> in_data((size_t)3 * modelH * modelW);

        const size_t plane = (size_t)modelH * modelW;
        float* p0 = in_data.data() + 0 * plane;
        float* p1 = in_data.data() + 1 * plane;
        float* p2 = in_data.data() + 2 * plane;


        // Tensor 데이터에 맞춰서 노멀라이즈 진행 
        const unsigned char* rsrc = resizeImage->data();
        const size_t rStride = resizeImage->stride();
        for (int y = 0; y < modelH; ++y) {
            const unsigned char* row = rsrc + (size_t)y * rStride;
            for (int x = 0; x < modelW; ++x) {
                const unsigned char* px = row + (size_t)x * 3;
                const size_t idx = (size_t)y * modelW + x;

                p0[idx] = px[0] / 255.0f;
                p1[idx] = px[1] / 255.0f;
                p2[idx] = px[2] / 255.0f;
            }
        }

        Ort::MemoryInfo mem = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            mem, in_data.data(), in_data.size(), in_shape.data(), in_shape.size()
        );


        const char* input_names[] = { in_name };
        const char* output_names[] = { out_name };
        auto outputs = impl->session->Run(
            Ort::RunOptions{ nullptr },
            input_names, &input_tensor, 1,
            output_names, 1
        );

        if (outputs.empty() || !outputs[0].IsTensor())
            throw std::runtime_error("lle::predict(): output is not a tensor");

        const float* outp = outputs[0].GetTensorData<float>();
        if (!outp) throw std::runtime_error("lle::predict(): output tensor data is null");


        const size_t out_elems = (size_t)3 * modelH * modelW;
        float vmin = std::numeric_limits<float>::infinity();
        float vmax = -std::numeric_limits<float>::infinity();
        for (size_t i = 0; i < out_elems; ++i) {
            float v = outp[i];
            if (v < vmin) vmin = v;
            if (v > vmax) vmax = v;
        }
        const float denom = (vmax > vmin) ? (vmax - vmin) : 1.0f;

        auto clampU8 = [](float v) -> unsigned char {
            if (v < 0.f) v = 0.f;
            if (v > 255.f) v = 255.f;
            return (unsigned char)(v + 0.5f);
            };


        auto out_small = this->impl->pool.expired()
            ? lleapi::v1::image::create(modelW, modelH, 3)
            : lleapi::v1::image::create(modelW, modelH, 3, this->impl->pool.lock());

        unsigned char* os = out_small->data();
        const uint32_t osStride = out_small->stride();
        if (!os) throw std::runtime_error("lle::predict(): out_small->data() is null");

        const float* q0 = outp + 0 * plane;
        const float* q1 = outp + 1 * plane;
        const float* q2 = outp + 2 * plane;

        for (int y = 0; y < modelH; ++y) {
            unsigned char* row = os + (size_t)y * osStride;
            for (int x = 0; x < modelW; ++x) {
                const size_t idx = (size_t)y * modelW + x;
                unsigned char* px = row + (size_t)x * 3;

                px[0] = clampU8((q0[idx] - vmin) / denom * 255.0f);
                px[1] = clampU8((q1[idx] - vmin) / denom * 255.0f);
                px[2] = clampU8((q2[idx] - vmin) / denom * 255.0f);
            }
        }


        auto out_img = lleapi::v1::image::resize(out_small, (uint32_t)W, (uint32_t)H);
        
        cv::waitKey();
        return out_img;
    }
    catch (const Ort::Exception& e) {
        throw std::runtime_error(std::string("ONNX Runtime error: ") + e.what());
    }
}

lleapi::v1::image_ptr lleapi::v1::lle::predict(const std::string & path) {

    try {

        

        auto image = this->impl->pool.expired() == true ?
            lleapi::v1::image::imread(path, colorType::color) :
            lleapi::v1::image::imread(path, colorType::color, this->impl->pool.lock());

        return this->predict(image);

    }
    catch (...) {
        throw;
    }
}
#pragma endregion


#pragma region Static Functions

lleapi::v1::lle_ptr lleapi::v1::lle::create(memoryPool_ptr pool) {
    try {
        return std::shared_ptr<lleapi::v1::lle>(new lle(pool));
    }
    catch (...) {
        throw;
    }
}


lleapi::v1::lle_ptr lleapi::v1::lle::create() {
    try {
        return std::shared_ptr<lleapi::v1::lle>(new lle());
    }
    catch (...) {
        throw;
    }
}

#pragma endregion


