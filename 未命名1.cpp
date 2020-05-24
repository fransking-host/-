#include <iostream>
using namespace std;
struct T{
	int data;
};
struct node{
	T* t;
	node* next;
};
int main(){
	T* t=new T();
	t->data=0;
	node* H=new node();
	H->t=t;
	cout<<H->t->data<<endl;
	t->data=1;
	cout<<H->t->data<<endl;
}
