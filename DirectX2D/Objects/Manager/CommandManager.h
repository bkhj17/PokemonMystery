#pragma once
class CommandManager : public Singleton<CommandManager>
{
private:
	friend class Singleton;
	CommandManager();
	~CommandManager() = default;
public:
	void Update();

	void PushCommand(int key);
	void CheckCommand();
private:
	UINT maxCommandLength = 0;
	queue<int> commandQueue;

	unordered_map<string, int> commandTable;
};

