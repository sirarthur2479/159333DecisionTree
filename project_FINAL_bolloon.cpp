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

class Line {
public:
	vector<string> word;
	Line(){}
	~Line(){}
	void addWord(string newWord){ word.push_back(newWord); }
	void printWord() { for (int i = 0; i < word.size(); ++i){cout<<word[i];} cout<<endl;}
};

class Lines {
public:
	vector<Line*> data;
	Lines(){}
	~Lines(){}
	void addLines(Line* newLine){ data.push_back(newLine);}
	void printLines() {for (int i = 0; i < data.size(); ++i) { cout<<i<<": "; data[i]->printWord();}}
};

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
	void addFeature(Feature newFeature);
	void addOption(string featureName, string newOption);
	void printFeatureSet();
	int size() { return data.size(); }
	Feature getFeature(int featureIndex) { return data[featureIndex]; }
	Feature getFeatureByPrivateIndex(int privateFeatureIndex); 
	void eraseFeatureByPrivateIndex(int privateFeatureIndex);
};

void FeatureSet::addFeature(Feature newFeature) { newFeature.setIndex(numOfFeatures); numOfFeatures++;data.push_back(newFeature); }

void FeatureSet::eraseFeatureByPrivateIndex(int privateFeatureIndex) {
	for (int i = 0; i < data.size(); ++i) {
		if (data[i].getIndex() == privateFeatureIndex) {
			privateFeatureIndex = i;
			break;
		}
	}
	data.erase(data.begin()+privateFeatureIndex);
	numOfFeatures--;
}
Feature FeatureSet::getFeatureByPrivateIndex(int privateFeatureIndex) {
	for (int i = 0; i < data.size(); ++i) {
		if (data[i].getIndex() == privateFeatureIndex) {
			return data[i];
		}
	}
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
	cout<<"FeatureSet: "<<endl;
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
			cout<<" ";
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
	Dataset() { }
  	~Dataset() { }
  	int size(){ return dataset.size();}
  	int addData(Data newthing);//add an unit of Data, return 1 if success; 0 if failed
  	Data getData(int num);//access a paticular data, mayby not needed, vector can just use [index] to access
  	void printDataset();
    void erase_data(int index);
};

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
		cout<<"Dataset: "<<endl;
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
	  vector<string> name;//name of subtree pointers
	  vector<Tree*> subtree; 
 
	public:
	  Tree() { } 
	  ~Tree() { }	  
	  void setLabel(string labelName) { label = labelName; }
	  string getLabel() { return label; }
	  void print_label() {cout<<"["<<label<<"] ";}

	  void addSubTree(Tree* newthing, string newName);
	  int getSubTreeSize() { return subtree.size(); }	
	  Tree* getSubTreeByIndex(int index) { return subtree[index];}
	  string getSubTreeNameByIndex(int index) { return name[index]; }
	  void printSubTreeNameByIndex(int index) { cout<<"--("<<name[index]<<")-- ";}	      
};

void Tree::addSubTree(Tree* newthing, string newName) {
	subtree.push_back(newthing);
	name.push_back(newName);
}

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
	for(int i = 2; i < fs.size(); ++i){
		float temp = Gain(ds,fs.getFeature(i));
		if (temp > max) {			
			max = temp;
			indexOfnextBranchFs = i;
		}	
	}
	int indexOfnextBranchDs = fs.getFeature(indexOfnextBranchFs).getIndex();
	return indexOfnextBranchDs;
}

void addFeatureFunction(FeatureSet& set, string featureName){
	Feature tempFeature(featureName);
	set.addFeature(tempFeature);
}

// Automaticcally get options from all features
void scanOption(FeatureSet &fs, string featureName, vector <string> vec){
	Feature tempFeature(featureName);
    std::set<std::string> s;//use set to remove repeated options
    unsigned size = vec.size();
    for( unsigned i = 0; i < size; ++i ) s.insert( vec[i] );
    vec.assign( s.begin(), s.end() );//convert from set to vector

    for (int i = 0; i < vec.size(); ++i) {
    	tempFeature.addOption(vec[i]);
    }
    fs.addFeature(tempFeature);
}

