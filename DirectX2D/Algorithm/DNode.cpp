#include "Framework.h"
#include "DNode.h"

DNode::DNode(Vector2 pos)
	: pos(pos)
{
	collider = new CircleCollider(30);
	collider->Pos() = pos;
	collider->UpdateWorld();
}

DNode::~DNode()
{
	delete collider;

	for (auto edge : edges)
		delete edge;
}

void DNode::Render()
{
	collider->Render();

	for (auto edge : edges)
		edge->line->Render();
}

void DNode::AddEdge(DNode* node)
{
	auto edge = new Edge(node->pos, pos);
	edge->node = node;
	edge->cost = Distance(node->pos, pos);

	edges.push_back(edge);

}
