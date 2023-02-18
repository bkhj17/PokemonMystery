#include "Framework.h"

void LogManager::InsertLog(string log)
{
	logs.push_back(log);
	Observer::Get()->ExecuteEvent("ShowLog");
}

void LogManager::GetRenderLog(OUT vector<string>& renderLogs)
{
	//�ڿ������� ���� ���������� ���
	for (int i = max((int)(logs.size()) - MAX_RENDER, 0); i < logs.size(); i++)
		renderLogs.push_back(logs[i]);
}