int checks_sample_positive_or_not(Dataset set){
    if (set.size()>1){
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
    if(set.size() == 1){
        return 1;
    }
    if(set.size() > 1) return set.size();
    if(set.size() < 1) return -1;
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
    //can add special instruction for generating leaf here
        // if(branch_name=="short") return 0;
        // if(branch_name=="tall") return 1;
        return 1;
    }
         
    
}
Tree* Function_ID3(Dataset ds, string branch_name, FeatureSet fs){
		Tree* Root = new Tree;
		
		if(checks_sample_positive_or_not(ds) == 1){//positive/yes result
		    Tree* leaf = new Tree;
		    leaf->setLabel("+");
		    
		    return leaf;
		}
		if(checks_sample_positive_or_not(ds) == 0){//negative/no result
		    Tree* leaf = new Tree;
		    leaf->setLabel("-");
		    return leaf;
		}
		
		if(no_more_feature_left(fs) == 1){
		    Tree* leaf = new Tree;
		    if(most_common_value(ds, branch_name) == 1) leaf->setLabel("+");
		    else leaf->setLabel("-");
		    return leaf; 
		} else {
		    int feature_index = nextBranchByInformationGain(ds, fs);
		    Root->setLabel(fs.getFeatureByPrivateIndex(feature_index).getFeatureName());
		    for(int i = 0; i <fs.getFeatureByPrivateIndex(feature_index).optionSize(); ++i ){
	            Dataset temp_set_for_pruning(ds);//E(a)
    		    for(int j = ds.size()-1; j >= 0; j-- ){
    		        if(ds.getData(j).getFeature(feature_index) != fs.getFeatureByPrivateIndex(feature_index).getOption(i)){
    		            temp_set_for_pruning.erase_data(j);
    		        }		    
    		    }
    		    //temp_set_for_pruning.printDataset();
    		               		    
    		    if(temp_set_for_pruning.size()==0){//EMPTY DATA SET
    		        Tree* leaf = new Tree;
        		    if(most_common_value(temp_set_for_pruning, branch_name) == 1) {leaf->setLabel("+");}
        		    else {leaf->setLabel("-");}
        		    Root->addSubTree(leaf, fs.getFeatureByPrivateIndex(feature_index).getOption(i)); 
    		    } else{//still data left in dataset
    		    	FeatureSet temp_fs_for_pruning(fs);
    		    	temp_fs_for_pruning.eraseFeatureByPrivateIndex(feature_index);
    		        Root->addSubTree(Function_ID3(temp_set_for_pruning, fs.getFeatureByPrivateIndex(feature_index).getOption(i), temp_fs_for_pruning), fs.getFeatureByPrivateIndex(feature_index).getOption(i));
    		    }
            }
		    return Root;
		}
}


void PreOrderForID3Short(Tree *T) {
	//for not binary options

	T->print_label();
	for (int i = 0; i < T->getSubTreeSize(); ++i) {
		PreOrderForID3Short(T->getSubTreeByIndex(i));	
	}	
}

void PreOrderForID3(Tree *T) {
	T->print_label();
	for (int i = 0; i < T->getSubTreeSize(); ++i) {
		T->printSubTreeNameByIndex(i);
		PreOrderForID3(T->getSubTreeByIndex(i));	
	}	
}

void PreOrderForID3Alter(Tree *T, int level) {
	//for not binary options

	cout<<"level "<<level<<" "<<T->getLabel()<<endl;
	level++;
	for (int i = 0; i < T->getSubTreeSize(); ++i) {
		cout<<T->getSubTreeNameByIndex(i)<<endl;
		PreOrderForID3Alter(T->getSubTreeByIndex(i), level);	
	}
}


