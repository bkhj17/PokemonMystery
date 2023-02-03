#pragma once
class BgTileManager : public Singleton<BgTileManager>
{
public:
	enum GridFlag : int {
		NONE		= 0,
		LEFT_UP		= 0x01,
		UP			= 0x01 << 1,
		RIGHT_UP	= 0x01 << 2,
		LEFT		= 0x01 << 3,
		RIGHT		= 0x01 << 4,
		LEFT_DOWN	= 0x01 << 5,
		DOWN		= 0x01 << 6,
		RIGHT_DOWN	= 0x01 << 7,
		ALL			= 0xFF
	};

	enum TileType : int {
		INVALID, WALL, WATER, LAND
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

	bool CheckMovable(int tileFlag, int dirX, int dirY);

	Texture* GetWallTexture() { return wallTexture; }
	Texture* GetLandTexture() { return landTexture; }
	Texture* GetWaterTexture() { return waterTexture; }


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

