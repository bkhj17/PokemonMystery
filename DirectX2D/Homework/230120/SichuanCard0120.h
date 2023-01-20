#pragma once

class SichuanCard0120 : public Button
{
public:
	SichuanCard0120(Vector2 size);
	~SichuanCard0120() = default;

	void Init(int index, Vector2 pos, wstring file);
	int GetIndex() { return index; }

	void SetSelected(bool selection) { selected = selection; }
private:
	int index = -1;
	bool selected = false;
};

