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
//flag״̬�֣�0���ȴ�|����״̬ 1������״̬ 2�������� 3������� 
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
	cout<<"��ǰʱ��:"<<time<<endl;
}
void disPCB(PCB* pcb){
	cout<<"������:����"<<pcb->name<<"\t";
	cout<<"���ȼ�:"<<pcb->priority<<"\t";
	cout<<"����ʱ��:"<<pcb->start<<"\t";
	cout<<"��Ҫ����ʱ��:"<<pcb->need<<"\t";
	cout<<"����CPUʱ��:"<<pcb->CPU<<"\t";
	cout<<"����״̬:";
	switch(pcb->flag){
		case 0:
			cout<<"����״̬"; break;
		case 1:
			cout<<"����״̬"; break;
		case 2:
			cout<<"������"; break;
		case 3:
			cout<<"�����\t����ʱ��:"<<pcb->end; break;
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
	cout<<"������"<<endl;
	Sleep(sleepTime);
	pcb->CPU++;
	if (pcb->CPU==pcb->need) {
		cout<<"����"<<pcb->name<<"�����"<<endl;
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
