// NativeTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//


#include <iostream>

#include <lle/memoryPool.h>
#include <lle/image.h>


int main()
{

    auto pool = lleapi::v1::memoryPool::create();
    while (true) {

        {
            auto block = pool->acquire(3145728);
            {
                auto block = pool->acquire(3145728);
                {
                    auto block = pool->acquire(3145728);
                    {
                        auto block = pool->acquire(3145728);
                    }
                }
            }
        }
        auto image = lleapi::v1::image::create(1024, 1024, 3, pool);
    }
    
}

