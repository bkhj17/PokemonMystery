#include "framework.h"

string Utility::ToString(wstring wstr)
{
	string tmp;
	tmp.assign(wstr.begin(), wstr.end());
	//wstring은 2바이트이므로 주의

	return tmp;
}

wstring Utility::ToWString(string str)
{
	wstring tmp;
	tmp.assign(str.begin(), str.end());

	return tmp;
}

wstring Utility::ChangeWString(string value)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0, &value[0], value.size(), nullptr, 0);
	wstring strUni(nLen + 1, 0);
	int len = MultiByteToWideChar(CP_ACP, 0, value.c_str(), value.size(), &strUni[0], nLen);
	strUni[len] = 0;

	return strUni;
}

vector<string> Utility::SplitString(string origin, const string& tok)
{
	vector<string> result;
	size_t cutAt = 0;

	while ((cutAt = origin.find_first_of(tok)) != origin.npos) {
		if (cutAt > 0)
			result.push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.size() > 0)
		result.push_back(origin);

	return result;
}
