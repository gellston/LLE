
#include <lle/memoryPool.h>
#include <lle/image.h>
#include <lle/lle.h>

#include <iostream>


int main()
{

    try {
        auto pool = lleapi::v1::memoryPool::create();

        auto lle = lleapi::v1::lle::create(pool);
        lle->setup("C://github//LLE//python//results//model.onnx", lleapi::v1::device::cpu);

        auto input = lleapi::v1::image::imread("C://github//dataset//lol_dataset//our485//low//low_15.png", lleapi::v1::colorType::color, pool);
        auto output1 = lle->predict(input);
        lleapi::v1::image::imwrite("C://github//LLE//LLE//x64//Debug//result1.jpg", output1);

        auto output2 = lle->predict("C://github//dataset//lol_dataset//our485//low//low_15.png");
        lleapi::v1::image::imwrite("C://github//LLE//LLE//x64//Debug//result2.jpg", output1);

    }
    catch (std::exception ex) {

        std::cout << ex.what() << std::endl;
    }


    ////메모리풀 클래스
    //auto pool = lleapi::v1::memoryPool::create();
    //while (true) {
    //    {
    //        //메모리블럭
    //        auto block = pool->acquire(3145728);
    //        {
    //            auto block = pool->acquire(3145728);
    //            {
    //                auto block = pool->acquire(3145728);
    //                {
    //                    auto block = pool->acquire(3145728);
    //                }
    //            }
    //        }
    //    }
    //    //메모리풀을 통해 생성한 이미지
    //    auto image = lleapi::v1::image::create(1024, 1024, 3, pool);
    //}



    
}