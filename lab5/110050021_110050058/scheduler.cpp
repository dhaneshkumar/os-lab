#include "event_manager.h"
using namespace std;



void Scheduler::addProcess(process p)
{
	if(!eventTable.empty())
	{
		process pt=eventTable.top();
		//cout<<"priority : "<<(eventTable.top()).start_priority<<endl;

		if(p.start_priority <= pt.start_priority)
		{
			eventTable.push(p);
		}
		else
		{
			saveStatus(eventTable.top());
			eventTable.push(p);
		}

	}
	else
	{
		eventTable.push(p);
	}
}



void Scheduler::schedule()
{
	cout<<"scheduled program : "<<endl<<endl;
	process cp= eventTable.top();
	

	cout<<"process id : "<< cp.p_id<<endl;
	cout<<"process state : "<<cp.state<<endl;
	cout<<"process time: "<<cp.admission<<endl;

	//cout<<cpuTime<<endl;
	//cout<<clocktime<<" --"<<endl;
	cpuTime= clocktime;

	Event  e;
	e.startTime = clocktime + cp.phases.front().cpu_time ;
	e.type = 2;
	e.newProcess = cp;

	em.addEvent(e);

}


void Scheduler::saveStatus(process p)
{
	int burstTime = clocktime-cpuTime;

	if(p.phases.front().cpu_time -burstTime ==0)
	{
		process cp= eventTable.top();
		cpuTime= clocktime;

		Event  e;
		e.startTime = clocktime + cp.phases.front().cpu_time ;
		e.type = 2;
		e.newProcess = cp;

		em.addEvent(e);
	}
	else if(p.phases.front().cpu_time -burstTime >0)
	{
		p.phases.front().cpu_time = p.phases.front().cpu_time -burstTime;
	}
	else
		cout<<"Error : burst time > phase cput time"<<endl;
	

}