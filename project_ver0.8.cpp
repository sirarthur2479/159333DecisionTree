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

class Feature {
private:
	string featureName;
	vector<string> options;
public:
	Feature(string name);
	~Feature() {}
	string getFeatureName() { return featureName; }
	void setFeatureName(string name) { featureName = name; }
	vector<string> getOptions() { if (options.size() != 0) return options;}
	void addOption(string newOption) { options.push_back(newOption); }
	void printFeature();
};
Feature::Feature(string name) {
	setFeatureName(name);
}

void Feature::printFeature() {
	cout<<featureName<<":\n";
	for (int i = 0; i < options.size(); ++i) {
    	cout<<options[i]<<" || ";
    }
    cout<<endl;
}

class FeatureSet {
private:
	vector<Feature> data;
public:
	FeatureSet() {}
	~FeatureSet() {}
	void addFeature(Feature newFeature) { data.push_back(newFeature); }
	vector<Feature> getFeatureSet(){ return data; }
	void addOption(string featureName, string newOption);
	void printFeatureSet();
};

void FeatureSet::addOption(string featureName, string newOption) {
	for (int i = 0; i < data.size(); ++i) {
    	if (featureName == data[i].getFeatureName()) {
    		data[i].addOption(newOption);
    		break;
    	}
    }
}

void FeatureSet::printFeatureSet(){
	for (int i = 0; i < data.size(); ++i) {
		data[i].printFeature();
	}
}
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
  	int size(){ return dataset.size();}
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
	  bool endNode;
	  string label;
	  Tree *leftptr, *rightptr;
 
	public:
	  Tree(Dataset newthing, Tree* L, Tree* R);  
	  ~Tree() { }
	  Dataset RootData() { return data; }  
	  Tree* Left() { return leftptr; }
	  Tree* Right() { return rightptr; }
	  bool isEndNode() { return endNode; }
	  void setEndNode(bool temp) { endNode = temp; }
	  string getLabel() { return label; }
	  void setLabel(string labelName) { label = labelName; }
};

