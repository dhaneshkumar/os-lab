#include "event_manager.h"
using namespace std;



void Scheduler::addProcess(process p)
{
	//cout<<"phase size : "<<p.phases.size()<<endl;

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
			process pc =eventTable.top();
			eventTable.pop();
			saveStatus(pc);
			eventTable.push(pc);
			eventTable.push(p);
			schedule();
		}

	}
	else
	{
		eventTable.push(p);
		schedule();
	}
}



void Scheduler::schedule()
{
	if(!eventTable.empty())
	{
	//cout<<"scheduled program -size: "<<eventTable.size()<<endl<<endl;

	process cp= eventTable.top();
	

	cout<<"pid : "<< cp.p_id<<"  pstate : "<<cp.state<<" pphase time : "<<cp.phases.front().cpu_time<<endl;
	//cout<<cpuTime<<endl;
	//cout<<clocktime<<" --"<<endl;
	cpuTime= clocktime;

	Event  e;
	e.startTime = clocktime + cp.phases.front().cpu_time ;
	e.type = 2;
	e.newProcess = cp;

	em.addEvent(e);
	}

}


void Scheduler::saveStatus(process &p)
{
	int burstTime = clocktime-cpuTime;

	//cout<<"cpu time : "<<cpuTime<<"   "<<clocktime<<endl;

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
		//cout<<"phase time : "<<p.phases[0].cpu_time<<endl;
		p.phases.front().cpu_time = p.phases[0].cpu_time -burstTime;

		

		Event  e;
		//e.startTime = clocktime ;
		e.type = 1;
		e.newProcess = p;
		em.removeEvent(e);
		//cout<<"remaining phase time : "<<p.phases.front().cpu_time<<endl;
		//cout<<"process removed"<<endl;
	}
	else
		cout<<"Error : burst time > phase cput time"<<endl;
	

}