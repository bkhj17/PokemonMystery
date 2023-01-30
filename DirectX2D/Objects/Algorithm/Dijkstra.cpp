#include "Framework.h"
#include "Dijkstra.h"

Dijkstra::~Dijkstra()
{
	for (auto node : nodes)
		delete node;
}

void Dijkstra::Update()
{
	if (KEY_DOWN('A'))
		CreateNode(mousePos);

	if (KEY_DOWN(VK_RBUTTON)) {
		for (auto node : nodes) {
			if (node->collider->IsPointCollision(mousePos)) {
				selectNode = node;
				break;
			}
		}
	}

	if (KEY_UP(VK_RBUTTON)) {
		if (selectNode == nullptr)
			return;

		for (auto node : nodes) {
			if (node == selectNode)
				continue;

			if (node->collider->IsPointCollision(mousePos))	{
				node->AddEdge(selectNode);
				selectNode->AddEdge(node);
				break;
			}
		}
	}
}

void Dijkstra::Render()
{
	for (auto node : nodes)
		node->Render();
}

int Dijkstra::FineCloseNode(Vector2 pos)
{
	float minDist = FLT_MAX;
	int index = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		float distance = Distance(pos, nodes[i]->pos);
		if (minDist > distance) {
			minDist = distance;
			index = i;
		}
	}

	return index;
}

void Dijkstra::SetEdgeInfo(int curIndex)
{
	if (curIndex == -1) return;

	nodes[curIndex]->isClosed = true;

	float cost = nodes[curIndex]->cost;
	for (auto edge : nodes[curIndex]->edges) {
		if (edge->node->cost > cost + edge->cost) {
			edge->node->cost = cost + edge->cost;
			edge->node->via = curIndex;
		}
	}
}

int Dijkstra::FindMinCostNode()
{
	float minCost = FLT_MAX;
	int index = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		if (nodes[i]->isClosed)
			continue;

		float cost = nodes[i]->cost;
		if (minCost > cost) {
			minCost = cost;
			index = i;
		}
	}

	return index;
}

void Dijkstra::ResetNodes()
{
	for (auto node : nodes) {
		node->isClosed = false;
		node->via = -1;
		node->cost = FLT_MAX;
	}
}

vector<Vector2> Dijkstra::GetPath(int start, int end)
{
	ResetNodes();

	//1.시작노드 초기화
	int curIndex = start;
	nodes[curIndex]->cost = 0;
	nodes[curIndex]->via = start;
	for (auto node : nodes) {
		//선택 노드의 인접노드 초기화 + 선택 노드 닫기
		SetEdgeInfo(curIndex);

		//열린 노드 중 제일 효율 좋은 노드 찾기
		curIndex = FindMinCostNode();

	}

	//백트래킹

	curIndex = end;
	vector<Vector2> path;
	while (curIndex != start) {
		path.push_back(nodes[curIndex]->pos);
		curIndex = nodes[curIndex]->via;

		if (curIndex == -1)
			break;
	}

	return path;
}

void Dijkstra::CreateNode(Vector2 pos)
{
	auto node = new DNode(pos);
	node->index = nodes.size();
	nodes.push_back(node);
}
