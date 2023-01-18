#include "Framework.h"
#include "Command0118.h"

Command0118::Command0118()
{
	inputs.resize(15);
}

Command0118::~Command0118()
{
}

void Command0118::SetCommand(string sign, vector<UINT>& keys)
{
	if (commands.find(sign) != commands.end())
		return;

	commands[sign] = keys;
}

void Command0118::InputCommand(UINT in)
{
	if (head == (tail + 1) % inputs.size())
		head = (head + 1) % inputs.size();

	inputs[tail] = in;
	tail = (tail + 1) % inputs.size();

	lastCommand = CheckCommand();
}

string Command0118::CheckCommand()
{
	for (auto& command : commands) {
		//애초에 그 만큼 없다
		if (command.second.size() > InputsNum())
			continue;
		
		bool collect = true;
		int t = tail - 1;
		if (t < 0)
			t += inputs.size();

		for (int i = 0; i < command.second.size(); i++) {
			if (command.second[command.second.size() - 1 - i] != inputs[t]) {
				collect = false;
				break;
			}
			if (--t < 0)
				t += inputs.size();
		}

		if (collect)
			return command.first;
	}

	return "";
}

void Command0118::PostRender()
{
	string s;
	for (int i = head; i != tail; i = (i + 1) % inputs.size()) {
		switch (inputs[i]) {
		case VK_RIGHT:	s += ">";	break;
		case VK_LEFT:	s += "<";	break;
		case VK_UP:		s += "^";	break;
		case VK_DOWN:	s += "V";	break;
		default:		s += inputs[i];	break;
		}
	}

	ImGui::Text(s.c_str());

	if(!lastCommand.empty())
		ImGui::Text(lastCommand.c_str());
}

int Command0118::InputsNum()
{
	int t = tail;

	while (head > t)
		t += inputs.size();

	return t - head;
}
