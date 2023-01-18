#include "Framework.h"

Clip::Clip(vector<Frame*> frames, bool isLoop, float speed)
	: frames(frames), isLoop(isLoop), speed(speed)
{
	delayTime = 1.0f / FPS;
}

Clip::~Clip()
{
	for (auto frame : frames)
		delete frame;
}

void Clip::Update()
{
	if (!isPlay)
		return;

	frameTime += speed * DELTA;

	if (frameTime < delayTime)
		return;

	frameTime -= delayTime;
	++curFrameNum;

	if (events.count(curFrameNum) > 0)
		events[curFrameNum]();

	if (isLoop) {
		curFrameNum %= frames.size();
	}
	else {
		if (curFrameNum >= frames.size()) {
			curFrameNum--;
			Stop();
		}
	}

}

void Clip::Render()
{
	frames[curFrameNum]->Render();
}

void Clip::Play()
{
	isPlay = true;
	curFrameNum = 0;
	frameTime = 0.0f;
}

void Clip::SetEvent(Event event, int frameNum)
{
	if (frameNum < 0 || frameNum >= frames.size())
		frameNum = (int)frames.size() - 1;

	events[frameNum] = event;
}
