#include <iostream>
#include <windows.h>
using namespace std;
static int Time = 0;
static long sleepTime = 100;
static int setting = 0;
static int setting1 = 0;
static int finished=0;
static int size=0;
struct PCB{
	int name;
	int priority;
	int start;
	int end;
	int need;
	int CPU;
	int flag;
	int index;
};
//flag状态字，0：等待|阻塞状态 1：就绪状态 2：运行中 3：已完成 
struct node{
	int index;
	node* H;
	node* R;
	PCB* pcb;
	node* next;
};
struct node2{
//	int index2;
	node* n;
	node2* current;
	node2* next;
};
void init1(node* head){
//	head=new node();
	head->H=head;
	head->R=head;
	head->pcb=NULL;
	head->next=NULL;
}
//length表示有几个优先级队列，默认为3 
void init2(node2* head,int length=3){
//	head=new node2();
	node2* p=head;
	int index=1;
	for (int i=0;i<length;++i){
		p->next=new node2();
		p=p->next;
		p->n=new node();
		init1(p->n);
		p->n->index=index;
		++index;
	}
	head->current=head->next;
	p->next=NULL;
}
void dis(node* ready){
	node* p=ready->next;
	while(p!=NULL){
		cout<<"进程"<<p->pcb->name<<endl;
		p=p->next;
	}
}
void disTimeNow(){
	cout<<"***当前时刻:"<<Time<<"*****\n"<<endl;
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
	cout<<"index:"<<pcb->index;
	cout<<endl;
}
PCB* createProcess(int name,int priority,int start,int need){
	PCB* pcb=new PCB();
	pcb->name=name;
	pcb->priority=priority;
	pcb->start=start;
	pcb->end=0;
	pcb->need=need;
	pcb->CPU=0;
	pcb->flag=0; 
	return pcb;
}
void putToBlock(node* block,PCB* pcb){
	node* p=block->R;
	p->next=new node();
	p=p->next;
	p->H=block->H;
	p->H->R=p;
	p->pcb=pcb;
	p->next=NULL;
}
void cleanReady(node* ready){
	node* p=ready->H;
	while (p->next!=NULL){
		int flag=p->next->pcb->flag;
		if (flag==3) {
			node* tmp=p->next;
			p->next=tmp->next;
//			cout<<"进程"<<tmp->pcb->name<<"清除"<<endl;
			if (tmp==ready->R) {
				ready->R=p;
			}
			free(tmp);
		}
		p=p->next;
		if (p==NULL) break;
	}
	cout<<"clean success!"<<endl; 
}
void moveToEnd(node* ready,PCB* pcb){
	cout<<11<<endl;
	PCB* tmp=createProcess(pcb->name,pcb->priority,pcb->start,pcb->need);
	tmp->CPU=pcb->CPU;
	tmp->end=pcb->end;
	tmp->flag=pcb->flag;
	node* p=ready->R;
	cout<<22<<endl;
	p->next=new node();
	p=p->next;
	p->pcb=tmp;
	p->next=NULL;
	ready->R=p;
	pcb->flag=3;
	cleanReady(ready);
	cout<<"进程"<<pcb->name<<"moveToEnd"<<endl; 
}
int getReady(node* block,node* ready){
	node* p=block->H->next;
	int size=0;
	while (p!=NULL){
		if (p->pcb->start==Time){
			++size;
			p->pcb->flag=1;
			p->pcb->index=ready->index;
			cout<<"进程"<<p->pcb->name<<"就绪"<<endl;
			moveToEnd(ready,p->pcb);
		}
		p=p->next;
	}
	return size;
}
void getReady2(node* block,node2* ready2){
	getReady(block,ready2->next->n);
}
void putToFinish(node* ready,node* finish,PCB* pcb){
	++finished;
	PCB* tmp=createProcess(pcb->name,pcb->priority,pcb->start,pcb->need);
	tmp->CPU=pcb->CPU;
	tmp->end=pcb->end;
	tmp->flag=3;
	tmp->start=pcb->start;
	node* p=finish->R;
	p->next=new node();
	p=p->next;
	p->pcb=tmp;
	p->next=NULL;
	finish->R=p;
	cleanReady(ready);
	cout<<pcb->name<<"put to finish success"<<endl;
}
//time控制运行时间，由调度算法决定，默认为1 
//同时在这里更新当前时刻 
void runProcess(node2* ready2,node* ready,node* finish,PCB* pcb,int time=1){
	pcb->flag=2;
	disPCB(pcb);
	cout<<"进程"<<pcb->name<<"运行中"<<endl;
	Sleep(sleepTime*time);
	pcb->CPU++;
	if (pcb->CPU>=pcb->need) {
		cout<<"进程"<<pcb->name<<"已完成"<<endl;
		pcb->flag=3;
		Time+=time;
		pcb->end=Time;
		node2* tmp=ready2->next;
		for (int i=0;i<pcb->index;++i) {
			if (tmp!=NULL) tmp=tmp->next;
		}
//		putToFinish(ready,finish,pcb);
		putToFinish(tmp->n,finish,pcb);
//		cleanReady(ready);
		cleanReady(tmp->n);
		pcb=NULL;
		
		return ;
	}
	pcb->flag=1;
	
	if (pcb->priority>0) pcb->priority--;
//	moveToEnd(ready,pcb);//这是其他三个算法的 
	node2* tmp=ready2->next;
	for (int i=0;i<=pcb->index;++i) {
		if (tmp!=NULL) tmp=tmp->next;
	}
	cout<<"移动index完成"<<endl;
	if (tmp!=NULL) moveToEnd(tmp->n,pcb);
	pcb->index++;
	Time+=time;
}
void start(node* block){
	cout<<"请输入所需创建进程数量"<<endl;
	int length;
	cin>>length;
	size=length;
	for (int i=0;i<length;++i){
		int priority,start,need;
		cout<<"请输入进程"<<i+1<<"的相关信息："<<endl;
		cout<<"请输入优先级"<<endl; 
		//cin>>priority;
		priority=0;
		cout<<"请输入就绪时间"<<endl;
		cin>>start;
		cout<<"请输入需要运行时间"<<endl;
		cin>>need;
		PCB* pcb=createProcess(i+1,priority,start,need);
		cout<<"create success!"<<endl;
		putToBlock(block,pcb);
	}
	cout<<length<<"个进程创建完毕\n"<<endl;
} 
//高优先级优先 priority-scheduling algorithm
PCB* PSA(node* H){
	node* p=H->next;
	node* tmp=new node();
	tmp->pcb=p->pcb;
	while (p!=NULL){
		int a=tmp->pcb->priority;
		int b=p->pcb->priority;
		if (a<b){
			tmp->pcb=p->pcb;
		}
		p=p->next;
	}
	return tmp->pcb->need==999?NULL:tmp->pcb;
} 
//先来先服务 first come first serverd
PCB* FCFS(node* H){
	node* p=H->next;
	cout<<"**"<<endl;
	node* tmp=new node();
	if (p==NULL) return NULL;
	tmp->pcb=p->pcb;
	while (p!=NULL){
		int a=tmp->pcb->start;
		int b=p->pcb->start;
		if (a>b){
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
	tmp->pcb=new PCB();
	tmp->pcb->need=999;
	while (p!=NULL){
		int a=tmp->pcb->need;
		int b=p->pcb->need;
		if (a>b){
			tmp->pcb=p->pcb;
		}
		p=p->next;
	}
	return tmp->pcb->need==999?NULL:tmp->pcb;
} 
static int dui=1;
//多级反馈队列 multileved feedback queue
PCB* MFQ(node2* ready2){
	node2* p=ready2->next;
	cout<<1<<endl;
	node* pp=p->n;
	cout<<2<<endl;
	while (pp->next==NULL&&p!=NULL) {
		p=p->next;
		++dui;
		pp=p->n;
	}
	cout<<"队列"<<dui<<endl;
//	if (pp->R==NULL) 
	PCB* pcb=NULL;
	if (pp->R!=NULL) pcb=FCFS(pp);
	return pcb;
} 

int main(){
	node* block=new node();
	init1(block);
	node* ready1=new node();
	init1(ready1);
	node2* ready2=new node2();
	init2(ready2);
	node* finish=new node();
	init1(finish);
	start(block);
	PCB* run=NULL;
	int se=0;
		cin>>se;
		int Run=0;
		//0326446582
		int tt=0;
		for (int i=0;i<100;++i){
		
				
		disTimeNow();
//		getReady(block,ready1);
		getReady2(block,ready2);
//		if (==0&&ready1->next==NULL) {
//			cout<<"无就绪进程"<<endl;
//			Time++;
//			continue;
//		}
		if (run==NULL||run->flag==3){
			if (run==NULL) cout<<"检索"<<endl;
			else if (run->flag==3) cout<<"3"<<"-----"<<run->name<<endl;
//			if (se==1) run=SJF(ready1);
//			if (se==0) run=FCFS(ready1);
//			if (se==2) run=PSA(ready1);
			run=MFQ(ready2);
		}
		cout<<"MFQsuccess"<<endl; 
		if (run==NULL) {
			++tt;
			if (finished<size) {
				Time++;
				if(tt>10) break;
				continue;
			}
			break;
		}
		runProcess(ready2,ready2->next->n,finish,run);	
		++Run;		
	}
	
	node* p=finish->H->next;
	while (p!=NULL){
		disPCB(p->pcb);
		p=p->next;
	} 
//	while (1){
//		disTimeNow();
//		if (getReady(block,ready1)==0&&ready1->next==NULL) {
//			Time++;
//			continue;
//		}
//		runProcess(ready1,finish,SJF(ready1));		
//	}
	
	return 0;
} 
