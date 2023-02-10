#pragma once
class MiniMap : public GameObject
{
public:
	MiniMap();
	~MiniMap();

	void Init();
	void UpdateMap(void* pointPtr);	//�÷��̾ �����̸� �ش���ġ���� ����Ÿ���� �޾ƿ� ������Ʈ
	void Render();

	void Clear();
private:
	map<POINT, int> info; //����Ÿ�� : �ش� ��ġ�� ��� Ÿ���� �׸��� �÷���
	set<POINT> objTilePos;
	set<POINT> itemPos;	
	set<POINT> unitPos;
	Quad* quad;
};

