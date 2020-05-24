#include <iostream>
#include <malloc.h>
#define SIZE 1000
using namespace std;
/*Table结构用于保存记录各个内存块的信息，back，next指向上一块与下一块，
addr记录该内存块的首地址，size为大小，flag记录该内存块是否被占用，h则指向永远的头结点。
即以h为分界点，h之前链接的是空闲内存块，h以后的链接的为在使用的内存块*/
//注：本程序未实现各算法，可能是某个算法，只是能较为正常的实现内存的分配与回收
// 
static int setting=1;
struct Table{
	Table* back;
	Table* next;
	char* addr;
	int size;
	int capacity;
	int flag;
	Table* H;
	Table* current;
};


void remove(Table* T){
	if (T!=NULL){
		Table* p=T->back;
		Table* q=T->next;
//		cout<<"removing"<<endl; 
		if (p!=NULL) {	
			p->next=q;
		}
		
		if (q!=NULL) {
			q->back=p;
		}
		free(T);
	}	
}
void swapNodes(Table* &a,Table* &b){
	char* tmp=a->addr;
	int size=a->size;
	a->addr=b->addr;
	a->size=b->size;
	b->addr=tmp;
	b->size=size; 
}
void sortEmpty(Table* T){
	reSort:
	Table* p=T->H->back; 
	int flag=0;
	while (p!=NULL){
		Table* q=p->back;
		if (q==NULL) {
			break;
		}
		if (p->addr>q->addr) {
			swapNodes(q,p);
			++flag;
		}
		char* a=p->addr;
		char* b=q->addr;
		if (a+p->size==b||b+q->size==a) {
			cout<<"可以合并"<<endl;
			p->addr=a>=b?b:a;
			p->size+=q->size;
			remove(q);
			++flag;
		}
		p=p->back;
		
	}
	cout<<2<<endl;
	if (flag>0) goto reSort;
	
}
bool lmalloc(int size,Table* T,char* &addr){
//	如果当前操作节点对应的空闲分区满足需求，就修改表项 。否则往前查找空闲分区，直到结束仍不能分配则返回一个 false
	if (T->size>=size) {
		if (T->size==size){ 
			Table* tmp=new Table();
			tmp->next=T->H->next;
			if (T->H->next!=NULL) T->H->next->back=tmp;
			tmp->back=T->H;
			T->H->next=tmp;
			tmp->addr=T->addr;
			tmp->size=size;
			tmp->flag=1;
			tmp->H=T->H;
			addr=T->addr;
			if (T->back!=NULL){
				T->size=T->back->size;
				T->addr=T->back->addr;
			}
			else {
				T->size=0;
				cout<<"内存已全部分配完毕"<<endl<<endl;
			}
			T->flag=0;
			remove(T->back);
			return true; 
		}
		Table* p=new Table();
		p->addr=T->addr;
		p->size=size;
		p->next=T->H->next;
		if (T->H->next!=NULL) T->H->next->back=p;
		p->back=T->H;
		T->H->next=p;
		p->flag=1;
		p->H=T->H;
		T->size-=size;
		char* ad;
		ad=T->addr+size;
		T->addr=ad;
		return true; 
	}
	else return false;
}

bool lfree(Table* T){
	Table* tmp=new Table();
	tmp->H=T->H;
	tmp->addr=T->addr;
	tmp->size=T->size;
	tmp->flag=0;
//	cout<<"复制完成\n";
	remove(T);
//	cout<<"清除完成\n";
	
	Table* q=new Table();
	q=tmp->H->back;
	tmp->back=q;
	if (q!=NULL) {
		q->next=tmp;
	}
	tmp->H->back=tmp;
	tmp->next=tmp->H;
	cout<<"释放完成\n\n";
	sortEmpty(tmp->H);
}
//根据size查找要释放的内存块 
Table* at(int size,Table* T){
	Table* h=T->H->next;
	while (h!=NULL){
		if (h->size==size) return h;
		h=h->next;
	}
	return NULL;
}

