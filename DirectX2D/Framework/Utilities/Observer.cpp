#include "Framework.h"

void Observer::AddEvent(string key, Event e)
{
	totalEvent[key].push_back(e);
}

void Observer::AddParamEvent(string key, ParamEvent pe)
{
	totalParamEvent[key].push_back(pe);
}

void Observer::AddIntParamEvent(string key, IntParamEvent paramEvent)
{
	totalIntParamEvent[key].push_back(paramEvent);
}

void Observer::AddGetEvent(string key, function<void(void**)> getEvent)
{
	totalGetEvent[key] = getEvent;
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

void Observer::ExecuteIntParamEvent(string key, int param)
{
	for (auto& pe : totalIntParamEvent[key]) {
		pe(param);
	}
}

void Observer::ExecuteGetEvent(string key, void** param)
{
	if(totalGetEvent.find(key) != totalGetEvent.end())
		totalGetEvent[key](param);
}

