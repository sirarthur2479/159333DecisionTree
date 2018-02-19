/*
Many data used put result as the last feature
This short program used the data and dataset class to move the result feature 
to the beginning 
*/
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
};

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

int main(){
	vector<string> a;
	vector<string>::iterator it;

	string token [100];
	ifstream input_file;
	int featureSize = 5;
    Dataset dsRoot;
   
   	char filename_to_convert[50] = "yellow-small+adult-stretch.txt";
    input_file.open(filename_to_convert);
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    while(!input_file.eof()) {  		
    	for (int i = 0; i < featureSize; ++i) {
    		input_file>>token[i];
    		if (i < featureSize - 1) a.push_back(token[i]);
    		else {
    			it = a.begin();
    			a.insert(it, token[i]);
    		}
    	}
    	dsRoot.addData(a);
    	a.clear(); 
    }
    dsRoot.printDataset();
    input_file.close();

    ofstream output_file;
    output_file.open(filename_to_convert);
    for (int i = 0; i < dsRoot.size(); ++i) {
    	for (int j = 0; j < featureSize; ++j){
    		output_file << dsRoot.getData(i).getFeature(j);
    		if (j < featureSize - 1) output_file << " ";
    		else if (i == dsRoot.size() - 1 && j == featureSize - 1) break;
    		else  output_file << "\n";
    	}
    }
    output_file.close();
}

