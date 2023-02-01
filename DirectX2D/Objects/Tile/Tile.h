#pragma once

class Tile : public Button
{
public:
	enum Type : int {
		BG, OBJ
	};

	struct Data {
		wstring textureFile;
		Vector2 pos;
		float angle = 0.0f;
		Type type = BG;
	};

public:
	Tile(Data data);
	~Tile();

	void Update() override;

	void SetTexture(wstring file);
	void SetAngle(float angle);

	Data& GetData() { return data; }
private:
	Data data;
};

