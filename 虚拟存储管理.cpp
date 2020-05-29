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
		cout<<i<<"****"<<index<<"***"<<data[i].start<<"****页:*****"<<data[i].page<<endl;
		if (compare(data[index],data[i],0)) index=i;
	}
	return index;
}
int LRU(node data[],int length){
	int index=0;
	for (int i=0;i<length;++i){
		cout<<i<<"****"<<index<<"***"<<data[i].lastUsed<<"****页:*****"<<data[i].page<<endl;
		if (compare(data[index],data[i],1)) index=i;
	}
	return index;
}
int OPT(node data[],int length){
	int index=0;
	for (int i=0;i<length;++i){
		cout<<i<<"****"<<index<<"***"<<data[i].willUsed<<"****页:*****"<<data[i].page<<endl;
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
//	要定义一个数组用于存放访问页面的序列 
//	用于查找下一次要访问的页面及方便LRU算法的实现 
//	size表示有几个虚页 
//	int size=3;
//	int a[size];
//	要调入页面x，从a中查找，是否存在，y：继续N：根据各算法修改a中表项
//	失效
	float Y=0,N=0;
	cout<<"请输入总共要调入的页面数"<<endl;
	int size=19;
//	cin>>size;
	int pages[size]={1,2,3,4,2,1,5,6,2,1,2,3,7,6,3,2,1,2,3};
	for (int i=0;i<size;++i){
		cout<<"请输入第"<<i<<"页"<<endl; 
	}
	int length=3;
	node data[length];
//	对data进行初始化，-1代表未调入任何页面 
	for (int i=0;i<length;++i){
		data[i].page=-1;
		data[i].start=-1;
		data[i].willUsed=999;
		data[i].lastUsed=-1;
	}
	for (int i=0;i<size;++i){
		int index=0;
		if ((index=at(pages[i],data,length))==-1) {
//			未找到pages[i]页，调入，根据相应算法修改data表项 
			 N++;
//			 int da=FIFO(data,length);
//			 int da=LRU(data,length);
			 int da=OPT(data,length);
			 cout<<pages[i]<<"不在data中"<<endl;
			 cout<<"往data中"<<data[da].page<<"调入"<<pages[i]<<endl<<endl;
			 data[da].page=pages[i];
			 data[da].start=i;
			 data[da].willUsed=willUsed(pages,size,i);
			 data[da].lastUsed=i;
			 continue;
		}
		Y++;
		data[index].lastUsed=i;
		data[index].willUsed=willUsed(pages,size,i);
		cout<<endl<<pages[i]<<"在data中"<<endl<<endl;
	}
	printf("缺页率为%.2f%%",N/size*100); 
	return 0;
}
