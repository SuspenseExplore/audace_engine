//
// Created by Suspense on 5/28/2023.
//

#ifndef AU_BOOLEANACTIONHANDLER_H
#define AU_BOOLEANACTIONHANDLER_H

#include <string>
#include <vector>
#include <functional>
#include "openxr/openxr_common.h"

namespace Audace {
	class BooleanActionHandler {
		XrAction xrAction;
		bool state_;
		bool changed_;
		long lastChangeTime_;

	public:
		const std::string name;
		const std::string displayName;
		const std::string bindingPath;
		const std::vector<std::string> subPaths;

		BooleanActionHandler(std::string name, std::string displayName, std::string bindingPath, std::vector<std::string> subPaths) :
				name(name), displayName(displayName), bindingPath(bindingPath), subPaths(subPaths) {}

		void update(bool state, bool changed, long lastChangeTime) {
			state_ = state;
			changed_ = changed;
			lastChangeTime_ = lastChangeTime;
		}
		XrAction* getAction() {return &xrAction;}
		bool getState() {return state_;}
		bool hasChanged() {return changed_;}
		long lastChangedTime() {return lastChangeTime_;}
	};
}

#endif //AU_BOOLEANACTIONHANDLER_H
