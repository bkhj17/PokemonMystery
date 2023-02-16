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
	map<pair<int, int>, int> info; //����Ÿ�� : �ش� ��ġ�� ��� Ÿ���� �׸��� �÷���
	Quad* quad;

	IntValueBuffer* gridBuffer;

	map<pair<int, int>, int> objInfo;
};

