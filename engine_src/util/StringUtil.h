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

		static bool startsWith(string s, string comp)
		{
			return s.compare(0, comp.length(), comp) == 0;
		}

		static bool endsWith(string s, string comp)
		{
			return s.length() >= comp.length() &&
				   s.compare(s.length() - comp.length(), comp.length(), comp) == 0;
		}

		static vector<string> splitFilePath(string filepath)
		{
			int i = filepath.find_last_of("/") + 1;
			string s1 = filepath.substr(0, i);
			string s2 = filepath.substr(i);
			return {s1, s2};
		}
	};
}

#endif