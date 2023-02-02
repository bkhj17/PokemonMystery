#pragma once
class BgTileManager : public Singleton<BgTileManager>
{
public:
	enum TileType {
		NONE, WALL, WATER, LAND
	};
	const POINT GRID_SIZE = { 3, 24 };
private:
	friend class Singleton;
	BgTileManager();
	~BgTileManager() = default;

	void InitGridInfo();
public:
	void SetTexture(wstring path);
	POINT GetGrid(int flag);	//상하좌우 대각선 정보를 비트마스킹
	bool IsRoom(int flag);
	TileType GetTileType(Texture* texture);

private:
	void InitVCheck();
	void InitRoomCheck();
	bool CheckGrid(const int& flag, int check, int uncheck);
	
private:
	map<int, POINT> tileGrid;

	Texture* wallTexture = nullptr;
	Texture* waterTexture = nullptr;
	Texture* landTexture = nullptr;

	vector<pair<int, int>> vcheck;
	vector<pair<int, int>> roomCheck;

};

