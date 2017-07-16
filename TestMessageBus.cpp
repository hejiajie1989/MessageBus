#include <iostream>

#include "MessageBus.h"


int main()
{
	MessageBus bus;

	bus.Attach([](int a){
			std::cout << "no reference" << std::endl;
			});

	bus.Attach([](int &a){
			std::cout << "lvalue reference" << std::endl;
			});

	bus.Attach([](int && a){
			std::cout << "rvalue reference" << std::endl;
			});

	bus.Attach([](const int& a){
			std::cout << "const lvalue reference" << std::endl;
			});

	bus.Attach([](int a){
			std::cout << "noreference has return value and key" << std::endl;
			return a;
			}, "a");

	int i = 2;
	bus.SendReq<void, int>(2);
	bus.SendReq<int, int>(2, "a");
	bus.SendReq<void, int&>(i);
	bus.SendReq<void, const int&>(2);
	bus.SendReq<void, int &&>(2);

	std::cout << "*********************" << std::endl;
	return 0;
}
