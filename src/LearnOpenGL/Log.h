#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Durik 
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

// Logging macros
#define DV_TRACE(...)	::Durik::Log::GetLogger()->trace(__VA_ARGS__)
#define DV_INFO(...)	::Durik::Log::GetLogger()->info(__VA_ARGS__)
#define DV_WARN(...)	::Durik::Log::GetLogger()->warn(__VA_ARGS__)
#define DV_ERROR(...)	::Durik::Log::GetLogger()->error(__VA_ARGS__)
#define DV_CRIT(...)	::Durik::Log::GetLogger()->critical(__VA_ARGS__)