#pragma once
class Command0118 : public Singleton<Command0118>
{
	//커맨드 입력장치
	//입력에 따른 커맨드에 따라 신호를 반환

private:
	friend class Singleton;
	Command0118();
	~Command0118();
public:

	void SetCommand(string sign, vector<UINT>& keys);
	
	void InputCommand(UINT in);

	void PostRender();

	string LastCommand() { return lastCommand; }
	void ClearInputs() { tail = head; lastCommand = ""; }
private:
	float waitTime = 0.0f;

private:
	int InputsNum();
	string CheckCommand();


	int head = 0;
	int tail = 0;
	vector<UINT> inputs;	//검사를 위한 서큘러 큐


private:

	string lastCommand = "";
	unordered_map<string, vector<UINT>> commands;
};

