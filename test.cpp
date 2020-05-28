#include "MessageBus.h"
#include <iostream>
#include <sstream>
#include "function_traits.h"

MessageBus g_bus;
const string Topic = "Drive";
const string EatTopic = "Eating";
const string CallBackTopic = "DriveOK";

//不带回复的主题
struct Subject
{
    Subject()
    {
    }

    void SendReq(const string& topic, int speed)
    {
        g_bus.SendReq<void, int>(std::move(speed), topic);
    }

    void AsyncSendReq(const string& topic, int speed)
    {
        g_bus.AsyncSendReq<void, int>(std::move(speed), topic);
    }
};


//带回复的主题，需要两条件：
//1、主题要注册带回复的topic
//2、业务类需要在topic回调处理函数中，处理完成后，向消息总线发布消息
struct ReplySubject
{
    ReplySubject()
    {   
        //使用类成员函数,向消息总线注册带回复的主题
        auto fun = std::bind(&ReplySubject::DriveOk, this);
        g_bus.Attach<std::function<void(void)>>(fun, CallBackTopic);
 
    }

    void SendReq(const string& topic, int speed)
    {
        g_bus.SendReq<void, int>(std::move(speed), topic);
    }

    void AsyncSendReq(const string& topic, int speed)
    {
        g_bus.AsyncSendReq<void, int>(std::move(speed), topic);
    }

    void DriveOk()
    {
        std::stringstream s;
        s << "[thread_id:" << std::this_thread::get_id() << "] " << "DriveOk";
        std::cout << s.str() << std::endl;
    }

    ~ReplySubject()
    {
        g_bus.Remove<void,void>(CallBackTopic);
    }
};


struct Car
{
    Car()
    {
        //g_bus.Attach([this](int speed){Drive(speed);}, Topic);
        auto fun = std::bind(&Car::Drive, this,std::placeholders::_1);
        g_bus.Attach<std::function<void(int)>>(fun, Topic);
    }

    void Drive(int speed)
    {
        std::stringstream s;
        s<< "[thread_id:" << std::this_thread::get_id() << "] " << "Car Drive.."<<speed;
        std::cout << s.str() << std::endl;
        g_bus.SendReq<void>(CallBackTopic);
    }
};

struct Bus
{
    Bus()
    {
        g_bus.Attach([this](int speed){Drive(speed);}, Topic);
    }

    void Drive(int speed)
    {
        std::stringstream s;
        s << "[thread_id:" << std::this_thread::get_id() << "] " << "Bus Drive.." << speed;
        std::cout << s.str() << std::endl;
        g_bus.SendReq<void>(CallBackTopic);
    }
};


struct Truck
{
    Truck()
    {
        g_bus.Attach([this](int speed){Drive(speed);});
    }

    void Drive(int speed)
    {
        std::stringstream s;
        s << "[thread_id:" << std::this_thread::get_id() << "] " << "Truck Drive.." << speed;
        std::cout << s.str() << std::endl;
        g_bus.SendReq<void>(CallBackTopic);
    }
};

int main()
{
    Car car;
    Bus bus;
    Truck truck;
    int count = 1;
    //带回复的主题
    {
        ReplySubject subject;
       
        std::cout << "*****reply subject exam test*******" << std::endl;
        std::cout << "*****topic message send*******" << std::endl;
        subject.SendReq(Topic,100);
        std::cout << "*****empty topic message send*******" << std::endl;
        subject.SendReq("",80);
        std::cout <<" ***topic message async send*** "<< std::endl;
        while (count-- > 0)
        {
            subject.AsyncSendReq(Topic,60);
        }
    }
    //不带回复的主题
    {
        std::cout << "*****no reply subject exam test*******" << std::endl;
        Subject noReplyS;
        noReplyS.SendReq(Topic,90);
        //noReplyS.SendReq("");
        //noReplyS.AsyncSendReq(Topic);
        count = 2;
        while (count-- > 0)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    g_bus.uninit();
    std::cout << "*****exit*******" << std::endl;
    return 0;
}
