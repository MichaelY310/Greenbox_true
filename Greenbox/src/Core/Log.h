#pragma once

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Greenbox {

	class Log
	{
	public: 
		static void Init() {
			spdlog::set_pattern("%^[%T] %n: %v%$");
			Log::s_Logger = spdlog::stdout_color_mt("GreenBox");
			Log::s_Logger->set_level(spdlog::level::trace);
		}

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}


#define GB_INFO(...)		::Greenbox::Log::GetCoreLogger()->info(__VA_ARGS__);
#define GB_WARN(...)		::Greenbox::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define GB_ERROR(...)		::Greenbox::Log::GetCoreLogger()->error(__VA_ARGS__);
#define GB_TRACE(...)		::Greenbox::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define GB_ASSERT(x, ...)	{ if(!(x)) { GB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }