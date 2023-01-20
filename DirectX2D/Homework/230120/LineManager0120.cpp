#include "Framework.h"
#include "LineManager0120.h"
#include "LineRenderer0120.h"

LineManager0120::LineManager0120()
{
	lines.resize(50);
	for (auto& line : lines)
		line = new LineRenderer0120();
}

LineManager0120::~LineManager0120()
{
	for (auto line : lines)
		delete line;
}

void LineManager0120::Update()
{
	for (auto line : lines)
		line->Update();
}

void LineManager0120::Render()
{
	for (auto line : lines)
		line->Render();
}

LineRenderer0120* LineManager0120::Pop()
{
	for (auto line : lines) {
		if (!line->IsActive())
			return line;
	}

	return nullptr;
}
