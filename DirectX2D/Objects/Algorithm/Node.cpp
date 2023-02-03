#include "Framework.h"

Node::Node(Vector2 pos, int index)
	: pos(pos), index(index)
{
	collider = new CircleCollider(10);
	collider->Pos() = pos;
	collider->UpdateWorld();
}

Node::~Node()
{
	for (auto edge : edges)
		delete edge;
	delete collider;
}

void Node::Render()
{
	switch (state)
	{
	case Node::NONE:
		collider->GetColor() = { 0, 1, 1, 1 };
		break;
	case OPEN:
		collider->GetColor() = { 1, 0, 1, 1 };
		break;
	case CLOSED:
		collider->GetColor() = { 1, 1, 0, 1 };
		break;
	case USING:
		collider->GetColor() = { 0, 1, 0, 1 };
		break;
	case OBSTACLE:
		collider->GetColor() = { 1, 0, 0, 1 };
		break;
	default:
		break;
	}

	if (CAM->ContainFrustum(collider->GlobalPos(), { 10, 10 }))
		collider->Render();

	for (Edge* edge : edges)
		edge->line->Render();
}

void Node::AddEdge(Node* node)
{
	Edge* edge = new Edge(node->pos, pos);
	edge->index = node->index;
	edge->cost = Distance(node->pos, pos);
	edges.push_back(edge);
}
