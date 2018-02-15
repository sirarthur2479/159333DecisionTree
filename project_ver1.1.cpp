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
#include <set>
#include <cmath>
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
	void erase_feature(int featureIndex){data.erase(data.begin()+featureIndex);numOfFeatures--;}
	Feature getFeatureByPrivateIndex(int privateFeatureIndex); 
	void erase_feautreByPrivateIndex(int privateFeatureIndex) {
		for (int i = 0; i < data.size(); ++i) {
			if (data[i].getIndex() == privateFeatureIndex) {
				privateFeatureIndex = i;
				break;
			}
		}
		cout<<"fs erasing : "<<data[privateFeatureIndex].getFeatureName()<<endl;
		data.erase(data.begin()+privateFeatureIndex);
		cout<<"erase completed"<<endl;
		numOfFeatures--;
	}
	
};
Feature FeatureSet::getFeatureByPrivateIndex(int privateFeatureIndex) {
	for (int i = 0; i < data.size(); ++i) {
		//cout<<"searching fs option by comparing indexes :"<<data[i].getIndex()<<" ?= "<<privateFeatureIndex<<endl;
		if (data[i].getIndex() == privateFeatureIndex) {
			//cout<<"found index in fs : "<<privateFeatureIndex<<" and its index in ds :"<<i<<endl;
			return data[i];
		}
	}
}

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
  	int size() { return features.size(); }
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
  	//Dataset(const Dataset&);
    void erase_data(int index);
};

Dataset::Dataset() {
	//vector<Data> dataset;
}

/*Dataset::Dataset(const Dataset& set){
	numOfData = set.size();
	for(int i = 0; i < numOfData; ++i){
		dataset.push_back(set.getData(i));
	}
}*/

void Dataset::erase_data(int index){
	dataset.erase(dataset.begin()+index);
	numOfData--;
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
		cout<<"Printing all data"<<endl;
		for (int i = 0; i < dataset[0].size(); ++i) { cout<<i<<"     ";} cout<<endl;
		for (int i = 0; i < numOfData; ++i) { dataset[i].printData(); }
	} else {
		cout<<"EMPTY DATA SET"<<endl;
	}
}

// Tree implementation
class Tree {
	private:
	  string label;
	  string result;
	  vector<string> name;//name of subtree pointers
	  vector<Tree*> subtree; 
 
	public:
	  Tree();  
	  ~Tree() { }
	  //Dataset RootData() { return data; }  
	  //Tree* Left() { return leftptr; }
	  //Tree* Right() { return rightptr; }
	  //bool isEndNode() { return endNode; }
	  //void setEndNode(bool temp) { endNode = temp; }
	  string getLabel() { return label; }
	  void setLabel(string labelName) { label = labelName; }
	  string getResult() { return result; }
	  void setResult(string resultName) { result = resultName; }
	  
	  void addSubTree(Tree* newthing, string newName);
	  Tree* getSubTreeByIndex(int index) { return subtree[index];}
	  void printSubTreeNameByIndex(int index) { cout<<"--("<<name[index]<<")-- ";}
	  int getSubTreeSize() { return subtree.size(); }

	  int checkResult();
	  void print_label() {cout<<"["<<label<<"] ";}
	  string RootData() { return label+"||"+result;}
	  Tree* Left() { return subtree[0]; }
	  Tree* Right() { return subtree[1]; }
	  void left_name() { cout<<"["<<name[0]<<"] ";}
	  void right_name() { cout<<"["<<name[1]<<"] "; }

	  
};

Tree::Tree() {
	label = "empty";
}

void Tree::addSubTree(Tree* newthing, string newName) {
	subtree.push_back(newthing);
	name.push_back(newName);
}

int Tree::checkResult() {
	if (label == "+") return 1;
	if (label == "-") return 0;
	return -1;
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

// Stack S;
// Tree *T1, *T2, *T3, *T4,*mytree;

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
	int indexOfnextBranchFs = 1;
	cout<<"informationGain featureSize now: "<<fs.getFeatureSize()<<endl;
	fs.printFeatureSet();
	for(int i = 2; i < fs.getFeatureSize(); ++i){
		float temp = Gain(ds,fs.getFeature(i));
		//cout<<"compare information gain: "<<temp<<"||"<<max<<endl;
		if (temp > max) {			
			max = temp;
			indexOfnextBranchFs = i;
		}
		//fs.getFeature(i).printFeature();		
	}
	int indexOfnextBranchDs = fs.getFeature(indexOfnextBranchFs).getIndex();
	cout<<"feaure name pick for infromation gain : "<<fs.getFeature(indexOfnextBranchFs).getFeatureName()<<endl;
	cout<<"		-indexOfnextBranch in ds: "<<indexOfnextBranchDs<<endl;
	cout<<"		-indexOfnextBranch in fs: "<<indexOfnextBranchFs<<endl;
	return indexOfnextBranchDs;
}



