#pragma once
class AStar
{
public:
	AStar(class GameTileMap* map);
	~AStar();

	void Update();
	void Render();

	int FindCloseNode(Vector2 pos);
	int FindRandomPos(Vector2 pos, float range);

	void GetPath(IN int start, IN int end, OUT vector<Vector2>& path);
protected:
	void Reset();
	float GetManhattanDistance(int start, int end);

	void Extend(int center, int end);
	int GetMinNode();

	void SetEdge(UINT width);
	void SetEdgeDiagonal(UINT width);

protected:
	bool isDiagonal = false;
private:
	vector<Node*> nodes;
	//vector<int> openNodes;
	Heap* heap;

};

