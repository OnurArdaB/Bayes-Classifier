#include <bits/stdc++.h>
#include "naive-bayes/model.hpp"
using namespace std;


vector<string> split(const string& str, const string& delim){
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        	string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        	prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    	return tokens;
}

int main(){

	string filename;
	string encode_wanted;
	/*
	cout << "Enter train data as a  .CSV file: ";
	cin >> filename;
	*/
	filename="dataset/train_data.csv";
	//filename="train_images.csv";
	ifstream file_train_features;

	file_train_features.open(filename);
	
	vector<vector<double> > dataframe;
	
	if(file_train_features.fail()){
		cout << "Program can't find your file that you have entered please enter a valid path that exists" << endl;
	}
	else{
		cout << "File is opened successfully!" << endl;
		while(!file_train_features.eof()){
			string line;
			getline(file_train_features,line);
			vector<string> s = split(line,",");
			vector<double> s_;
			for(int i=0;i<s.size();i++){
				s_.push_back(stod(s[i]));
			}
			dataframe.push_back(s_);
		}
		file_train_features.close();
	}	
/*
	cout << "Enter train labels as a  .CSV file: ";
	cin >> filename;
*/
	filename="dataset/train_label.csv";
	//filename="train_labels.csv";
	ifstream file_train_labels;

	file_train_labels.open(filename);
	
	vector<double> labels;
	set<double> labels_;
	if(file_train_labels.fail()){
		cout << "Program can't find your file that you have entered please enter a valid path that exists" << endl;
	}
	else{
		cout << "File is opened successfully!" << endl;
		//Training
		while(!file_train_labels.eof()){
			string line;
			getline(file_train_labels,line);
			labels.push_back(stod(line));
			if(labels_.count(stod(line))==0)
				labels_.insert(stod(line));
		}
		file_train_labels.close();
	}
	BayesClassifier model;
	model.train(dataframe,labels);
/*	
	cout << "Enter test data as a  .CSV file: ";
	cin >> filename;
*/
	filename="dataset/test_data.csv";
	//filename="test_images.csv";
	ifstream file_test_features;

	file_test_features.open(filename);
	
	vector<vector<double> > test_dataframe;
	
	if(file_test_features.fail()){
		cout << "Program can't find your file that you have entered please enter a valid path that exists" << endl;
	}
	else{
		cout << "File is opened successfully!" << endl;
		while(!file_test_features.eof()){
			string line;
			getline(file_test_features,line);
			vector<string> s = split(line,",");
			vector<double> s_;
			for(int i=0;i<s.size();i++){
				s_.push_back(stold(s[i]));
			}
			test_dataframe.push_back(s_);
		}
		file_test_features.close();
	}	
	/*
	cout << "Enter test labels as a  .CSV file: ";
	cin >> filename;
	*/
	filename="dataset/test_label.csv";
	//filename="test_labels.csv";
	ifstream file_test_labels;

	file_test_labels.open(filename);
			
	vector<double> test_labels;

	if(file_test_labels.fail()){
		cout << "Program can't find your file that you have entered please enter a valid path that exists" << endl;
	}
	else{
		cout << "File is opened successfully!" << endl;
		while(!file_test_labels.eof()){
			string line;
			getline(file_test_labels,line);
			test_labels.push_back(stod(line));
		}
		file_test_labels.close();
	}
	cout << "Testing" <<endl;
	int match=0;
	int counter = 0;
	for(auto test_line:test_dataframe){
		auto pred = model.predict(test_line);
		double class_followed = 0;
		double prob_log_followed = 0;
		for(auto&[class_,prob_log]:pred){
			if(prob_log_followed==0){
				class_followed = class_;
				prob_log_followed=prob_log;
			}
			else if(prob_log_followed>prob_log){
				class_followed = class_;
				prob_log_followed=prob_log;
			}
		}
		if(class_followed==test_labels[counter])
			match++;
		counter++;
	}
	cout << match << endl;
}
