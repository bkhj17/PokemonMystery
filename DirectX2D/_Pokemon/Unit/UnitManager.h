#pragma once

class Unit;
enum PokemonType {
	UNKNOWN = 0,//???
	GRASS,		//풀
	FIRE,		//불
	WATER,		//물
	THUNDER,	//전기
	FLY,		//비행
	ICE,		//얼음
	ROCK,		//바위
	GROUND,		//땅
	POISON,		//독
	BUG,		//벌레
	NORMAL,		//노말
	FIGHT,		//격투
	PSYCHIC,	//에스퍼
	GHOST,		//유령
	DRAGON,		//드래곤
	DARK,		//악
	METAL,		//강철
	FAIRY,		//페어리
	END			//타입 종류 수
};

struct PokemonTableData {
	//번호(키)
	int key = 0;
	//이름
	string name;
	//포켓몬의 타입. 기술 타입으로도 사용
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
	//특성은 제외
	//상태이상 정보는 유닛에서 직접 갖기로 한다
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
		END	//턴 끝나는 거 아님. 그냥 마지막 인덱스임
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
	//선언 관련
	map<int, PokemonTableData> pokemonTable;


	map<int, AnimData> animTable;

	//행동 관련
	Unit* player = nullptr;
	vector<Unit*> friends;
	vector<Unit*> enemies;

	Phase curPhase = PLAYER_COMMAND;
};

