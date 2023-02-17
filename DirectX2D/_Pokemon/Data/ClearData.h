#pragma once
struct FloorData;
struct PokemonData;
class ClearData : public Singleton<ClearData>
{
private:
	friend class Singleton;
	ClearData() = default;
	~ClearData() = default;
public:

	void SetDungeonData(FloorData* data) { dungeonData = data; }
	//던전 이름
	string GetDungeonName();
	string GetFloorStr() { return dungeonData->FloorString(); }

	void SetPlayerData(PokemonData* data) { playerData = data;}
	//플레이어 이름
	string GetPlayerName();
	//플레이어 레벨
	int GetPlayerLevel();
private:
	FloorData* dungeonData = nullptr;
	PokemonData* playerData = nullptr;
};

