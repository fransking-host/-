#include <iostream>
using namespace std;
struct node{
	int page;
	int start;
	int lastUsed;
	int willUsed;
};
bool compare(node a,node b,int x){
	switch(x){
		case 0:
			if (a.start>b.start) return true;
			return false;
		case 1:
			if(a.lastUsed>b.lastUsed) return true;
			return false;
		case 2:
			if (a.willUsed<b.willUsed) return true;
			return false;
	}
}
int at(int a,node data[],int n){
	for (int i=0;i<n;++i){
		if (a==data[i].page) return i;
	}
	return -1;
}
int FIFO(node data[],int length){
	int index=0;
	for (int i=0;i<length;++i){
		cout<<i<<"****"<<index<<"***"<<data[i].start<<"****ҳ:*****"<<data[i].page<<endl;
		if (compare(data[index],data[i],0)) index=i;
	}
	return index;
}
int LRU(node data[],int length){
	int index=0;
	for (int i=0;i<length;++i){
		cout<<i<<"****"<<index<<"***"<<data[i].lastUsed<<"****ҳ:*****"<<data[i].page<<endl;
		if (compare(data[index],data[i],1)) index=i;
	}
	return index;
}
int OPT(node data[],int length){
	int index=0;
	for (int i=0;i<length;++i){
		cout<<i<<"****"<<index<<"***"<<data[i].willUsed<<"****ҳ:*****"<<data[i].page<<endl;
		if (compare(data[index],data[i],2)) index=i;
	}
	return index;
}
int willUsed(int pages[],int size,int index){
	int re=index;
	for (int i=index+1;i<size;++i){
		if (pages[i]==pages[index]) {
			re=i;
			break;
		}
		else continue;
	}
	return re==index?999:re;
}
int main(){
//	Ҫ����һ���������ڴ�ŷ���ҳ������� 
//	���ڲ�����һ��Ҫ���ʵ�ҳ�漰����LRU�㷨��ʵ�� 
//	size��ʾ�м�����ҳ 
//	int size=3;
//	int a[size];
//	Ҫ����ҳ��x����a�в��ң��Ƿ���ڣ�y������N�����ݸ��㷨�޸�a�б���
//	ʧЧ
	float Y=0,N=0;
	cout<<"�������ܹ�Ҫ�����ҳ����"<<endl;
	int size=19;
//	cin>>size;
	int pages[size]={1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3};
	for (int i=0;i<size;++i){
		cout<<"�������"<<i<<"ҳ"<<endl; 
	}
	int length=3;
	node data[length];
//	��data���г�ʼ����-1����δ�����κ�ҳ�� 
	for (int i=0;i<length;++i){
		data[i].page=-1;
		data[i].start=-1;
		data[i].willUsed=999;
		data[i].lastUsed=-1;
	}
	for (int i=0;i<size;++i){
		int index=0;
		if ((index=at(pages[i],data,length))==-1) {
//			δ�ҵ�pages[i]ҳ�����룬������Ӧ�㷨�޸�data���� 
			 N++;
//			 int da=FIFO(data,length);
//			 int da=LRU(data,length);
			 int da=OPT(data,length);
			 cout<<pages[i]<<"����data��"<<endl;
			 cout<<"��data��"<<data[da].page<<"����"<<pages[i]<<endl<<endl;
			 data[da].page=pages[i];
			 data[da].start=i;
			 data[da].willUsed=willUsed(pages,size,i);
			 data[da].lastUsed=i;
			 continue;
		}
		Y++;
		data[index].lastUsed=i;
		data[index].willUsed=willUsed(pages,size,i);
		cout<<endl<<pages[i]<<"��data��"<<endl<<endl;
	}
	printf("ȱҳ��Ϊ%.2f%%",N/size*100); 
	return 0;
}
