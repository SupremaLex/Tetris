#pragma once

#include <vector>

class Observer
{
public:
	virtual void update() = 0;
};

class Observable
{
public:
	void add_observer(Observer* observer)
	{
		observers.push_back(observer);
	}
	void notify_update()
	{
		for (auto& obs : observers)
			obs->update();
	}
private:
	std::vector<Observer*> observers;
};