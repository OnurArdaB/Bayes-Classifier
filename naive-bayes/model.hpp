#include <bits/stdc++.h>
#pragma once

using namespace std;

class BayesClassifier{
    public:
        BayesClassifier();
        ~BayesClassifier();
        bool train(vector<vector<double>>&dataframe,vector<double>&labels);
        
        map<double,double> predict(vector<double>&unclassified);

    private:
        double total;
    	
        vector<double> labels;
        set<double> labels_;
        
        vector<map<double,map<double,double>>> likelihood_;
        
        map<double,int> prior_;
        
        void likelihood(vector<vector<double>>&dataframe);
        
        void prior();
};
BayesClassifier::BayesClassifier(){

}
BayesClassifier::~BayesClassifier(){

}
bool BayesClassifier::train(vector<vector<double>>&dataframe,vector<double>&labels){
    this->labels = labels;
    set<double> s(labels.begin(),labels.end());
    this->labels_ = s;
    this->total = dataframe.size();
    prior();
    likelihood(dataframe);
    return true;
}

void BayesClassifier::prior(){
	map<double,int> res;
	for(auto label:this->labels){
		if(res.count(label)==0)
			res.insert({label,1});
		else
			res[label]+=1;
	}
    this->prior_  = res;
}

void BayesClassifier::likelihood(vector<vector<double>>&dataframe){
	vector<map<double,map<double,double> > > table_(dataframe[0].size());
	
	for(int i=0;i<dataframe.size();i++){
		for(int feat = 0 ; feat<dataframe[0].size();feat++){
			if(table_[feat].count(dataframe[i][feat])==0){
				map<double,double> class_count;
				for(auto class_:this->labels_)
					class_count[class_]=class_!=this->labels[i] ? 0.0 : 1.0;
				table_[feat][{dataframe[i][feat]}] = class_count;
			}
			else{
				table_[feat][{dataframe[i][feat]}][this->labels[i]]+=1;
			}
		}
	}
    this->likelihood_ = table_;
}

map<double,double> BayesClassifier::predict(vector<double>&unclassified){
	map<double,double> res;
	for(auto&[class_,count]:this->prior_){
		double likelihood_=0;
		for(int feat = 0; feat<unclassified.size();feat++){
			likelihood_+= ((count != 0 && this->likelihood_[feat][unclassified[feat]][class_]!=0) ? (log(this->likelihood_[feat][unclassified[feat]][class_])-log(count)) : (log(this->likelihood_[feat][unclassified[feat]][class_]+0.0000001)-log(count+0.0000001)));
		}
		likelihood_+=(log(count)-log(this->total));
		res[class_] = abs(likelihood_);
	}
	return res;
}