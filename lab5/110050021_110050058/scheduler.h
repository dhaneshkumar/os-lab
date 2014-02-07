#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include "tools.h"
#include "event_manager.h"
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

extern vector<Event> eventList;
extern priority_queue<int> eventTable;

class Scheduler
{
	EventManager em;
	int cpuTime;
	
public:
	Scheduler(){};
	void addProcess(process p);
	void saveStatus(process p);
	void schedule();


};



#endif