void addFeatureFunction(FeatureSet& set, string featureName){
	Feature tempFeature(featureName);
	set.addFeature(tempFeature);
}

// Automation for get the different options from different features
void scanOption(FeatureSet &fs, string featureName, vector <string> vec){
	//vector <string> temp1;
	Feature tempFeature(featureName);
	
    std::set<std::string> s;
    unsigned size = vec.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( vec[i] );
    vec.assign( s.begin(), s.end() );

    for (int i = 0; i < vec.size(); ++i) {
    	tempFeature.addOption(vec[i]);
	
    }
    fs.addFeature(tempFeature);
}

int checks_sample_positive_or_not(Dataset set){
    if (set.size()>1){
        //set.printDataset();
        int y = 0;//index location of result
        for (int i = 0; i < set.size(); ++i) {
            if(set.getData(0).getFeature(y) != set.getData(i).getFeature(y)){
                return -1;
            }
        }
    }
    if(set.getData(0).getFeature(0) == "yes") return 1;
    if(set.getData(0).getFeature(0) == "no") return 0;
}

int no_more_feature_left(FeatureSet set){
    if(set.getFeatureSize() == 1){
        return 1;
    }
    if(set.getFeatureSize() > 1) return set.getFeatureSize();
    if(set.getFeatureSize() < 1) return -1;
}

int most_common_value(Dataset set, string branch_name){
    int number_of_yes = 0;
    int number_of_no = 0;
    for(int i = 0; i < set.size(); ++i){
        if(set.getData(i).getFeature(0) == "yes") number_of_yes++;
        else number_of_no++;
    }
    if(number_of_yes > number_of_no) return 1;
    if(number_of_yes < number_of_no) return 0;
    if(number_of_yes == 0 && number_of_no == 0) return 0;
    if(number_of_yes == number_of_no) {
        // if(branch_name=="short") return 0;
        // if(branch_name=="tall") return 1;
        // if(branch_name=="bad") return 0;
        // if(branch_name=="good") return 1;
        return 1;
    }
         
    
}
Tree* Function_ID3(Dataset ds, string branch_name, FeatureSet fs, string feature_option_for_tree ){
		Tree* Root = new Tree;
		
		//ds.printDataset();cout<<"\n|||||||||||||||||||||||\n";
		if(checks_sample_positive_or_not(ds) == 1){//positive/yes result
		    Tree* leaf = new Tree;
		    leaf->setLabel("+ "  + feature_option_for_tree);
		    
		    return leaf;
		}
		if(checks_sample_positive_or_not(ds) == 0){//negative/no result
		    Tree* leaf = new Tree;
		    leaf->setLabel("- "  + feature_option_for_tree);
		    return leaf;
		}
		
		if(no_more_feature_left(fs) == 1){
		    Tree* leaf = new Tree;
		    if(most_common_value(ds, branch_name) == 1) leaf->setLabel("+ "  + feature_option_for_tree);
		    else leaf->setLabel("- "  + feature_option_for_tree);
		    return leaf; 
		} else {
		    int feature_index = nextBranchByInformationGain(ds, fs);
		    Root->setLabel(fs.getFeatureByPrivateIndex(feature_index).getFeatureName());
		    for(int i = 0; i <fs.getFeatureByPrivateIndex(feature_index).optionSize(); ++i ){
		    	//cout<<"name :"<< fs.getFeature(feature_index).getFeatureName()<<" size :"<< fs.getFeature(feature_index).optionSize()<<endl;
	            Dataset temp_set_for_pruning(ds);//E(a)
	            // need a judgemnet here to decide whether add a subtree or not
    		    for(int j = ds.size()-1; j >= 0; j-- ){
    		        if(ds.getData(j).getFeature(feature_index) != fs.getFeatureByPrivateIndex(feature_index).getOption(i)){
    		        	cout<<"erasing :"<<ds.getData(j).getFeature(feature_index)<<"||"<<endl;
    		        	cout<<"comparing : "<<fs.getFeatureByPrivateIndex(feature_index).getOption(i)<<endl;
    		            temp_set_for_pruning.erase_data(j);
    		        }
    		    
    		    }
    		    temp_set_for_pruning.printDataset();
    		    
            		    
    		    if(temp_set_for_pruning.size()==0){//EMPTY DATA SET
    		    	//cout<<"5"<<endl;
    		        Tree* leaf = new Tree;
    		        cout<<"branch_name: "<<branch_name<<endl<<endl;
        		    if(most_common_value(temp_set_for_pruning, branch_name) == 1) {leaf->setLabel("+ "  + fs.getFeatureByPrivateIndex(feature_index).getOption(i));}
        		    else {leaf->setLabel("- "  + fs.getFeatureByPrivateIndex(feature_index).getOption(i));}
        		    Root->addSubTree(leaf, fs.getFeatureByPrivateIndex(feature_index).getOption(i)); 
    		    } else{//still data left in dataset
    		    //	cout<<"6"<<endl;
    		    	FeatureSet temp_fs_for_pruning(fs);
    		    	temp_fs_for_pruning.erase_feautreByPrivateIndex(feature_index);
    		        Root->addSubTree(Function_ID3(temp_set_for_pruning, fs.getFeatureByPrivateIndex(feature_index).getOption(i), temp_fs_for_pruning, fs.getFeatureByPrivateIndex(feature_index).getOption(i)), fs.getFeatureByPrivateIndex(feature_index).getOption(i));
    		    }
            }
		    return Root;
		}
}

