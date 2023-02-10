#pragma once
class MiniMap : public GameObject
{
public:
	MiniMap();
	~MiniMap();

	void Init();
	void UpdateMap(void* pointPtr);	//플레이어가 움직이면 해당위치에서 공개타일을 받아와 업데이트
	void Render();

	void Clear();
private:
	map<POINT, int> info; //공개타일 : 해당 위치의 배경 타일의 그리드 플래그
	set<POINT> objTilePos;
	set<POINT> itemPos;	
	set<POINT> unitPos;
	Quad* quad;
};

