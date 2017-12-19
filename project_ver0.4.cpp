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

int main(){
	//test class Data and Dataset
	vector<string> a;
	a.push_back("yes");
	a.push_back("no");
	a.push_back("yes");
	Data d(a);

	//d.printData();
	//cout<<"test getData():"<<d.getFeature(0)<<d.getFeature(1)<<d.getFeature(2);
	a.clear();
	a.push_back("x");
	a.push_back("y");
	a.push_back("x");
	Data d2(a);
	//d2.printData();

	Dataset ds;
	ds.addData(d);
	//ds.printDataset();
	ds.addData(d2);
	//ds.printDataset();//final test success

	//test IO with new classes
	Dataset ds2;
	a.clear();

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
    	ds2.addData(a);
    	a.clear(); 
    }
	input_file.close();
	ds2.printDataset();
} 