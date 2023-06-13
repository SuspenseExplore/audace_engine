#ifndef AU_STRINGUTIL_H
#define AU_STRINGUTIL_H

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace Audace
{
	class StringUtil
	{
	public:
		static vector<string> split(string s, char delim)
		{
			stringstream ss(s);
			vector<string> v;
			string token;
			while (getline(ss, token, delim))
			{
				v.push_back(token);
			}

			return v;
		}

		static bool startsWith(string s, string comp) {
			return !s.substr(0, comp.length()).compare(comp);
		}
	};
}

#endif