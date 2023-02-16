#pragma once

class MiniMap : public GameObject
{
private:
	enum OBJ_SIGN {
		NONE,
		PLAYER,
		ENEMY,
		STAIR,
		REFRESH,
		ITEM
	};

public:
	MiniMap();
	~MiniMap();

	void UpdateMap();
	
	void PostRender();

	void Clear();
private:
	map<pair<int, int>, int> info; //공개타일 : 해당 위치의 배경 타일의 그리드 플래그
	Quad* quad;

	IntValueBuffer* gridBuffer;

	map<pair<int, int>, int> objInfo;
};

