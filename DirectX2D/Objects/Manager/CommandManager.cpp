#include "Framework.h"

CommandManager::CommandManager()
{
	commandTable["261"] = 12;
	commandTable["4261"] = 11;
	commandTable["624261"] = 13;
	commandTable["661"] = 10;

	for (const auto& command : commandTable) {
		maxCommandLength = max(command.first.size(), maxCommandLength);
	}
}

void CommandManager::Update()
{
	if (KEY_DOWN(VK_RIGHT))
		PushCommand('0'+6);
	if (KEY_DOWN(VK_LEFT))
		PushCommand('0'+4);
	if (KEY_DOWN(VK_UP))
		PushCommand('0'+8);
	if (KEY_DOWN(VK_DOWN))
		PushCommand('0'+2);
	if (KEY_DOWN('A')) {
		PushCommand('0' + 1);
		CheckCommand();
	}
}

void CommandManager::PushCommand(int key)
{
	commandQueue.push(key);
	if (commandQueue.size() > maxCommandLength) {
		commandQueue.pop();
	}
}

void CommandManager::CheckCommand()
{
	string command;

	while (!commandQueue.empty()) {
		command += commandQueue.front();
		commandQueue.pop();
	}

	while (command.size() > 0) {
		if (commandTable.count(command) > 0) {
			Observer::Get()->ExecuteIntParamEvent("Action", commandTable[command]);
			break;
		}

		command.erase(0, 1);
	}

	command = "";
}