void inOrder(Tree *T){
	//if(T==NULL){return;}
	//if(T->Left()!=NULL)cout<<"(";
	if(T->getSubTreeSize() != 0) inOrder(T->Left());
	T->print_label();
	if(T->getSubTreeSize() != 0) inOrder(T->Right());
	//if(T->Right()!=NULL)cout<<")";
}

void PostOrder(Tree *T) {

	if(T->getSubTreeSize() != 0) PostOrder(T->Left());
	if(T->getSubTreeSize() != 0) PostOrder(T->Right());
	T->print_label();
}

void PreOrder(Tree *T) {
	//root, left, right

	T->print_label();
	if(T->getSubTreeSize() != 0) PreOrder(T->Left());
	if(T->getSubTreeSize() != 0) PreOrder(T->Right());
}

void PreOrderForID3Short(Tree *T) {
	//for not binary options

	T->print_label();
	for (int i = 0; i < T->getSubTreeSize(); ++i) {
		PreOrderForID3Short(T->getSubTreeByIndex(i));	
	}	
}

void PreOrderForID3(Tree *T) {
	//for not binary options

	T->print_label();
	for (int i = 0; i < T->getSubTreeSize(); ++i) {
		T->printSubTreeNameByIndex(i);
		PreOrderForID3(T->getSubTreeByIndex(i));	
	}	
}

int main(){
	vector<string> a;
	string token [100];
	ifstream input_file;
	
    Dataset dsRoot;
	
	FeatureSet fs;
	
	vector<string> featureNames;
    featureNames.push_back("result");featureNames.push_back("Height");featureNames.push_back("Dex");featureNames.push_back("Fitness");//featureNames.push_back("iq");
    
    input_file.open("data.2.txt");
    //input_file.open("data.txt");
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    while(!input_file.eof()) {  		
    	//input_file>>token[0]>>token[1]>>token[2];
    	for (int i = 0; i < featureNames.size(); ++i) {
    		input_file>>token[i];
    		a.push_back(token[i]);
    	}
    	dsRoot.addData(a);
    	a.clear(); 
    }

    input_file.close();
    for (int i = 0; i < featureNames.size(); ++i) {
    	vector<string> tempVec;//store data vertically
	    for (int j = 0;  j < dsRoot.size(); ++j) {
	    	tempVec.push_back(dsRoot.getData(j).getFeature(i));
	    }
	    scanOption(fs, featureNames[i], tempVec);
    }
   
   fs.printFeatureSet();
    
	Tree* root = new Tree;
	root = Function_ID3(dsRoot,"root", fs, "none");

	//inOrder(root);cout<<"||"<<endl;
	//PostOrder(root);cout<<"||"<<endl;
	PreOrderForID3Short(root);cout<<"||"<<endl<<endl;
	PreOrderForID3(root);cout<<"||"<<endl;
}
