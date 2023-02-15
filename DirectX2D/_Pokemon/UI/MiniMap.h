#pragma once
class MiniMap : public GameObject
{
public:
	MiniMap();
	~MiniMap();

	void Init();
	void UpdateMap(void* unitPtr);	//�÷��̾ �����̸� �ش���ġ���� ����Ÿ���� �޾ƿ� ������Ʈ
	void PostRender();

	void Clear();
private:
	map<pair<int, int>, int> info; //����Ÿ�� : �ش� ��ġ�� ��� Ÿ���� �׸��� �÷���
	Quad* quad;


	IntValueBuffer* gridBuffer;
};

