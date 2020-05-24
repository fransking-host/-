#include <iostream>
#include <malloc.h>
#define SIZE 1000
using namespace std;
/*Table�ṹ���ڱ����¼�����ڴ�����Ϣ��back��nextָ����һ������һ�飬
addr��¼���ڴ����׵�ַ��sizeΪ��С��flag��¼���ڴ���Ƿ�ռ�ã�h��ָ����Զ��ͷ��㡣
����hΪ�ֽ�㣬h֮ǰ���ӵ��ǿ����ڴ�飬h�Ժ�����ӵ�Ϊ��ʹ�õ��ڴ��*/
//ע��������δʵ�ָ��㷨��������ĳ���㷨��ֻ���ܽ�Ϊ������ʵ���ڴ�ķ��������
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
			cout<<"���Ժϲ�"<<endl;
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
//	�����ǰ�����ڵ��Ӧ�Ŀ��з����������󣬾��޸ı��� ��������ǰ���ҿ��з�����ֱ�������Բ��ܷ����򷵻�һ�� false
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
				cout<<"�ڴ���ȫ���������"<<endl<<endl;
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
//	cout<<"�������\n";
	remove(T);
//	cout<<"������\n";
	
	Table* q=new Table();
	q=tmp->H->back;
	tmp->back=q;
	if (q!=NULL) {
		q->next=tmp;
	}
	tmp->H->back=tmp;
	tmp->next=tmp->H;
	cout<<"�ͷ����\n\n";
	sortEmpty(tmp->H);
}
//����size����Ҫ�ͷŵ��ڴ�� 
Table* at(int size,Table* T){
	Table* h=T->H->next;
	while (h!=NULL){
		if (h->size==size) return h;
		h=h->next;
	}
	return NULL;
}

//��ӡ���з�����δ���� 
void disEmpty(Table* T){
	cout<<"*------------*"<<endl;
	cout<<"���з�����"<<endl;
	while(T!=NULL){
		
		if (T->back!=NULL) {
			char* a=T->addr;
			char* b=T->back->addr;
			if (a+T->size==b||b+T->back->size==a) {
				cout<<"���Ժϲ�"<<endl;
				T->addr=a>=b?b:a;
				T->size+=T->back->size;
				remove(T->back);
			}
//			sortEmpty(T);
		}
		cout<<"�׵�ַ��"<<&(T->addr)<<"*****"<<"��С��"<<T->size<<endl;
		T=T->back;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disFull(Table* T){
	Table* p=T->H->next;
	if (p==NULL) {
		cout<<"����������ڴ����\n\n";
		return ;
	}
	cout<<"*------------*"<<endl;
	cout<<"��������ڴ������\n";
	while (p!=NULL){
		cout<<"�׵�ַ��"<<&(p->addr)<<"*****"<<"��С��"<<p->size<<endl;
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disAll(Table* T){
	Table* p=T->H;
	while (p->back) p=p->back;
	cout<<"/**********/"<<endl;
	cout<<"�ܷ�����"<<endl;
	while (p!=NULL){
		cout<<"�׵�ַ��"<<&(p->addr)<<"*****"<<"��С��"<<p->size;
		if (p==p->H) cout<<"����head";
		cout<<endl;
		p=p->next;
	}
	cout<<"/**********/"<<endl<<endl;
}
//ѭ���״���Ӧ�㷨:�����ǽ������ڴ水��ַ����,ֻҪ���ҵ����ڴ��������Ҫ�ͷ��䣬����back 
Table* recycle(Table* T,int size){
	Table* tmp=T;
	Table* p=T->H->current;
	while (p!=NULL){
		if ((p->size>=size)&&(tmp->addr>p->addr)){
			tmp=p;
		}
		p=p->back;
	}
	cout<<"���ηָ���ڴ����"<<tmp->size<<endl;
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
	cout<<"���ηָ���ڴ����"<<tmp->size<<endl;
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
	cout<<"���ηָ���ڴ����"<<tmp->size<<endl;
	return tmp;
}

void realMalloc(Table* head,char* p){
		bool flag=true;
		cout<<"������Ҫ������ڴ�"<<endl;
		int x;
		cin>>x;
		if (x==0) {
			cout<<"���������0����ֵ"<<endl;
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
	cout<<"������Ҫ�ͷŵ��ڴ�"<<endl;
		int freeSize;
		cin>>freeSize;
		Table* f=at(freeSize,head);
		cout<<"��ѯ�����ڴ����Ϣ"<<endl;
		if (f!=NULL){
			cout<<"�׵�ַ��"<<&(f->addr)<<"*****"<<"��С��"<<f->size<<endl;
			lfree(f);
			disEmpty(head);
			head->H->size+=f->size;
		}else {
			cout<<"���ڴ治����"<<endl;
			disFull(head);
		}
}
void set(int &setting){
	
	cout<<"**********------*********"<<endl;
	cout<<"1.ѭ���״���Ӧ�㷨\t2.�����Ӧ�㷨"<<endl;
	cout<<"3.���Ӧ�㷨"<<endl;
	cout<<"**********------*********"<<endl<<endl;
	reSet:
	cout<<"����������ѡ��\n"<<endl;
	int tmp;
	cin>>tmp;
	if (tmp<1||tmp>3) {
		cout<<"������1-3����Ч��ֵ"<<endl;
		goto reSet;
	}else setting=tmp;
}

int main(){
	int size=SIZE;
	Table* head=new Table();
	char* p=(char*) malloc(size);
	cout<<"�����ڴ���׵�ַ"<<&p<<"*****"<<endl;
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
		cout<<"1.�����ڴ�"<<"\t\t"<<"2.�ͷ��ڴ�\n";
		cout<<"3.��ӡ�����ڴ������"<<"\t"<<"4.��ӡ��������ڴ������\n";
		cout<<"5.�����ڴ�����㷨\n";
		cout<<"**********------*********"<<endl<<endl;
		cout<<"������Ҫִ�еĲ���"<<endl;
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
				cout<<"������1-5����Ч��ֵ\n"<<endl; 
				
		}
	}
	return 0;
}
