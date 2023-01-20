#pragma once

namespace Utility {
	string ToString(wstring wstr);
	wstring ToWString(string str);

	wstring ChangeWString(string value);

	vector<string> SplitString(string origin, const string& tok);
}