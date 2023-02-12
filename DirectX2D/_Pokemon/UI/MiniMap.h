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
	map<pair<int, int>, int> info; //����Ÿ�� : �ش� ��ġ�� ��� Ÿ���� �׸��� �÷���
	set<pair<int, int>> objTilePos;
	set<pair<int, int>> itemPos;	
	set<pair<int, int>> unitPos;
	Quad* quad;
};

