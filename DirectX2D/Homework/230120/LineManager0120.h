#pragma once

class LineRenderer0120;

class LineManager0120 : public Singleton<LineManager0120>
{
private:
	friend class Singleton;
	LineManager0120();
	~LineManager0120();

public:
	void Update();
	void Render();

	LineRenderer0120* Pop();

private:
	vector<LineRenderer0120*> lines;

};

