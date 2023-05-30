//
// Created by Suspense on 5/29/2023.
//

#ifndef AU_RANDOM_H
#define AU_RANDOM_H

#include <random>
using namespace std;

namespace Audace {
	class RandomUniformFloats {
		default_random_engine generator;
		uniform_real_distribution<float> dist;

		RandomUniformFloats(float min, float max) : dist(min, max) {}

	public:
		static RandomUniformFloats normalizedRange() {
			return RandomUniformFloats(0.0f, 1.0f);
		}

		float get() {return dist(generator);}
	};
}

#endif //AU_RANDOM_H
