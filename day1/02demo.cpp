#include<iostream>
#include<thread>

class Log{

public:
    Log(){};
    Log(const Log& log) = delete;
    Log& operator=(const Log& log) = delete;

    static Log& GetInstance(){
        static Log log;
        return log;
    }

    void PrintInfo(std::string msg){
        std::cout<<__TIME__<<" "<<msg<<std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Log::GetInstance().PrintInfo("error");
    
    return 0;
}
