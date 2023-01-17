#pragma once
#include "Singleton.h"
class Observer : public Singleton<Observer>
{
private:
	friend class Singleton;

	Observer() = default;
	~Observer() = default;
public:
	void AddEvent(string key, Event e);
	void AddParamEvent(string key, ParamEvent pe);

	void ExecuteEvent(string key);
	void ExecuteParamEvent(string key, void* param);

private:
	unordered_map<string, vector<Event>> totalEvent;
	unordered_map<string, vector<ParamEvent>> totalParamEvent;
};

