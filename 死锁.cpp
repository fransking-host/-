#include <iostream>
using namespace std;
//int maxq[5][3]={
//	{5,5,9},
//	{5,3,6},
//	{4,0,11},
//	{4,2,5},
//	{4,2,4}
//};
//int allocation[5][3]={
//	{2,1,2},
//	{4,0,2},
//	{4,0,5},
//	{2,0,4},
//	{3,1,4}
//};
int maxq[5][3]= {
	{7,5,3},
	{3,2,2},
	{9,0,2},
	{2,2,2},
	{4,3,3}
};
int allocation[5][3]= {
	{0,1,0},
	{2,0,0},
	{3,0,2},
	{2,1,1},
	{0,0,2}
};
struct node {
	int max;
	int need;
	int needTmp;
	int allocation;
	int allocationTmp;
	node* next;
};
struct node2 {
	int num;
	node* p;
	node2* next;
};
int init(node2* head,int &length) {
	cout<<"�����빲�м�������"<<endl;
	int size=5;
	cin>>size;
	cout<<"�������м�����Դ"<<endl;
	int x=3;
	cin>>x;
	length=x;
	node2* p=head;
	for (int i=0; i<size; ++i) {
		p->next=new node2();
		p=p->next;
		p->num=i+1;
		p->p=new node();
//		cout<<endl<<"i:"<<i<<endl;
		node* pp=p->p;
		for (int j=0; j<x; ++j) {
//			cout<<endl<<"j:"<<j<<endl;
			cout<<"���������"<<p->num<<"��Դ"<<j+1<<"���й���Ϣ"<<endl;
			int max,allocation;
			cout<<"������max"<<endl;
			cin>>max;
			cout<<"������allocation"<<endl;
			cin>>allocation;
			pp->next=new node();
			pp=pp->next;
//			pp->max=maxq[i][j];
//			pp->allocation=allocation[i][j];
			pp->max=max;
			pp->allocation=allocation;

//			cout<<"allocation"<<pp->allocation<<" ";
			pp->need=pp->max-pp->allocation;
//			cout<<"need"<<pp->need<<" "<<endl;
			pp->allocationTmp=pp->allocation;
			pp->needTmp=pp->need;
			pp->next=NULL;
		}
		p->next=NULL;
	}
	return size;
}
void dis(node2* head) {
	node2* t=head->next;
	while (t!=NULL) {
		cout<<t->num<<"\t";
		node* tt=t->p->next;
		while (tt!=NULL) {
			cout<<tt->max<<"\t";
			tt=tt->next;
		}

		cout<<endl;
		t=t->next;
	}
}
void run(node2* head,node2* stack[],int size,int length){
	head->num=-1;
	
	int flag[size+1];
	for (int i=0; i<size+1; ++i) {
		flag[i]=0;
	}
	int av[length]= {3,3,2};
//	for (int i=0;i<length;++i){
//		cout<<"������available"<<i+1<<endl;
//		cin>>av[i];
//	}
	
	int top=1;
	stack[0]=head;
	stack[1]=stack[0]->next;
	while (top!=size+1) {
		node2* p=stack[top];
		cout<<"�ߵ�����"<<p->num<<endl;
		int c=1;
		node* tmp=p->p->next;
		for (int i=0; i<length; ++i) {
			if (flag[p->num-1]==1) {
				c=0;
				cout<<"����"<<p->num<<"�Ѿ�������"<<endl;
				break;
			}
			if (tmp->need>av[i]) {
				c=0;
				cout<<"����"<<p->num<<"��Դ"<<i+1<<"����"<<endl;
				break;
			}
			tmp=tmp->next;
		}


		if (c==0) {
			cout<<"����"<<p->num<<"����Ҫ��"<<endl;
			if (p->next==NULL) {
				cout<<"ջ������һ��Ԫ��Ϊ��"<<endl;
				if (top==0) {
					cout<<"topΪ0"<<endl;
					break;
				}
				cout<<"top����0����ת��head0>next"<<endl;
				stack[top]=head->next;
				continue;
			}
			stack[top]=p->next;
			if (stack[top]->num==stack[top-1]->num) {
				cout<<"ò���Ѿ�����һ��ѭ����"<<endl;
				if (top!=0) {
					cout<<"top��Ϊ0"<<endl;
					flag[stack[top]->num-1]=0;
					node* temp=stack[top]->p->next;
					for (int i=0; i<length; ++i) {
						av[i]-=temp->allocationTmp;
						temp=temp->next;
					}
					--top;
				} else {
					cout<<"topΪ0"<<endl;
					break;
				}
			}
		} else {
			cout<<stack[top]->num<<"����Ҫ��"<<endl;

			flag[stack[top]->num-1]=1;
			node* temp=p->p->next;
			for (int i=0; i<length; ++i) {
				av[i]+=temp->allocation;
				temp->need=0;
				temp->allocation=0;
				temp=temp->next;
			}
			cout<<"��ǰ������Դ"<<endl;
			for (int i=0; i<3; ++i) cout<<av[i]<<" ";
			cout<<endl;
			if (p->next!=NULL) stack[++top]=p->next;
			else stack[++top]=head->next;
		}
	}
}
int main() {
//	cout<<"***************"<<endl;
//	cout<<"1.��������������ݵ����뷽ʽ(������������max��allocation����"<<endl;
//	cout<<"***************"<<endl;
	node2* head=new node2();
	int length;
	int size=init(head,length);
	node2* stack[size+1];
	run(head,stack,size,length);
	cout<<"���к�:";
	for (int i=1; i<size+1; ++i) {
		cout<<stack[i]->num<<'\t';
	}
	cout<<endl;
	dis(head);
	return 0;
}
