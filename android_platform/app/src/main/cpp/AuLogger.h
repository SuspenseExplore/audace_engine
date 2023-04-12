//
// Created by Suspense on 4/12/2023.
//

#ifndef AULOGGER_H
#define AULOGGER_H

#include <jni.h>
#include <android/log.h>
#include <memory>
#include <string>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"

namespace Audace {
	class AuLogger {
		static std::shared_ptr<AuLogger> engineLogger;
		std::shared_ptr<spdlog::logger> logger;

	public:
		static void init();
		static void shutdown();
		static std::shared_ptr<AuLogger> getEngineLogger() {return engineLogger;}

		AuLogger(std::string name);
		std::shared_ptr<spdlog::logger> getLogger() {return logger;}
	};
}

#define AU_ENGINE_LOG_CRITICAL(...) Audace::AuLogger::getEngineLogger()->getLogger()->critical(__VA_ARGS__)
#define AU_ENGINE_LOG_ERROR(...) Audace::AuLogger::getEngineLogger()->getLogger()->error(__VA_ARGS__)
#define AU_ENGINE_LOG_WARN(...) Audace::AuLogger::getEngineLogger()->getLogger()->warn(__VA_ARGS__)
#define AU_ENGINE_LOG_INFO(...) Audace::AuLogger::getEngineLogger()->getLogger()->info(__VA_ARGS__)
#define AU_ENGINE_LOG_DEBUG(...) Audace::AuLogger::getEngineLogger()->getLogger()->debug(__VA_ARGS__)
#define AU_ENGINE_LOG_TRACE(...) Audace::AuLogger::getEngineLogger()->getLogger()->trace(__VA_ARGS__)

#endif //AULOGGER_H
