#pragma once

#include <Core/Log.h>
#include <Core/Application.h>

extern Greenbox::Application* Greenbox::CreateApplication();

int main()
{
	Greenbox::Log::Init();

	Greenbox::Application* app = Greenbox::CreateApplication();
	app->Run();

	return 0;
}