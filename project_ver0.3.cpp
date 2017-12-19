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

class Dataset {
private:
	vector<vector<string>> features;
	int numOfFeatures;
public:
	Dataset(vector<string> newthing);  // constructor with paramters
  	~Dataset() { }
  	string getFeature(int num);
  	void printDataset();
};

Dataset::Dataset(vector<string> newthing) {
	if (!newthing.empty()){
		numOfFeatures = newthing.size();
		for (vector<string>::iterator it = newthing.begin() ; it != newthing.end(); ++it) {
			features.push_back(*it);
		}
	}
}

string Dataset::getFeature(int num){
	if (num < numOfFeatures && num >= 0) return features[num];
	return "ERROR";
}

void Dataset::printDataset(){
	if(numOfFeatures > 0) {
		for (int i = 0; i < numOfFeatures; ++i) {
			cout<<features[i]<<", ";
		}
		cout<<endl;
	} else {
		cout<<"EMPTY FEATURE SET"<<endl;
	}
}

/*template <class Container>
void split1(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter(cont));
}*/

int main(){
	string token [100];
	vector<string> newthing;
    //newthing.push_back("yes, tall, good");

    ifstream input_file;
    input_file.open("data.txt");
    if(!input_file.is_open()){
    	cout<< "Error: Could not found your file!"<<endl;
    }

    //std::vector<std::string> words;

    while(!input_file.eof()) {  		
    	input_file>>token[0]>>token[1]>>token[2];
    	newthing.push_back(token[0]);
    	newthing.push_back(token[1]);
    	newthing.push_back(token[2]);
    	Dataset *data_one = new Dataset(newthing);

    }
   	
    input_file.close();
    
    data_one->printDataset();
	cout<<"Al good!"<<endl;
	getchar();
	
}