#pragma once
class Dijkstra
{
public:
	Dijkstra() = default;
	~Dijkstra();

	void Update();
	void Render();

	int FineCloseNode(Vector2 pos);
	void SetEdgeInfo(int curIndex);
	int FindMinCostNode();

	void ResetNodes();

	vector<Vector2> GetPath(int start, int end);
private:
	void CreateNode(Vector2 pos);

private:
	vector<DNode*> nodes;

	DNode* selectNode;
};

