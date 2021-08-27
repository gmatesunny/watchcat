#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "timer.hpp"

void quick(bool &done)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << std::this_thread::get_id() << ": quick() executed" << std::endl;
    done = true;
}

int slow(bool &done)
{
    for (size_t i = 0; i < 10; ++i)
    {
        std::cout << std::this_thread::get_id() << ": slow() executing" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));        
    }
    std::cout << std::this_thread::get_id() << ": slow() executed" << std::endl;
    done = true;
    return 0;
}

int main()
{
    std::cout << std::this_thread::get_id() << ": Main thread" << std::endl;

    auto timer  = std::make_shared<watchcat::Timer>();

    bool quickDone = false;
    unsigned int id1 = timer->RegisterEvent([&quickDone](unsigned int)
                                            {
                                                if (quickDone == false)
                                                {
                                                    std::cout << std::this_thread::get_id() << ": quick() not executed, will not observe" << std::endl;
                                                }
                                            },
                                            std::chrono::seconds(2));
    timer->ActivateEvent(id1);    
    quick(quickDone); // calling the quickFunc on main thread

    bool slowDone = false;
    unsigned int id2 = timer->RegisterEvent([&](unsigned int)
                                            {
                                                if (slowDone == false)
                                                {
                                                    std::cout << std::this_thread::get_id() << ": slow() is not yet completed, continue observing" << std::endl;
                                                }
                                                else
                                                {
                                                    std::cout << std::this_thread::get_id() << ": slow() is completed, deactivating myself" << std::endl;
                                                    timer->DeactiveEvent(id2);
                                                }
                                            },
                                            std::chrono::seconds(2), true);
    timer->ActivateEvent(id2);
    std::thread th(slow, std::ref(slowDone)); //calling on different thread

    std::this_thread::sleep_for(std::chrono::seconds(5));

    th.join();
    return 0;
}
