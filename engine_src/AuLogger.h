#ifndef AU_LOGGER_H
#define AU_LOGGER_H

#include "platform_defs.h"
#include "spdlog/spdlog.h"

#ifdef AU_GLFW_PLATFORM
#include "spdlog/sinks/stdout_color_sinks.h"
#define AU_PLATFORM_LOGGER spdlog::stdout_color_mt
#endif

#ifdef AU_ANDROID_PLATFORM
#include "spdlog/sinks/android_sink.h"
#define AU_PLATFORM_LOGGER spdlog::android_logger_mt
#endif

namespace Audace
{
	class AuLogger
	{
		static std::shared_ptr<AuLogger> engineLogger;
		static std::shared_ptr<AuLogger> rendererLogger;

		std::shared_ptr<spdlog::logger> logger;

	public:
		static void init();
		static void shotdown();
		static std::shared_ptr<AuLogger> getEngineLogger() { return engineLogger; }
		static std::shared_ptr<AuLogger> getRendererLogger() { return rendererLogger; }

		AuLogger(std::string name);
		std::shared_ptr<spdlog::logger> getLogger() { return logger; }
	};
}

#define AU_ENGINE_LOG_CRITICAL(...) Audace::AuLogger::getEngineLogger()->getLogger()->critical(__VA_ARGS__)
#define AU_ENGINE_LOG_ERROR(...) Audace::AuLogger::getEngineLogger()->getLogger()->error(__VA_ARGS__)
#define AU_ENGINE_LOG_WARN(...) Audace::AuLogger::getEngineLogger()->getLogger()->warn(__VA_ARGS__)
#define AU_ENGINE_LOG_INFO(...) Audace::AuLogger::getEngineLogger()->getLogger()->info(__VA_ARGS__)
#define AU_ENGINE_LOG_DEBUG(...) Audace::AuLogger::getEngineLogger()->getLogger()->debug(__VA_ARGS__)
#define AU_ENGINE_LOG_TRACE(...) Audace::AuLogger::getEngineLogger()->getLogger()->trace(__VA_ARGS__)

#endif