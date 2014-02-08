#include "event_manager.h"
using namespace std;



void Scheduler::addProcess(process p)
{
	//cout<<"queue size : "<<eventTable.size()<<endl;

	if(!eventTable.empty())
	{		
			eventTable.push(p);
	}
	else
	{
		eventTable.push(p);
		if(cpuFree==1)
		{
			schedule();
		}
	}

	//cout<<"queue size : "<<eventTable.size()<<endl;
}



void Scheduler::schedule()
{
	//cout<<"scheduled program -size: "<<eventTable.size()<<endl<<endl;
	if(!eventTable.empty())
	{
	//cout<<"scheduled program -size: "<<eventTable.size()<<endl<<endl;
	cpuFree =0;
	process cp= eventTable.front();
	

	cout<<"pid : "<< cp.p_id<<"  pstate : "<<cp.state<<" pphase time : "<<cp.phases.front().cpu_time<<endl;
	//cout<<cpuTime<<endl;
	//cout<<clocktime<<" --"<<endl;
	cpuTime= clocktime;

	if(timeslice >= cp.phases.front().cpu_time)
	{
		//cout<<1111<<endl;
		Event  e;
		e.startTime = clocktime + cp.phases.front().cpu_time ;
		e.type = 2;
		e.newProcess = cp;
		em.addEvent(e);
	}
	else
	{

		cout<<cp.phases[0].cpu_time<<"  - time -  "  << timeslice <<endl;
		
		if(cp.phases[0].iterations>1){
			vector<process_phase>::iterator it = cp.phases.begin();
  			it = cp.phases.insert ( it , cp.phases[0] );
			cp.phases[0].cpu_time = cp.phases[0].cpu_time -timeslice;
			cp.phases[0].iterations =1;
			cp.phases[1].iterations--;
		}
		else
		{
			cp.phases[0].cpu_time = cp.phases[0].cpu_time -timeslice;
		}

		Event  e;
		e.startTime = clocktime + timeslice;
		e.type = 4;
		e.newProcess = cp;
		em.addEvent(e);
		//eventTable.push(cp);
	}
	eventTable.pop();

	}

}

/*
void Scheduler::saveStatus(process &p)
{
	int burstTime = clocktime-cpuTime;

	//cout<<"cpu time : "<<cpuTime<<"   "<<clocktime<<endl;

	if(p.phases.front().cpu_time -burstTime ==0)
	{
		process cp= eventTable.front();
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
	

}*/