Tree::Tree(Dataset newthing, Tree* L, Tree* R) {
	  data = newthing;
	  leftptr = L;
	  rightptr = R;
	  endNode = false;
	  label = "empty";
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
Tree *T1, *T2, *T3, *T4,*mytree;

//Function_ID3(Samples,Classes,Features) 


vector<Dataset> datasetSet;


float Entropy(Dataset set){ // a is the answer of the features
	int NP = set.size();
	int NY=0, NN=0;
	for (int i = 0; i < NP; ++i) {
				if(set.getData(i).getFeature(0) == "yes"){
				NY++;
			}
	}
	NN=NP-NY;
	float temp1=log2((float)NY/(float)NP);
	float temp2=log2((float)NN/(float)NP);
	
	if (NY == 0) temp1=0;
	if (NN == 0) temp2=0;
	return -( ((float)NY/(float)NP) * temp1+ ((float)NN/(float)NP) * temp2 );
}

float Gain(Dataset set, Dataset set_one, Dataset set_two){
	float NP = set.size();
	float NT = set_one.size();
	float NS = set_two.size();
	float NTP = NT/NP;
	float NSP = NS/NP;
	return (Entropy(set) - NSP * Entropy(set_two) - NTP * Entropy(set_one));
}

void addFeatureFunction(FeatureSet& set, string featureName){
	Feature tempFeature(featureName);
	set.addFeature(tempFeature);
}

int main(){
	vector<string> a;
	Dataset data, dsRoot, dsT, dsS, dsTG, dsTB, dsSG, dsSB;// Root_dataset, Tall_dataset, Short_dataset, TallandGood_dataset, TallandBad_dataset, ShortandGood_dataset, ShortandBad_dataset
	Dataset dsG,dsB,dsGT,dsGS,dsBT,dsBS;
	string token [100];
	ifstream input_file;

	
	FeatureSet fs;
	addFeatureFunction(fs, "result");
	addFeatureFunction(fs, "height");
	addFeatureFunction(fs, "dex");
	
	fs.addOption("result", "yes");
	fs.addOption("result", "no");
	fs.addOption("height", "tall");
	fs.addOption("height", "short");
	fs.addOption("dex", "good");
	fs.addOption("dex", "bad");
	
	fs.printFeatureSet();
	
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
    	a.clear(); 
    }
    input_file.close();
    
    for (int i = 0; i < dsRoot.size(); ++i) {
    	
    }
    /*	if(a[1] == "tall"){
    		//cout<<"This man is tall\n";
    		dsT.addData(a);
    		if(a[2] == "good"){
    			dsTG.addData(a);
    		}
    		else if(a[2] == "bad"){
    			dsTB.addData(a);
    		}
    		else{
    			cout<<"Error\n";
    		}
    	}
    	else if(a[1] == "short"){
    		//cout<<"This man is short\n";
    		dsS.addData(a);
    		if(a[2] == "good"){
    			dsSG.addData(a);
    		}
    		else if(a[2] == "bad"){
    			dsSB.addData(a);
    		}
    		else{
    			cout<<"Error\n";
    		}
    	}
    	else{
    		cout<<"Error!\n";
    	}
    	// collect dexterity information 
    	if(a[2] == "good"){
    		dsG.addData(a);
    		if(a[1] == "tall"){
    			dsGT.addData(a);
    		}
    		else if(a[1] == "short"){
    			dsGS.addData(a);
    		}
    		else{
    			cout<<"Error\n";
    		}
    	}
    	else if(a[2] == "bad"){
    		dsB.addData(a);
    		if(a[1] == "tall"){
    			dsBT.addData(a);
    		}
    		else if(a[1] == "short"){
    			dsBS.addData(a);
    		}
    		else{
    			cout<<"Error\n";
    		}
    	}
    	else{
    		cout<<"Error!\n";
    	}
    	a.clear(); 
    }

	// test the construction of the tree
	/*S.Top()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->Right()->RootData().printDataset();
    cout<<"----------------\n";
	S.Top()->Left()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Left()->Right()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->RootData().printDataset();*/
	/*
	float entropyx = Entropy(dsRoot); // Calculate the entropy
	cout<<"Root entropy: "<<entropyx<<endl;
	float entropyy = Entropy(dsT); // Calculate the entropy
    cout<<"Tall entropy entropy: "<<entropyy<<endl;
	float entropyz = Entropy(dsS); // Calculate the entropy
	cout<<"Short entropy: "<<entropyz<<endl;
	float Gain_Height = Gain(dsRoot, dsT, dsS);
	cout<<"Gain_Height: "<<Gain_Height<<endl;
	
	float entropya = Entropy(dsG); // Calculate the entropy
    cout<<"Good entropy: "<<entropya<<endl;
	float entropyb = Entropy(dsB); // Calculate the entropy
	cout<<"Bad entropy: "<<entropyb<<endl;
	float Gain_Dexterity = Gain(dsRoot, dsG, dsB);
	cout<<"Gain_Dexterity: "<<Gain_Dexterity<<endl;
	
	//Two ways to build the tree, depends on the value of information gain.
	if(Gain_Height>=Gain_Dexterity){
		for (int i = 0; i < dsRoot.size(); ++i) {
			S.Push(new Tree(dsTG,NULL,NULL));
	    	S.Push(new Tree(dsTB,NULL,NULL));
	    	T1=S.Top();
			S.Pop();
			T2=S.Top();
			S.Pop();
			S.Push(new Tree(dsT,T2,T1));
			S.Push(new Tree(dsSG,NULL,NULL));
	    	S.Push(new Tree(dsSB,NULL,NULL));
			T3=S.Top();
			S.Pop();
			T4=S.Top();
			S.Pop();
	    	S.Push(new Tree(dsS,T4,T3));
	    	T1=S.Top();
			S.Pop();
			T2=S.Top();
			S.Pop();
	    	S.Push(new Tree(dsRoot,T2,T1));
	    }
	}
    else{
    	for (int i = 0; i < dsRoot.size(); ++i) {
			S.Push(new Tree(dsGT,NULL,NULL));
	    	S.Push(new Tree(dsGS,NULL,NULL));
	    	T1=S.Top();
			S.Pop();
			T2=S.Top();
			S.Pop();
			S.Push(new Tree(dsG,T2,T1));
			S.Push(new Tree(dsBT,NULL,NULL));
	    	S.Push(new Tree(dsBS,NULL,NULL));
			T3=S.Top();
			S.Pop();
			T4=S.Top();
			S.Pop();
	    	S.Push(new Tree(dsB,T4,T3));
	    	T1=S.Top();
			S.Pop();
			T2=S.Top();
			S.Pop();
	    	S.Push(new Tree(dsRoot,T2,T1));
	    }
    }
    S.Top()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Right()->Right()->RootData().printDataset();
    cout<<"----------------\n";
	S.Top()->Left()->Left()->RootData().printDataset();
	cout<<"----------------\n";
	S.Top()->Left()->Right()->RootData().printDataset();
	cout<<"----------------\n";
	*/
}