//打印空闲分区表，未排序 
void disEmpty(Table* T){
	cout<<"*------------*"<<endl;
	cout<<"空闲分区表"<<endl;
	while(T!=NULL){
		
		if (T->back!=NULL) {
			char* a=T->addr;
			char* b=T->back->addr;
			if (a+T->size==b||b+T->back->size==a) {
				cout<<"可以合并"<<endl;
				T->addr=a>=b?b:a;
				T->size+=T->back->size;
				remove(T->back);
			}
//			sortEmpty(T);
		}
		cout<<"首地址："<<&(T->addr)<<"*****"<<"大小："<<T->size<<endl;
		T=T->back;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disFull(Table* T){
	Table* p=T->H->next;
	if (p==NULL) {
		cout<<"无已申请的内存分区\n\n";
		return ;
	}
	cout<<"*------------*"<<endl;
	cout<<"已申请的内存分区表\n";
	while (p!=NULL){
		cout<<"首地址："<<&(p->addr)<<"*****"<<"大小："<<p->size<<endl;
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disAll(Table* T){
	Table* p=T->H;
	while (p->back) p=p->back;
	cout<<"/**********/"<<endl;
	cout<<"总分区表"<<endl;
	while (p!=NULL){
		cout<<"首地址："<<&(p->addr)<<"*****"<<"大小："<<p->size;
		if (p==p->H) cout<<"我是head";
		cout<<endl;
		p=p->next;
	}
	cout<<"/**********/"<<endl<<endl;
}
//循环首次适应算法:本质是将空闲内存按地址排序,只要查找到的内存块满足需要就分配，否则back 
Table* recycle(Table* T,int size){
	Table* tmp=T;
	Table* p=T->H->current;
	while (p!=NULL){
		if ((p->size>=size)&&(tmp->addr>p->addr)){
			tmp=p;
		}
		p=p->back;
	}
	cout<<"本次分割的内存块是"<<tmp->size<<endl;
	if (T->back==NULL){
		T->H->current=T->H;
	}
	else T->H->current=T->back;
	return tmp;
}
Table* best(Table* T,int size){
	T->H->current=T->H;
	Table* p=T->H;
	Table* tmp=p;
	while (p!=NULL){
		if ((p->size>=size)&&(tmp->size>p->size)) tmp=p;
		p=p->back;
	}
	cout<<"本次分割的内存块是"<<tmp->size<<endl;
	return tmp;
}
Table* worst(Table* T,int size){
	T->H->current=T->H;
	Table* p=T->H;
	Table* tmp=p;
	while (p!=NULL){
		if ((p->size>=size)&&(tmp->size<p->size)) tmp=p;
		p=p->back;
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
		switch(setting){
			case 1:
				lmalloc(x,recycle(head,x),p); break;
			case 2:
				lmalloc(x,best(head,x),p); break;
			case 3:
				lmalloc(x,worst(head,x),p); break;
		}
		head->H->capacity-=x;
}
void realFree(Table* head){
	cout<<"请输入要释放的内存"<<endl;
		int freeSize;
		cin>>freeSize;
		Table* f=at(freeSize,head);
		cout<<"查询到的内存块信息"<<endl;
		if (f!=NULL){
			cout<<"首地址："<<&(f->addr)<<"*****"<<"大小："<<f->size<<endl;
			lfree(f);
			disEmpty(head);
			head->H->size+=f->size;
		}else {
			cout<<"该内存不存在"<<endl;
			disFull(head);
		}
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

int main(){
	int size=SIZE;
	Table* head=new Table();
	char* p=(char*) malloc(size);
	cout<<"申请内存的首地址"<<&p<<"*****"<<endl;
	head->addr=p;
	head->size=size;
	head->capacity=size;
	head->back=NULL;
	head->next=NULL;
	head->flag=0;
	head->H=head;
	head->current=head;
	while(1){
		cout<<endl;
		cout<<"**********------*********"<<endl;
		cout<<"1.申请内存"<<"\t\t"<<"2.释放内存\n";
		cout<<"3.打印空闲内存分区表"<<"\t"<<"4.打印已申请的内存分区表\n";
		cout<<"5.设置内存分配算法\n";
		cout<<"**********------*********"<<endl<<endl;
		cout<<"请输入要执行的操作"<<endl;
		int x;
		cin>>x;
		switch(x){
			case 1:
				realMalloc(head,p); break;
			case 2:
				realFree(head); break;
			case 3:
				disEmpty(head); break;
			case 4:
				disFull(head); break;
			case 5:
				set(setting); break;
			default:
				cout<<"请输入1-5的有效数值\n"<<endl; 
				
		}
	}
	return 0;
}
