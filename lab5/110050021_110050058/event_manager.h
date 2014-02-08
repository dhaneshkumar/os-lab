#ifndef EVENT_MANAGER
#define EVENT_MANAGER

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include "tools.h"
#include <algorithm>
using namespace std;

extern vector<Event> eventList;
extern priority_queue<process> eventTable;
extern vector<process> process_list;
extern scheduler my_scheduler;
extern int clocktime;
extern int cpuTime;
/*
* Event type : 
	admit :1
	ioStart	:2
	ioEnd :3
*/



/*********************************************************************/
//event manager class
class Scheduler;
class EventManager;

class  EventManager
{
	Event event; 
	Scheduler* sd;

public:
	 EventManager();
	 void addEvent(Event e);
	 void removeEvent(Event e);
	 void admit(process p);
	 void ioStart(process p);
	 void ioComplete(process p);
	 void run();

};


class Scheduler
{
	EventManager em;
	
public:
	Scheduler(){
		cpuTime=1;
	};
	void addProcess(process p);
	void saveStatus(process &p);
	void schedule();


};




#endif