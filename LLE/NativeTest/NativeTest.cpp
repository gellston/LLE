#include <lle/memoryPool.h>
#include <lle/image.h>
#include <lle/lle.h>

#include <iostream>

int main()
{

    try {
        // create lle instance
        auto lle = lleapi::v1::lle::create();

        // load zerodce model and load on cpu
        // its also support onnx model load from path
        lle->setup(lleapi::v1::dlType::zeroDCE, lleapi::v1::device::cpu);

        // load color image
        auto input = lleapi::v1::image::imread("C://github//dataset//lol_dataset//our485//low//low_15.png", lleapi::v1::colorType::color);

        // predict 
        auto output = lle->predict(input);

        // save image file on disk
        lleapi::v1::image::imwrite("C://github//LLE//LLE//x64//Debug//result1.jpg", output);


        // cleanup internal instance
        lle->shutdown();

    }
    catch (std::exception ex) {

        std::cout << ex.what() << std::endl;
    }
}