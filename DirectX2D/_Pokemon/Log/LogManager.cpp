#include "Framework.h"

void LogManager::InsertLog(string log)
{
	logs.push_back(log);
	Observer::Get()->ExecuteEvent("ShowLog");
}

void LogManager::GetRenderLog(OUT vector<string>& renderLogs)
{
	//뒤에서부터 일정 개수까지만 출력
	for (int i = max((int)(logs.size()) - MAX_RENDER, 0); i < logs.size(); i++)
		renderLogs.push_back(logs[i]);
}