void AccuracyTree(Tree *T, Dataset ds, FeatureSet fs, int resultIndex) {
	Tree* treeRoot = T;
	int numOfCorrectResult = 0;
	for (int i = 0; i < ds.size(); ++i) {//for each dataset entry
		int featureIndex = -1, valueIndex = -1;
		while(true){
			if ((T->getLabel() == "+")||(T->getLabel() == "-")){				
				string resultTranslation;
				if (T->getLabel() == "+") resultTranslation = "yes";
				else if (T->getLabel() == "-") resultTranslation = "no";
				else { cout<<"failed result translation, STOP"<<endl;break; }
				
				if (resultTranslation == ds.getData(i).getFeature(resultIndex)) {
					cout<<resultTranslation<<" ";
					numOfCorrectResult++;
				} else cout<<"["<<resultTranslation<<"] ";
				T = treeRoot;
				break;
			}
			for (int j = 0; j < fs.size(); ++j){//go through festure name
				if (T->getLabel() == fs.getFeature(j).getFeatureName()){
					featureIndex = j;//get index, then find its value
				}
			}
			if (featureIndex == -1) {//failed to find right featureName
				cout<<"failed to find right featureName, STOP"<<endl;break;
			}
			for (int j = 0; j < T->getSubTreeSize(); ++j) {//go through all branchess of tree
				if (ds.getData(i).getFeature(featureIndex) == T->getSubTreeNameByIndex(j)){
					valueIndex = j;
					break;
				}
			}
			if (valueIndex == -1) {//failed to find right value
				cout<<"failed to find right value, training dataset does not cover all possible outcomes"<<endl;break;
			}
			T = T->getSubTreeByIndex(valueIndex);			
		}
	}
	cout<<endl;
	cout<<"Accuracy of Tree: "<<(float)numOfCorrectResult/(float)ds.size() *100 <<"% for "<<numOfCorrectResult<<" out of "<< ds.size()<<endl;
}

int main(){
	vector<string> a;
	string token [100];
	ifstream input_file;
	
    Dataset dsTraining;
    Dataset dsTest;
	FeatureSet fs;
	char  training_dataset_input_file[50] = "yellow-small.txt";
	char  testing_dataset_input_file[50] = "yellow-small2.txt";
	
	vector<string> featureNames;
    featureNames.push_back("inflated");featureNames.push_back("Color");
    featureNames.push_back("size");featureNames.push_back("act");
    featureNames.push_back("age");
    //featureNames.push_back("iq");
    
    input_file.open(training_dataset_input_file);
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    while(!input_file.eof()) {  		
    	for (int i = 0; i < featureNames.size(); ++i) {
    		input_file>>token[i];
    		a.push_back(token[i]);
    	}
    	dsTraining.addData(a);
    	a.clear(); 
    }
    dsTraining.printDataset();

    input_file.close();
    for (int i = 0; i < featureNames.size(); ++i) {
    	vector<string> tempVec;//store data vertically
	    for (int j = 0;  j < dsTraining.size(); ++j) {
	    	tempVec.push_back(dsTraining.getData(j).getFeature(i));
	    }
	    scanOption(fs, featureNames[i], tempVec);
    }
   
   fs.printFeatureSet();
    
	Tree* root = new Tree;
	root = Function_ID3(dsTraining,"root", fs);


	PreOrderForID3(root);//working ||horizontal version of tree
	//PreOrderForID3Alter(root, 0);//working ||vertical version of tree
	cout<<endl;


	input_file.open(testing_dataset_input_file);
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }
	while(!input_file.eof()) {  		
    	for (int i = 0; i < featureNames.size(); ++i) {
    		input_file>>token[i];
    		a.push_back(token[i]);
    	}
    	dsTest.addData(a);
    	a.clear(); 
    }
    dsTest.printDataset();

	AccuracyTree(root, dsTest, fs, 0);//working
}

