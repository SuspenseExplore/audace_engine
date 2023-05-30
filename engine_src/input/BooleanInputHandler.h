//
// Created by Suspense on 5/28/2023.
//

#ifndef AU_BOOLEANACTIONHANDLER_H
#define AU_BOOLEANACTIONHANDLER_H

#include <string>
#include <vector>
#include <functional>
#include "openxr/openxr_common.h"
#include "BooleanInputEvent.h"

namespace Audace {
	class BooleanInputHandler {
		XrAction xrAction;

	public:
		const std::string name;
		const std::string displayName;
		const std::string bindingPath;
		const std::vector<std::string> subPaths;
		const std::function<void(BooleanInputEvent)> callback;

		BooleanInputHandler(std::string name,
							std::string displayName,
							std::string bindingPath,
							std::vector<std::string> subPaths,
							std::function<void(BooleanInputEvent)> callback) :
				name(name), displayName(displayName), bindingPath(bindingPath), subPaths(subPaths),
				callback(callback) {}

		XrAction getAction() { return xrAction; }
		void setAction(XrAction action) {xrAction = action;}
	};
}

#endif //AU_BOOLEANACTIONHANDLER_H
