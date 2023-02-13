#pragma once

class Unit;
enum PokemonType {
	UNKNOWN = 0,//???
	GRASS,		//Ǯ
	FIRE,		//��
	WATER,		//��
	THUNDER,	//����
	FLY,		//����
	ICE,		//����
	ROCK,		//����
	GROUND,		//��
	POISON,		//��
	BUG,		//����
	NORMAL,		//�븻
	FIGHT,		//����
	PSYCHIC,	//������
	GHOST,		//����
	DRAGON,		//�巡��
	DARK,		//��
	METAL,		//��ö
	FAIRY,		//��
	END			//Ÿ�� ���� ��
};

struct PokemonTableData {
	//��ȣ(Ű)
	int key = 0;
	//�̸�
	string name;
	//���ϸ��� Ÿ��. ��� Ÿ�����ε� ���
	vector<PokemonType> type;

	int maxHp = 1;
	int attack = 1;
	int defense = 1;
	int sAttack = 1;
	int sDefense = 1;
	int speed = 1;

	static int ApplyLevelHp(int level, int hp) {
		float levelRate = (float)level / 100.0f;
		return (int)((hp * 2 + 100) * levelRate) + 10;
	}

	static int ApplyLevel(int level, int num) {
		float levelRate = (float)level / 100.0f;
		return (int)((num * 2) * levelRate) + 5;
	}

};

struct PokemonData {
	int key = 0; 
	int level = 1;
	int curHp = 1;
	PokemonTableData statusData;
	//Ư���� ����
	//�����̻� ������ ���ֿ��� ���� ����� �Ѵ�
};

class UnitManager : public Singleton<UnitManager>
{
private:
	const Vector2 UNIT_SIZE = { 80.0f, 80.0f };

private:
	friend class Singleton;
	UnitManager();
	~UnitManager();

public:
	enum UnitType {
		PLAYER, FRIEND, ENEMY
	};

	enum Phase {
		PLAYER_COMMAND,
		PLAYER_ACT,
		FRIEND_SKILL,
		FRIEND_MOVE,
		ENEMY_SKILL,
		ENEMY_MOVE,
		TURN_END,
		END	//�� ������ �� �ƴ�. �׳� ������ �ε�����
	};

	void Init();
	void Update();
	void Render();

	void RunPhase();

	bool IsActing();

	void GetPokemonData(IN int key, IN int level, OUT PokemonData*& data);

	Unit* GetPlayer() { return player; }
	vector<Unit*>& GetFriends() { return friends; }
	vector<Unit*>& GetEnemies() { return enemies; }

	bool IsUnitOnPoint(POINT point);
	Unit* GetUnitOnPoint(POINT point);
	bool CheckMovablePoint(POINT point, int dirX, int dirY);



	struct AnimData {
		int pokemonNum = 1;
		wstring textureFile;
		Vector2 texcoord;
		map<int, vector<POINT>> clipData;
	};
	AnimData* GetAnimData(int pokemonNum) {
		if (animTable.find(pokemonNum) == animTable.end())
			return nullptr;

		return &animTable[pokemonNum];
	}

private:
	void ApplyLevel(IN int level, OUT PokemonData* data);
	void LoadPokemonTable(string fileName);
	void LoadAnimData();
private:
	//���� ����
	map<int, PokemonTableData> pokemonTable;


	map<int, AnimData> animTable;

	//�ൿ ����
	Unit* player = nullptr;
	vector<Unit*> friends;
	vector<Unit*> enemies;

	Phase curPhase = PLAYER_COMMAND;
};

