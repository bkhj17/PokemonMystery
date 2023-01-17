#include "Framework.h"
#include "Observer.h"

void Observer::AddEvent(string key, Event e)
{
	totalEvent[key].push_back(e);
}

void Observer::AddParamEvent(string key, ParamEvent pe)
{
	totalParamEvent[key].push_back(pe);
}

void Observer::ExecuteEvent(string key)
{
	for (auto& e : totalEvent[key]) {
		e();
	}
}

void Observer::ExecuteParamEvent(string key, void* param)
{
	for (auto& pe : totalParamEvent[key]) {
		pe(param);
	}
}
