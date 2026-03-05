#include <iostream>
#include <thread>
#include <mutex>

int a = 0;
std::mutex m;
void threadFun(){
    for(int i=0;i<10000000;i++){
        std::unique_lock<std::mutex> mylock(m);
        a++;
        
    }
}

int main(int argc, char const *argv[])
{
    std::thread th1(threadFun);
    std::thread th2(threadFun);

    th1.join();
    th2.join();
    std::cout << a << std::endl;
    std::cout << "over" << std::endl;
    return 0;
}
