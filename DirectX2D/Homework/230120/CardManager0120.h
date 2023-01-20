#pragma once
class SichuanCard0120;

struct PqNode {
	int curved;
	Vector2 postPos, direction;

	bool operator<(const PqNode& node) const {
		return curved > node.curved;
	}
};

class CardManager0120 : public Singleton<CardManager0120>
{
private:
	const int MAX_CURVE = 2;
	const int POOL_SIZE = 12;
	static const int TABLE_X = 4;
	static const int TABLE_Y = 3;
	const Vector2 CARD_SIZE = { 40.0f, 60.0f };

	static const Vector2 DIRECTION[];
private:
	friend class Singleton;
	CardManager0120();
	~CardManager0120();

	void SelectCard(void* card);
public:
	void Update();
	void Render();

	void InitTable(int);

	void SetMatchFunc(function<void()> event);
private:
	pair<int, int> SearchSetablePoint(vector<vector<bool>>& table, int x, int y);

	void LoadTextures();

	bool Pairing(SichuanCard0120* card1, SichuanCard0120* card2);
private:
	vector<wstring> cardTextures;
	vector<SichuanCard0120*> cards;

	SichuanCard0120* select1 = nullptr;
	SichuanCard0120* select2 = nullptr;

	Event matchFunc = nullptr;

	deque<Vector2> lineQueue;
	float showTime = 0.0f;
	float showRate = 1.0f;
	class LineRenderer0120* lineRenderer;

};

