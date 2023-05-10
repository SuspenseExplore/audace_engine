#include "AuLogger.h"

namespace Audace {
	std::shared_ptr<AuLogger> AuLogger::engineLogger;

	void AuLogger::init() {
		spdlog::set_pattern("%^[%T.%e] [%n] [%l] %v%$");
		spdlog::set_level(spdlog::level::info);
		engineLogger = std::make_shared<AuLogger>(AuLogger("engine_log"));
	}

	AuLogger::AuLogger(std::string name) {
		logger = AU_PLATFORM_LOGGER(name);
		logger->set_level(spdlog::level::info);
	}
}