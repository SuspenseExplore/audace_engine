//
// Created by Suspense on 4/12/2023.
//

#include "AuLogger.h"

namespace Audace {
	std::shared_ptr<AuLogger> AuLogger::engineLogger;

	void AuLogger::init() {
		spdlog::set_pattern("%^[%T.%e] [%n] [%l] %v%$");
		engineLogger = std::make_shared<AuLogger>(AuLogger("engine_log"));
	}

	AuLogger::AuLogger(std::string name) {
		logger = spdlog::android_logger_mt(name);
		logger->set_level(spdlog::level::info);
	}
}