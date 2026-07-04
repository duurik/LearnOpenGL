#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Durik {

	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%Y-%m-%d %T.%e] %v%$");

		s_Logger = spdlog::stdout_color_mt("LearnOpenGL");
		s_Logger->set_level(spdlog::level::trace);
	}
}