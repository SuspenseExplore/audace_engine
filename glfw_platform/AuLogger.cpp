#include "AuLogger.h"

namespace Audace {
	std::shared_ptr<AuLogger> AuLogger::engineLogger;

	void AuLogger::init() {
		spdlog::set_pattern("%^[%T.%e] [%n] [%l] %v%$");
		spdlog::set_level(spdlog::level::trace);
		engineLogger = std::make_shared<AuLogger>(AuLogger("engine_log"));
	}

	AuLogger::AuLogger(std::string name) {
		logger = spdlog::stdout_color_mt(name);
		logger->set_level(spdlog::level::trace);
	}
}