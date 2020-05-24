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
	cout<<"�׵�ַΪ��"<<&T->addr<<"\t�߼���ַ��"<<T->logicAddr<<"\t��СΪ:"<<T->size<<"\t״̬:";
	if (T->flag==1) cout<<"��";
	else cout<<"δ";
	cout<<"ռ��\n";
}
void remove(Table* T){
	Table* p=T->back;
	Table* q=T->next;
	if (p!=NULL) p->next=q;
	if (q!=NULL) q->back=p;
	disNodes(T);
	free(T);
	cout<<"ɾ���ڵ����\n"<<endl;
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
		if (T->H->capacity==0) cout<<"�ڴ��ѷ�����"<<endl;
		else cout<<"δ�ҵ����ʵ��ڴ�鹩����"<<endl;
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
			cout<<"ǰһ���ڵ㣺";disNodes(p);
			cout<<"��һ���ڵ�: ";disNodes(q);
			cout<<"�ϲ����\n"<<endl; 
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
	cout<<"<�ڴ������>"<<endl;
	while (p!=NULL){
		disNodes(p);
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
}
void disEmpty(Table* T){
	Table* p=T->H;
	cout<<"*------------*"<<endl;
	cout<<"<���з�����>"<<endl;
	while (p!=NULL){
		if (p->flag==0) disNodes(p);
		p=p->next;
	}
	cout<<"*------------*"<<endl<<endl;
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
//ѭ���״���Ӧ�㷨:�����ǽ������ڴ水��ַ����,ֻҪ���ҵ����ڴ��������Ҫ�ͷ��䣬����next
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
	cout<<"���ηָ���ڴ����"<<tmp->size<<endl;
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
	cout<<"���ηָ���ڴ����"<<tmp->size<<endl;
	return tmp;
}
Table* worst(Table* T,int size){
	Table* p=T->H;
	Table* tmp=p;
	while (p!=NULL){
		if ((p->flag==0)&&(p->size>=size)&&(tmp->size<p->size)) tmp=p;
		p=p->next;
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
		if (x>head->max){
			cout<<"δ�ҵ����ʵ��ڴ�鹩����"<<endl;
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
		cout<<"---��ǰ�������ڴ���СΪ:"<<getMax(head)<<"---\n";
		cout<<"��ǰ�����ڴ湲:"<<head->H->capacity<<endl;
		cout<<"current:"<<head->H->current->size<<endl;
		cout<<"1.�����ڴ�"<<"\t\t"<<"2.�ͷ��ڴ�\n";
		cout<<"3.��ӡ�����ڴ������"<<"\t"<<"4.��ӡ�ڴ������\n";
		cout<<"5.�����ڴ�����㷨\t6.��ʼ��\n";
		cout<<"**********------*********"<<endl<<endl;
		cout<<"������Ҫִ�еĲ���"<<endl;
		int x;
		cin>>x;
		int size;
		switch(x){
			case 1:
				realMalloc(head,head->addr); break;
			case 2:
				cout<<"������Ҫ�ͷŵ��ڴ����߼���ַ"<<endl; 
				int logicAddr;
				cin>>logicAddr;
				cout<<"������Ҫ�ͷŵ��ڴ���С"<<endl;
				cin>>size;
				lfree(at(logicAddr,size,head)); break;
			case 3:
				disEmpty(head); break;
			case 4:
				dis(head); break;
			case 5:
				set(setting); break;
			case 6:
				cout<<"�����뱾�β����ܹ�Ҫ������ڴ�Ĵ�С\n����0��Ĭ��Ϊ1000�ֽ�"<<endl;
				cin>>size;
				head=init(size); break;
			default:
				cout<<"������1-6����Ч��ֵ\n"<<endl; 		
		}
}

int main(){
	int size=0;
	cout<<"�����뱾�β����ܹ�Ҫ������ڴ�Ĵ�С\n����0��Ĭ��Ϊ1000�ֽ�"<<endl;
	Table* H;
	cin>>size;
	if (size!=0) H=init(size);
	else {
		cout<<"����Ϊ0������Ĭ��ֵ��ʼ��\n";
		H=init(SIZE);
	}
	cout<<"��ʼ�����\n"<<endl;
	cout<<"��������ڴ�"<<endl;
	while(1){
		start(H);
	}
	return 0;
}
