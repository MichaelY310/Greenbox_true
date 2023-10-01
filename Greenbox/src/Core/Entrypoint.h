#pragma once

#include <iostream>
#include <string>
#include <Core/Log.h>

int main()
{
	Greenbox::Log::Init();

	GB_ASSERT(1 == 2, "fuck you!");

	std::cout << "Hello Greenbox!!" << std::endl;
	return 0;
}