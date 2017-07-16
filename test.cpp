#include "MessageBus.h"
#include <iostream>

MessageBus g_bus;
const string Topic = "Drive";
const string CallBackTopic = "DriveOK";


struct Subject
{
	Subject()
	{
		g_bus.Attach([this]{DriveOk();}, CallBackTopic);
	}

	void SendReq(const string& topic)
	{
		g_bus.SendReq<void, int>(50, topic);
	}

	void DriveOk()
	{
		std::cout << "DriveOk" << std::endl;
	}
};


struct Car
{
	Car()
	{
		g_bus.Attach([this](int speed){Drive(speed);}, Topic);
	}

	void Drive(int speed)
	{
		std::cout << "Car Drive.." << std::endl;
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
		std::cout << "Bus Drive..." << std::endl;
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
		std::cout << "Truck Drive.." << std::endl;
		g_bus.SendReq<void>(CallBackTopic);
	}
};


int main()
{
	Subject subject;
	Car car;
	Bus bus;
	Truck truck;
	subject.SendReq(Topic);
	std::cout << "*****" << std::endl;
	subject.SendReq("");
	return 0;
}
