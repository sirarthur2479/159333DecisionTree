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
	int index;
public:
	Feature(string name);
	~Feature() {}
	string getFeatureName() { return featureName; }
	void setFeatureName(string name) { featureName = name; }
	vector<string> getOptions() { if (options.size() != 0) return options;}
	void addOption(string newOption) { options.push_back(newOption); }
	string getOption(int optionIndex) { return options[optionIndex]; }
	int optionSize() {return options.size(); }
	void printFeature();
	int getIndex() { return index; }
	void setIndex(int num) { index = num; }
};
Feature::Feature(string name) {
	setFeatureName(name);
}

void Feature::printFeature() {
	cout<<featureName<<":\n";
	cout<<"index:"<<index<<":\n";
	for (int i = 0; i < options.size(); ++i) {
    	cout<<options[i]<<" || ";
    }
    cout<<endl;
}

class FeatureSet {
private:
	vector<Feature> data;
	int numOfFeatures;
public:
	FeatureSet() {numOfFeatures = 0;}
	~FeatureSet() {}
	void addFeature(Feature newFeature) { newFeature.setIndex(numOfFeatures); numOfFeatures++;data.push_back(newFeature); }
	vector<Feature> getFeatureSet(){ return data; }
	void addOption(string featureName, string newOption);
	void printFeatureSet();
	int getFeatureSize() { return data.size(); }
	Feature getFeature(int featureIndex) { return data[featureIndex]; }
	int getFeatureIndex(string featureName);
};

int FeatureSet::getFeatureIndex(string featureName) {
	for (int i = 0; i < data.size(); ++i) {
		if (featureName == data[i].getFeatureName()) return i;
	}
	return -1;
}

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

float Gain(Dataset ds, Feature f){
	vector<string> options = f.getOptions();
	
	float totalEntropy = Entropy(ds);
	float totalPopulationSize = ds.size();
	
	float eachOptionEntropy = 0;
	float eachOptoinSize = 0;

	for (int j = 0; j < options.size(); ++j) {
		Dataset tempDs;
		for (int i = 0; i < ds.size(); ++i) {
			int featureIndex = f.getIndex();
			if (ds.getData(i).getFeature(featureIndex) == options[j]) {
				tempDs.addData(ds.getData(i));
			}
		}
		eachOptoinSize = tempDs.size();
		eachOptionEntropy = Entropy(tempDs);

		
		totalEntropy = totalEntropy - eachOptoinSize/totalPopulationSize * eachOptionEntropy;
	}
	return totalEntropy;
}

int nextBranchByInformationGain(Dataset ds, FeatureSet fs){
	float max = Gain(ds,fs.getFeature(1));
	int indexOfnextBranch = 1;
	for(int i = 2; i < fs.getFeatureSize(); ++i){
		float temp = Gain(ds,fs.getFeature(i));
		cout<<temp<<"||"<<max<<endl;
		if (temp > max) {
			
			max = temp;
			indexOfnextBranch = i;
		}
	}
	return indexOfnextBranch;
}



void addFeatureFunction(FeatureSet& set, string featureName){
	Feature tempFeature(featureName);
	set.addFeature(tempFeature);
}

// Automation for get the different options from different features
void scanOption(FeatureSet &set, string featureName, vector <string> temp){
	vector <string> temp1;
	Feature tempFeature(featureName);
	temp1.push_back(temp[0]);
	for (int i = 0; i < temp.size()-1; ++i) {
    	if(temp[i+1] != temp[0]){
    		temp1.push_back(temp[i+1]);
    	}
    }
    temp1.erase( unique( temp1.begin(), temp1.end() ), temp1.end() );
    
    for (int i = 0; i < temp1.size(); ++i) {
    	tempFeature.addOption(temp1[i]);
	
    }
    set.addFeature(tempFeature);
}

int main(){
	vector<string> a;
	vector<string> b;
	vector<string> c;
	vector<string> d;
	vector<string> b1;
	vector<string> c1;
	vector<string> d1;
	Dataset data, dsRoot, dsT, dsS, dsTG, dsTB, dsSG, dsSB;// Root_dataset, Tall_dataset, Short_dataset, TallandGood_dataset, TallandBad_dataset, ShortandGood_dataset, ShortandBad_dataset
	Dataset dsG,dsB,dsGT,dsGS,dsBT,dsBS;
	string token [100];
	ifstream input_file;

	
	FeatureSet fs;
	
    input_file.open("data.txt");
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    while(!input_file.eof()) {  		
    	input_file>>token[0]>>token[1]>>token[2];
    	a.push_back(token[0]);
    	b.push_back(token[0]);
    	a.push_back(token[1]);
    	c.push_back(token[1]);
    	a.push_back(token[2]);
    	d.push_back(token[2]);
    	dsRoot.addData(a);
    	a.clear(); 
    }
    input_file.close();
    
    // Automation
    scanOption(fs, "result", b);
    scanOption(fs, "Height", c);
    scanOption(fs, "Dex", d);
   
    fs.printFeatureSet();
    
	Feature f = fs.getFeature(1);
	f.setIndex(1);
	float Gain_Height = Gain(dsRoot, f);
	cout<<"Gain_Height: "<<Gain_Height<<endl;
	
    Feature f2 = fs.getFeature(2);
	f2.setIndex(2);
	float Gain_Dexterity = Gain(dsRoot, f2);
	cout<<"Gain_Dexterity: "<<Gain_Dexterity<<endl;
	
	int m = nextBranchByInformationGain(dsRoot,fs);
	cout<<m<<endl;
	
	/*
	float entropya = Entropy(dsG); // Calculate the entropy
    cout<<"Good entropy: "<<entropya<<endl;
	float entropyb = Entropy(dsB); // Calculate the entropy
	cout<<"Bad entropy: "<<entropyb<<endl;
	float Gain_Dexterity = Gain(dsRoot, dsG, dsB);
	cout<<"Gain_Dexterity: "<<Gain_Dexterity<<endl;
	*/
	
	
	//Two ways to build the tree, depends on the value of information gain.
	/*(Gain_Height>=Gain_Dexterity){
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