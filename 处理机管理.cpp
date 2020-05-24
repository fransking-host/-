#include <iostream>
#include <String.h>
#include <windows.h>
using namespace std;
static int setting = 0;
static int time=0;
struct PCB{
	int name;
	int priority;
	int start;
	int need;
	int CPU;
	int flag;
	int finish;
	public :PCB(string name);
	public :PCB(int priority,int start,int need);
};

PCB:: PCB(int priority,int start,int need){
	this->priority=priority;
	this->start=start;
	this->name=start;
	this->need=need;
	this->CPU=0;
	this->flag=0;
}
struct node{
	node* H;
	PCB* pcb;
	node* R;
	node* next;
};
void dis(PCB* pcb){
	if (pcb==NULL) return ;
	cout<<"进程名:进程"<<pcb->name<<"\t";
	cout<<"优先级:"<<pcb->priority<<"\t";
	cout<<"到达时间:"<<pcb->start<<"\t";
	cout<<"需要运行时间:"<<pcb->need<<"\t";
	cout<<"已用CPU时间:"<<pcb->CPU<<"\t";
	cout<<"进程状态:";
	switch(pcb->flag){
		case 0:
			cout<<"就绪状态"; break;
		case 1:
			cout<<"运行状态"; break;
		case 2:
			cout<<"已完成"; break;
	}
	cout<<endl;
}

void swap(node* p,node* q){
	node* tmp=p;
	p->pcb=q->pcb;
	q->pcb=tmp->pcb;
}
//高优先级优先 priority-scheduling algorithm
PCB* PSA(node* H){
	node* p=H->next;
	node* tmp=new node();
	tmp->pcb=p->pcb;
	while (p!=NULL){
		if (p->pcb->flag==2){
			p=p->next;
			if (p==NULL) break;
			if (p->pcb->flag==0) tmp->pcb=p->pcb; 
			continue;
		}
		int a=tmp->pcb->priority;
		int b=p->pcb->priority;
		if (p->pcb->flag==0&&a<b){
			tmp->pcb=p->pcb;
		}
		p=p->next;
	}
	return tmp->pcb;
} 
//先来先服务 first come first serverd
PCB* FCFS(node* H){
	node* p=H->next;
	node* tmp=new node();
	tmp->pcb=p->pcb;
	while (p!=NULL){
		if (p->pcb->flag==2){
			p=p->next;
			if (p==NULL) break;
			if (p->pcb->flag==0) tmp->pcb=p->pcb; 
			continue;
		}
		int a=tmp->pcb->start;
		int b=p->pcb->start;
		if (p->pcb->flag==0&&a>b){
			tmp->pcb=p->pcb;
		}
		p=p->next;
	}
	return tmp->pcb;
} 
//短作业优先 short job first 
PCB* SJF(node* H){
	node* p=H->next;
	node* tmp=new node();
	tmp->pcb=p->pcb;
	while (p!=NULL){
		if (p->pcb->flag==2){
			p=p->next;
			if (p==NULL) break;
			if (p->pcb->flag==0) tmp->pcb=p->pcb; 
			continue;
		}
		int a=tmp->pcb->need;
		int b=p->pcb->need;
		if (p->pcb->flag==0&&a>b){
			tmp->pcb=p->pcb;
		}
		p=p->next;
	}
	return tmp->pcb;
} 
//多级反馈队列 multileved feedback queue
PCB* MFQ(){
	
} 
void run(PCB* pcb){
//	if (pcb==NULL) return ;
	cout<<"进程"<<pcb->name<<"正在运行..."<<endl;
	Sleep(100);
	pcb->CPU++;
}
int checkPCB(PCB* pcb){
	if (pcb->CPU<pcb->need) {
		pcb->flag=1;
		return 0;
	}
	else {
		pcb->flag=2;
		cout<<"进程"<<pcb->name<<"已完成"<<endl;
		cout<<endl;
		return 1;
	}
}
PCB* createProcess(int priority,int start,int need){
	PCB* pcb=new PCB(priority,start,need);
	return pcb;
}
void disNode(node* H){
	node *p=H->next;
	while (p!=NULL){
		dis(p->pcb);
		cout<<endl;
		p=p->next;
	}
}
void RUN(node* H){
	while (1){
		PCB* pcb;
		switch(setting){
			case 0: pcb=PSA(H); break;
			case 1: pcb=SJF(H); break;
			case 2: pcb=FCFS(H); break;
		}
		if (checkPCB(pcb)==1) break;
		
		dis(pcb);
		run(pcb);
		
		checkPCB(pcb);
		pcb->flag=0;
		++time;
	}
}
void set(int &setting){
	cout<<"**********------*********"<<endl;
	cout<<"1.高优先级优先调度算法\t2.先来先服务算法"<<endl;
	cout<<"3.短作业优先算法"<<endl;
	cout<<"**********------*********"<<endl<<endl;
	reSet:
	cout<<"请输入您的选择\n"<<endl;
	int tmp;
	cin>>tmp;
	if (tmp<1||tmp>3) {
		cout<<"请输入1-3的有效数值"<<endl;
		goto reSet;
	}else setting=tmp;
}
void init(node* H){
	H->H=H;
	H->next=NULL;
	H->pcb=NULL;
	H->R=H;
}
int main(){
	node* H=new node();
	init(H);
	while (1){
		cout<<"**********------*********"<<endl;
	cout<<"1.创建进程\t2.设置进程调度算法"<<endl;
	cout<<"**********------*********"<<endl;
	cout<<"请输入您的选择"<<endl;
	int x;
	cin>>x;
	switch(x){
		case 1:
			cout<<"需要创建几个进程?"<<endl;
			int y;
			cin>>y;
			for (int i=0;i<y;++i){
				cout<<"请输入进程优先数"<<endl;
				int priority=i+1;
//				cin>>priority;
				cout<<"请输入预计运行时间"<<endl;
				int need=i;
//				cin>>need;
				H->R->next=new node();
				node* p=H->R->next;
				p->H=H;
				p->next=NULL;
				p->pcb=createProcess(priority,i,need);
				H->R=p;
			}
			break;
		case 2: set(setting); break;
	}
	RUN(H);
	}
	
	return 0;
} 
