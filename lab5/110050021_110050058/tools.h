#ifndef _TOOLS
#define _TOOLS
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;



struct process_phase{
	int iterations;
	int cpu_time;
	int io_time;
};

struct process{
	string state;
	int p_id;
	int start_priority;
	int admission;
	vector<process_phase> phases;

	friend bool operator<(process i, process j){return (i.start_priority < j.start_priority);}
};

struct sc_level{
	int level_number;
	int priority;
	int time_slice;
};

struct scheduler{
	int no_levels;
	vector<sc_level> levels;
};	


struct  Event
{
	/* data */
	int startTime;
	int type;
	process newProcess;

	friend bool operator<(Event i, Event j){return (i.startTime < j.startTime);}
	friend bool operator==(Event i, Event j){return (i.newProcess.p_id == j.newProcess.p_id);}
};





#endif