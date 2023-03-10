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
	int key = 0;				//번호(키)
	string name;				//이름
	vector<PokemonType> type;	//포켓몬의 타입. 기술 타입으로도 사용

	int maxHp = 1;
	int attack = 1;
	int defense = 1;
	int sAttack = 1;
	int sDefense = 1;
	int speed = 1;

	static int ApplyLevelHp(int level, int hp) {	//최대 HP 계산식
		float levelRate = (float)level / 100.0f;
		return (int)((hp * 2 + 100) * levelRate) + 10;
	}

	static int ApplyLevel(int level, int num) {		//HP를 제외한 능력치 계산식
		float levelRate = (float)level / 100.0f;
		return (int)((num * 2) * levelRate) + 5;
	}
};

struct PokemonData {
	int key = 0; 
	int level = 1;
	int curHp = 1;
	PokemonTableData statusData;
};

class UnitManager : public Singleton<UnitManager>
{
private:
	const Vector2 UNIT_SIZE = { 80.0f, 80.0f };

	const string POKEMON_TABLE			= "TextData/PokemonData.csv";
	const string POKEMON_SKILL_TABLE	= "TextData/PokemonSkillData.csv";
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
	void GetInitSkills(IN int key, IN int level, OUT vector<int>& result);
	void GetSkillDataKey(IN int key, IN int level, OUT vector<int>& v);

	//주인공
	Unit* GetPlayer() { return player; }

	//동료
	vector<Unit*>& GetFriends() { return friends; }

	//적
	void InitEnemy();
	void InitEnemy(int key, int level);
	vector<Unit*>& GetEnemies() { return enemies; }
	void ClearEnemy();

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
	void LoadPokemonTable();
	void LoadAnimData();
	void LoadPokemonSkillTable();
private:
	//선언 관련
	map<int, PokemonTableData> pokemonTable;
	map<int, map<int, vector<int>>> pokemonSkillTable;
	map<int, AnimData> animTable;

	//행동 관련
	Unit* player = nullptr;
	vector<Unit*> friends;
	vector<Unit*> enemies;

	Phase curPhase = PLAYER_COMMAND;

	bool playerCommanded = false;

	ColorBuffer* notShadeBuffer;
};

