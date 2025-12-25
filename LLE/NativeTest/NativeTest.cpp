
#include <lle/memoryPool.h>
#include <lle/image.h>


int main()
{

    //메모리풀 클래스
    auto pool = lleapi::v1::memoryPool::create();
    while (true) {
        {
            //메모리블럭
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
        //메모리풀을 통해 생성한 이미지
        auto image = lleapi::v1::image::create(1024, 1024, 3, pool);
    }
    
}