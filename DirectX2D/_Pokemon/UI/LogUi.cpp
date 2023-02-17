#include "Framework.h"

LogUi::LogUi()
	: UIWindow(0.0f, WIN_WIDTH, WIN_HEIGHT * 0.3f, 0.0f)
{
	Observer::Get()->AddEvent("ShowLog", bind(&LogUi::SetRenderTime, this, 10.0f));
}

void LogUi::Update()
{
	renderTime -= DELTA;
}

void LogUi::PostRender()
{
	if (renderTime <= 0.0f)
		return;

	__super::PostRender();

	vector<string> renderLog;
	LogManager::Get()->GetRenderLog(renderLog);

	for (int i = 0; i < renderLog.size(); i++)
		Font::Get()->RenderTextLeft(renderLog[i], Pos() + Vector2(-Half().x + 80.0f, LineY(i)));
}

float LogUi::LineY(int lineNum)
{
	return Half().y - 40.0f - lineNum * (cQuad->GetSize().y + 20.0f);
}
