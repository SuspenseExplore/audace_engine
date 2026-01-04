#include "AuLogger.h"

namespace Audace
{
	std::shared_ptr<AuLogger> AuLogger::engineLogger = nullptr;
	std::shared_ptr<AuLogger> AuLogger::rendererLogger = nullptr;
	std::shared_ptr<AuLogger> AuLogger::xrLogger = nullptr;

	void AuLogger::init()
	{
		spdlog::set_pattern("%^[%T.%e] [%n] [%l] %v%$");
		spdlog::set_level(spdlog::level::debug);
		if (engineLogger == nullptr)
		{
			engineLogger = std::make_shared<AuLogger>(AuLogger("engine_log"));
		}
		if (rendererLogger == nullptr)
		{
			rendererLogger = std::make_shared<AuLogger>(AuLogger("renderer_log"));
		}
		if (xrLogger == nullptr)
		{
			xrLogger = std::make_shared<AuLogger>(AuLogger("xr_log"));
		}
	}

	AuLogger::AuLogger(std::string name)
	{
		logger = AU_PLATFORM_LOGGER(name);
		logger->set_level(spdlog::level::debug);
	}
}