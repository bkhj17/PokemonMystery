#pragma once
class Card : public Button
{
public:
	Card(wstring textureFile);
	~Card();

	void SetCoord(int x, int y) { coord = { x, y }; }
	void SetCoord(POINT coord) { this->coord = coord; }
	POINT GetCoord() { return coord; }

	const wstring& GetKey() { return key; }

	void SetSelected(bool value) { isSelected = value; }

	void Update();
	void Render();
private:
	void OnClick();

private:
	wstring key;
	
	POINT coord = {};

	IntValueBuffer* intValueBuffer;
	FloatValueBuffer* floatValueBuffer;
	ColorBuffer* outlineColorBuffer;

	bool isSelected = false;
};

