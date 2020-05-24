#include <iostream>
#include <windows.h>
using namespace std;
static int time = 0;
static long sleepTime = 100;
static int setting = 0;
static int setting1 = 0;
struct PCB{
	int name;
	int priority;
	int start;
	int end;
	int need;
	int CPU;
	int flag;
};
//flag状态字，0：等待|阻塞状态 1：就绪状态 2：运行中 3：已完成 
struct node{
	node* H;
	node* R;
	PCB* pcb;
	node* next;
};
void init(node* head){
	head->H=head;
	head->R=head;
	head->pcb=NULL;
	head->next=NULL;
}
void disTimeNow(){
	cout<<"当前时刻:"<<time<<endl;
}
void disPCB(PCB* pcb){
	cout<<"进程名:进程"<<pcb->name<<"\t";
	cout<<"优先级:"<<pcb->priority<<"\t";
	cout<<"到达时间:"<<pcb->start<<"\t";
	cout<<"需要运行时间:"<<pcb->need<<"\t";
	cout<<"已用CPU时间:"<<pcb->CPU<<"\t";
	cout<<"进程状态:";
	switch(pcb->flag){
		case 0:
			cout<<"阻塞状态"; break;
		case 1:
			cout<<"就绪状态"; break;
		case 2:
			cout<<"运行中"; break;
		case 3:
			cout<<"已完成\t结束时间:"<<pcb->end; break;
	}
	cout<<endl;
}
PCB* createProcess(int priority,int start,int need){
	PCB* pcb=new PCB();
	pcb->name=start;
	pcb->priority=priority;
	pcb->start=start;
	pcb->end=0;
	pcb->need=need;
	pcb->CPU=0;
	pcb->flag=0; 
}
void removeProcess(node* ready,node* finish,PCB* pcb){
	node* p=finish->R;
	p->next=new node();
	p=p->next;
	p->pcb=pcb;
	p->next=NULL;
	finish->R=p;
}
void cleanReady(node* ready){
	node* p=ready->H;
	while (p->next!=NULL){
		int flag=p->next->pcb->flag;
		if (flag==3) {
			node* tmp=p->next;
			p->next=tmp->next;
			free(tmp);
//			return ;
		}
		p=p->next;
	}
}
void moveToEnd(node* ready,PCB* pcb){
	PCB* tmp=new PCB();
	tmp->CPU=pcb->CPU;
	tmp->end=pcb->end;
	tmp->flag=pcb->flag;
	tmp->name=pcb->name;
	tmp->need=pcb->need;
	tmp->priority=pcb->priority;
	tmp->start=pcb->start;
	node* p=ready->R;
	p->next=new node();
	p=p->next;
	p->pcb=tmp;
	p->next=NULL;
	ready->R=p;
	pcb->flag=3;
	cleanReady(ready);
}
void runProcess(node* ready,node* finish,PCB* pcb){
	pcb->flag=2;
	disPCB(pcb);
	cout<<"运行中"<<endl;
	Sleep(sleepTime);
	pcb->CPU++;
	if (pcb->CPU==pcb->need) {
		cout<<"进程"<<pcb->name<<"已完成"<<endl;
		pcb->flag=3;
		pcb->end=time;
		removeProcess(finish,pcb);
		cleanReady(ready);
		return ;
	}
	pcb->flag=1;
	pcb->priority--;
	moveToEnd(ready,pcb);
}
void getReady(node* block,node* ready){
	node* p=block->H->next;
	while (p!=NULL){
		if (p->pcb->start==time){
			p->pcb->flag=1;
			moveToEnd(ready,p->pcb);
		}
		p=p->next;
	}
}
int main(){
	node* block=new node();
	init(block);
	node* ready=new node();
	init(ready);
	node* finish=new node();
	init(finish);
	return 0;
}
