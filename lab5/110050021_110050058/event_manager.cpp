#include "event_manager.h"
using namespace std;



EventManager::EventManager()
{
	clocktime = 0;
	 	for(int i=0;i<process_list.size();i++)
	 	{
	 		Event  e;
	 		e.startTime = process_list[i].admission;
	 		e.type = 1;
	 		e.newProcess = process_list[i];

	 		eventList.push_back(e);
	 	}

	 	sort (eventList.begin(), eventList.end());	
	 }



void EventManager::addEvent(Event  e)
{
	cout<<"new event added with process id : "<<e.newProcess.p_id<<"  | type : "<<e.type<<endl;
	Event  e1;
	e1.startTime = e.startTime;
	e1.type = e.type;
	e1.newProcess = e.newProcess;
	eventList.push_back(e1);
	sort (eventList.begin(), eventList.end());
}

void EventManager::removeEvent(Event  e)
{
	eventList.erase(remove(eventList.begin(), eventList.end(), e), eventList.end());
	// for(int j=0; j<eventList.size();j++)
	// {
	// 	if(eventList[j]==e)
	// 	{
	// 		eventList.erase (eventList.begin()+j);
	// 	}
	// }
	sort (eventList.begin(), eventList.end());
	//cout<<"event removed"<<endl;
}



void EventManager::admit(process p)
{
	//cout<<"process adding"<<endl;
		p.state="ready";
		sd->addProcess(p);

		//cout<<"process added"<<endl;
		//sd.schedule();
}


void EventManager::ioStart(process p){

	//cout<<"\iostarted"<<endl;
	p.state="blocked";
	eventTable.pop();

	cout<<"\nprocess id : "<<p.p_id<<" io--- time : "<<clocktime<<endl;

	Event  e;
	e.startTime = clocktime + p.phases.front().io_time ;
	e.type = 3;
	e.newProcess = p;

	addEvent(e);

	sd->schedule();
	//cout<<"iostarted  finished"<<endl;
}


void EventManager::ioComplete(process p){

	//cout<<"\nioComplete |size "<<p.phases.size()<<"  id "<<p.p_id<<"  "<<p.phases.front().iterations <<endl;
	
	
	if(p.phases.front().iterations==1)
	{
		p.phases.erase(p.phases.begin());
		cout<<"process id  : "<<p.p_id<<" | "<<clocktime<<endl;
	}
	else
	{
		cout<<"process id "<<p.p_id<<"next phase finished"<<p.phases.front().iterations<<endl;
		p.phases.front().iterations--;

	}

	if(!p.phases.empty())
	{
	Event  e;
	e.startTime = clocktime;
	e.type = 1;
	e.newProcess = p;

	addEvent(e);
	}else
	{
		cout<<"process completed ***************************************\\m/"<<endl<<endl;
	}
	//sd.schedule();
}

void EventManager::run()
{
	cout<<"program start :" <<endl;

	int dmd=1;
	while(!eventList.empty())
	{

		
		Event e = eventList.front();

		//cout<<"list size : "<<eventList.size()<< " -clocktime- "<<clocktime<<endl;

		clocktime =e.startTime;

		while(clocktime == e.startTime)
		{
			cout<<"\nevent :"<<dmd++<<" |type : "<<e.type<<" |time : "<<clocktime<<" |pid :"<<e.newProcess.p_id<<endl;
			if(e.type==1)
			{
				//cout<<" event type 1"<<endl;
				admit(e.newProcess);
			}
			else if(e.type==2)
			{
				//cout<<" event type 2"<<endl;
				ioStart(e.newProcess);
			}
			else if(e.type==3)
			{
				//cout<<" event type 3"<<endl;
				ioComplete(e.newProcess);
			}
			else
				cout<<"type error : "<<e.type<<endl;

			//cout<<"list size1 : "<<eventList.size()<<endl;
			
			eventList.erase(eventList.begin());
			//cout<<"list size1 : "<<eventList.size()<<endl;
			if(eventList.size()==0)
			{
				break;
			}
			e = eventList.front();
		}

		//cout<<"start scheduling "<<endl;
		//sd->schedule();

	}
}


