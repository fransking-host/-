#include <iostream>
#include <malloc.h>
using namespace std;
#define SIZE 1000
static int setting=1;
struct Table{
	char* addr;
	int logicAddr;
	int size;
	int flag;
	Table* H;
	Table* current;
	Table* back;
	Table* next;
	int capacity;
	int max; 
	Table();
};
Table::Table(){
	flag=0;
}
Table* init(int size){
	Table* T=new Table();
	T->addr=(char*) malloc(size);
	T->logicAddr=0;
	T->size=size;
	T->H=T;
	T->current=T;
	T->back=NULL;
	T->next=NULL;
	T->capacity=size;
	T->max=size;
	return T;
}
int getMax(Table* T){
	Table* p=T->H;
	int max=0;
	while (p!=NULL){
		if (p->flag==0&&max<p->size) max=p->size;
		p=p->next;
	}
	return max;
}
void disNodes(Table* T){
	cout<<"首地址为；"<<&T->addr<<"\t逻辑地址："<<T->logicAddr<<"\t大小为:"<<T->size<<"\t状态:";
	if (T->flag==1) cout<<"已";
	else cout<<"未";
	cout<<"占用\n";
}
void remove(Table* T){
	Table* p=T->back;
	Table* q=T->next;
	if (p!=NULL) p->next=q;
	if (q!=NULL) q->back=p;
	disNodes(T);
	free(T);
	cout<<"删除节点完成\n"<<endl;
}
Table* at(int logicAddr,int size,Table* T){
	Table* h=T->H->next;
	while (h!=NULL){
		if (h->logicAddr==logicAddr&&h->size==size) return h;
		h=h->next;
	}
	return NULL;
}
void lmalloc(Table* T,int size){
	if (T==NULL){
		if (T->H->capacity==0) cout<<"内存已分配完"<<endl;
		else cout<<"未找到合适的内存块供分配"<<endl;
		return ;
	}
	Table* tmp=T->next;
	T->next=new Table();
	Table* p=T->next;
	p->addr=T->addr+size;
	p->logicAddr=T->logicAddr+size;
	p->size=T->size-size;
	p->H=T->H;
	p->H->current=p;
	p->back=T;
	p->next=tmp;
	T->size=size;
	T->flag=1;
	T->H->capacity-=size;
	if (T!=T->H&&T->size==0) remove(T);
}
void merges(Table* p,Table* q){
	if (p!=NULL&&q!=NULL){
		if (p->flag+q->flag==0){
			p->size+=q->size;
			p->flag=0;
			remove(q);
			cout<<"前一个节点：";disNodes(p);
			cout<<"后一个节点: ";disNodes(q);
			cout<<"合并完成\n"<<endl; 
		}
		
	}
}
void lfree(Table* T){
	T->flag=0;
	T->H->capacity+=T->size;
	merges(T,T->next);
	merges(T->back,T);
}
void dis(Table* T){
	Table* p=T->H;
	cout<<"*------------*"<<endl;
	cout<<"<内存分区表>"<<endl;
	while (p!=NULL){
		disNodes(p);
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disEmpty(Table* T){
	Table* p=T->H;
	cout<<"*------------*"<<endl;
	cout<<"<空闲分区表>"<<endl;
	while (p!=NULL){
		if (p->flag==0) disNodes(p);
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
}
void set(int &setting){
	cout<<"**********------*********"<<endl;
	cout<<"1.循环首次适应算法\t2.最佳适应算法"<<endl;
	cout<<"3.最坏适应算法"<<endl;
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
//循环首次适应算法:本质是将空闲内存按地址排序,只要查找到的内存块满足需要就分配，否则next
Table* recycle(Table* T,int size){
	Table* temp=T->H->current;
	Table* tmp=NULL;
	Table* p=T->H->current;
	if (p==NULL) return NULL;
	int flag=0;
	re:
	while (p!=NULL){
		if ((p->flag==0)&&(p->size>=size)){
			tmp=p;
			break;
		}
		p=p->next;
	}
	if (p==NULL&&flag==0){
		++flag;
		p=T->H;
		goto re;
	}
	cout<<"本次分割的内存块是"<<tmp->size<<endl;
	return tmp;
}
Table* best(Table* T,int size){
	T->H->current=T->H;
	Table* p=T->H;
	Table* tmp=p;
	while (p!=NULL){
		if ((p->flag==0)&&(p->size>=size)&&(tmp->size>p->size)) tmp=p;
		p=p->next;
	}
	cout<<"本次分割的内存块是"<<tmp->size<<endl;
	return tmp;
}
Table* worst(Table* T,int size){
	Table* p=T->H;
	Table* tmp=p;
	while (p!=NULL){
		if ((p->flag==0)&&(p->size>=size)&&(tmp->size<p->size)) tmp=p;
		p=p->next;
	}
	cout<<"本次分割的内存块是"<<tmp->size<<endl;
	return tmp;
}

void realMalloc(Table* head,char* p){
		bool flag=true;
		cout<<"请输入要申请的内存"<<endl;
		int x;
		cin>>x;
		if (x==0) {
			cout<<"请输入大于0的数值"<<endl;
			return ;
		}
		if (x>head->max){
			cout<<"未找到合适的内存块供分配"<<endl;
			return ;
		}
		switch(setting){
			case 1:
				lmalloc(recycle(head,x),x); break;
			case 2:
				lmalloc(best(head,x),x); break;
			case 3:
				lmalloc(worst(head,x),x); break;
		}
//		head->H->capacity-=x;
}
void start(Table* head){
		cout<<endl;
		cout<<"**********------*********"<<endl;
		cout<<"---当前最大空闲内存块大小为:"<<getMax(head)<<"---\n";
		cout<<"当前可用内存共:"<<head->H->capacity<<endl;
		cout<<"current:"<<head->H->current->size<<endl;
		cout<<"1.申请内存"<<"\t\t"<<"2.释放内存\n";
		cout<<"3.打印空闲内存分区表"<<"\t"<<"4.打印内存分区表\n";
		cout<<"5.设置内存分配算法\t6.初始化\n";
		cout<<"**********------*********"<<endl<<endl;
		cout<<"请输入要执行的操作"<<endl;
		int x;
		cin>>x;
		int size;
		switch(x){
			case 1:
				realMalloc(head,head->addr); break;
			case 2:
				cout<<"请输入要释放的内存块的逻辑地址"<<endl; 
				int logicAddr;
				cin>>logicAddr;
				cout<<"请输入要释放的内存块大小"<<endl;
				cin>>size;
				lfree(at(logicAddr,size,head)); break;
			case 3:
				disEmpty(head); break;
			case 4:
				dis(head); break;
			case 5:
				set(setting); break;
			case 6:
				cout<<"请输入本次测试总共要申请的内存的大小\n输入0则默认为1000字节"<<endl;
				cin>>size;
				head=init(size); break;
			default:
				cout<<"请输入1-6的有效数值\n"<<endl; 		
		}
}

int main(){
	int size=0;
	cout<<"请输入本次测试总共要申请的内存的大小\n输入0则默认为1000字节"<<endl;
	Table* H;
	cin>>size;
	if (size!=0) H=init(size);
	else {
		cout<<"输入为0，采用默认值初始化\n";
		H=init(SIZE);
	}
	cout<<"初始化完成\n"<<endl;
	cout<<"申请多少内存"<<endl;
	while(1){
		start(H);
	}
	return 0;
}
