#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

class Data {//smallest unit, hold one line of data with all its features
private:
	vector<string> features;
	int numOfFeatures;
public:
	Data(vector<string> newthing);
	~Data() { }
	string getFeature(int num);// read one particular feature from the data, use int as index
  	void printData();//print the Data in format: feature0, feature2, feature3 ...
};

Data::Data(vector<string> newthing) {
	if (!newthing.empty()){
		numOfFeatures = newthing.size();
		for (vector<string>::iterator it = newthing.begin() ; it != newthing.end(); ++it) {
			features.push_back(*it);
		}
	}
}

string Data::getFeature(int num){
	if (num < numOfFeatures && num >= 0) return features[num];
	return "ERROR";
}

void Data::printData(){
	if(numOfFeatures > 0) {
		//cout<<"num Of Features = "<<numOfFeatures<<endl;
		for (int i = 0; i < numOfFeatures; ++i) {
			cout<<features[i];
			if (i == numOfFeatures-1) break; // do not print the last ", " in final loop
			cout<<", ";
		}
		cout<<endl;
	} else {
		cout<<"EMPTY DATA, NOTHING TO PRINT"<<endl;
	}
}

class Dataset {//multiple units of Data stored in a Dataset
private:
	vector<Data> dataset;
	int numOfData;
public:
	Dataset();
  	~Dataset() { }
  	int addData(Data newthing);//add an unit of Data, return 1 if success; 0 if failed
  	Data getData(int num);//access a paticular data, mayby not needed, vector can just use [index] to access
  	void printDataset();
};

Dataset::Dataset() {
	//vector<Data> dataset;
}

int Dataset::addData(Data newthing) {
	try {
		dataset.push_back(newthing);
		numOfData = dataset.size();
		return 1;
	}
	catch (...) {
		cout<<"ERROR adding Data to Dataset"<<endl;
		return 0;
	}
}

Data Dataset::getData(int num){
	if (num < numOfData && num >= 0) return dataset[num];
}

void Dataset::printDataset(){
	if(numOfData > 0) {
		cout<<"Printing all data\n""0     1     2\n";
		for (int i = 0; i < numOfData; ++i) {
			dataset[i].printData();
		}
	} else {
		cout<<"EMPTY DATA SET"<<endl;
	}
}

// Tree implementation
class Tree {
	private:
	  Dataset data;
	  Tree *leftptr, *rightptr;
 
	public:
	  Tree(Dataset newthing, Tree* L, Tree* R);  
	  ~Tree() { }
	  Dataset RootData() { return data; }  
	  Tree* Left() { return leftptr; }
	  Tree* Right() { return rightptr; }
};

Tree::Tree(Dataset newthing, Tree* L, Tree* R) {
	  data = newthing;
	  leftptr = L;
	  rightptr = R;
}

struct Node {
	Tree* data;
	Node *next;
};

class Stack {
	private:                       
		Node *listpointer;
	public:                       
		Stack();
		~Stack();
		void Push(Tree* newthing);
		void Pop();
		Tree* Top();
		bool isEmpty();
};

Stack::Stack() {
   listpointer = NULL;
}

Stack::~Stack() {
}

void Stack::Push(Tree* newthing) {
	Node *temp;
   temp = new Node;    
   temp->data = newthing;
   temp->next = listpointer; 
   listpointer = temp;
}
void Stack::Pop() {
	Node *p;
   p = listpointer;
   if (listpointer != NULL) {   
     listpointer = listpointer->next;
     delete p;               
  }
}

Tree* Stack::Top() {
  return listpointer->data;
}

bool Stack::isEmpty() {
  if (listpointer == NULL) {
     return true;
  }
  return false;
}

Stack S;
Tree *T1, *T2, *mytree;

//Function_ID3(Samples,Classes,Features) 

int main(){
	vector<string> a;
	Dataset data, dsRoot, dsT, dsS;
	string token [100];
	ifstream input_file;
    input_file.open("data.txt");
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    while(!input_file.eof()) {  		
    	input_file>>token[0]>>token[1]>>token[2];
    	a.push_back(token[0]);
    	a.push_back(token[1]);
    	a.push_back(token[2]);
    	dsRoot.addData(a);
    	
    	if(a[1] == "tall"){
    		cout<<"This man is tall\n";
    		dsT.addData(a);
    	}
    	else if(a[1] == "short"){
    		cout<<"This man is short\n";
    		dsS.addData(a);
    	}
    	else{
    		cout<<"Error!\n";
    	}
    	a.clear(); 
    	S.Push(new Tree(dsT,NULL,NULL));
    	S.Push(new Tree(dsS,NULL,NULL));
    	T1=S.Top();
		S.Pop();
		T2=S.Top();
		S.Pop();
    	S.Push(new Tree(dsRoot,T2,T1));
    }
	input_file.close();
	S.Top()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->RootData().printDataset();
}