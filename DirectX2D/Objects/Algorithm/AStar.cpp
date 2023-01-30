#include "Framework.h"

AStar::AStar(GameTileMap* map)
{
	map->GetNodes(nodes);
	SetEdge(map->GetWidth());

	heap = new Heap;
}

AStar::~AStar()
{
	for (auto node : nodes)
		delete node;

	delete heap;
}

void AStar::Render()
{
	for (auto node : nodes)
		node->Render();
}

int AStar::FindCloseNode(Vector2 pos)
{
	float minCost = FLT_MAX;
	int index = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		float cost = Distance(nodes[i]->pos, pos);
		if (minCost > cost) {
			minCost = cost;
			index = i;
		}
	}

	return index;
}

void AStar::GetPath(IN int start, IN int end, OUT vector<Vector2>& path)
{
	Reset();
	path.clear();

	if (start == end)
		return;

	float G = 0;
	float H = GetManhattanDistance(start, end);

	nodes[start]->f = G + H;
	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;

	heap->Insert(nodes[start]);
	while (nodes[end]->state != Node::CLOSED) {
		if (heap->Empty())
			return;

		int curIndex = GetMinNode();
		Extend(curIndex, end);
		nodes[curIndex]->state = Node::CLOSED;
	}

	int curIndex = end;
	while (curIndex != start) {
		nodes[curIndex]->state = Node::USING;
		path.push_back(nodes[curIndex]->pos);
		curIndex = nodes[curIndex]->via;
	}

	path.push_back(nodes[start]->pos);
}

void AStar::Reset()
{
	for (Node* node : nodes) {
		if (node->state != Node::OBSTACLE)
			node->state = Node::NONE;
	}

	heap->Clear();
}

float AStar::GetManhattanDistance(int start, int end)
{
	//dx + dy
	Vector2 startPos = nodes[start]->pos;
	Vector2 endPos = nodes[end]->pos;

	Vector2 temp = endPos - startPos;

	if (isDiagonal) {
		float mx = max(temp.x, temp.y);
		float mn = min(temp.x, temp.y);
		return mx - mn + mn * sqrtf(2.0f);
	}
	else
		return abs(temp.x) + abs(temp.y);
}

void AStar::Extend(int center, int end)
{
	for (auto edge : nodes[center]->edges) {
		int index = edge->index;
		//지나온
		if (nodes[index]->state == Node::CLOSED)
			continue;
		//못 감
		if (nodes[index]->state == Node::OBSTACLE)
			continue;

		float G = nodes[center]->g + edge->cost;
		float H = GetManhattanDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == Node::OPEN) {
			if (F < nodes[index]->f) {
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		else if (nodes[index]->state == Node::NONE) {
			nodes[index]->h = H;
			nodes[index]->g = G;
			nodes[index]->f = F;
			nodes[index]->via = center;
			nodes[index]->state = Node::OPEN;
			//openNodes.push_back(index);
			heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	/*
	int openIndex = 0;


	int nodeIndex = openNodes[openIndex];
	float minF = nodes[nodeIndex]->f;

	for (UINT i = 0; i < openNodes.size(); i++) {
		nodeIndex = openNodes[i];
		if (nodes[nodeIndex]->f < minF) {
			minF = nodes[nodeIndex]->f;
			openIndex = i;
		}
	}

	nodeIndex = openNodes[openIndex];
	openNodes.erase(openNodes.begin() + openIndex);
	return nodeIndex;
	*/
	return heap->DeleteRoot()->index;
}

void AStar::SetEdge(UINT width)
{
	//y * width + x

	for (int i = 0; i < nodes.size(); i++) {

		if ((i + 1) % width != 0) {
			nodes[i]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width) {
			nodes[i]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i]);
		}

	}
	
	if(isDiagonal)
		SetEdgeDiagonal(width);
}

void AStar::SetEdgeDiagonal(UINT width)
{
	for (int i = 0; i < nodes.size(); i++) {
		if ((i + 1) % width != 0 && i < nodes.size() - width) {
			nodes[i]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i]);
		}

		if (i % width != 0 && i < nodes.size() - width) {
			nodes[i]->AddEdge(nodes[i + width - 1]);
			nodes[i + width - 1]->AddEdge(nodes[i]);
		}
	}

}