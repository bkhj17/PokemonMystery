#pragma once
class LogManager : public Singleton<LogManager>
{
private:
	friend class Singleton;
	LogManager() = default;
	~LogManager() = default;

	const UINT MAX_RENDER = 3;
public:
	void InsertLog(string log);

	void GetRenderLog(OUT vector<string>& renderLogs);
private:
	vector<string> logs;
};