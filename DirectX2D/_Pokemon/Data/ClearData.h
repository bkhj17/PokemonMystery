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
	//���� �̸�
	string GetDungeonName();
	string GetFloorStr() { return dungeonData->FloorString(); }

	void SetPlayerData(PokemonData* data) { playerData = data;}
	//�÷��̾� �̸�
	string GetPlayerName();
	//�÷��̾� ����
	int GetPlayerLevel();
private:
	FloorData* dungeonData = nullptr;
	PokemonData* playerData = nullptr;
};

