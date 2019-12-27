/**
 * Generate Market Data 
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef MARKETDATAGENERATOR_HPP
#define MARKETDATAGENERATOR_HPP

#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

class MarketdataGenerator {
public:
	// constructor
	MarketdataGenerator();

	void operator()(int n=88);
};

MarketdataGenerator::MarketdataGenerator() {
}

void MarketdataGenerator::operator()(int n) {
	ofstream output;
	output.open("marketdata.txt");
	
	vector<int> count={0,0,0,0,0,0};
	
	for(int i=0;i<6*n;i++){
		int j=rand()%6;
		while(count[j]==n){
			j=rand()%6;
		}
		string cusip=bondCusip[j];++count[j];
		
		output<<cusip<<",";
		vector<int> bidPriceA; 
		vector<int> bidPriceB;
		vector<int> bidPriceC;
		for(int k=0;k<5;++k){
			int a=rand()%2+99;
			int b=rand()%32;
			int c=rand()%8;
			bidPriceA.push_back(a);
			bidPriceB.push_back(b);
			bidPriceC.push_back(c);
			
			string A=to_string(a);
			string B=to_string(b);
			string C=to_string(c);
			if(b<10)B="0"+B;
			if(c==4)C="+";
			string price=A+"-"+B+C;
			
			string quantity=to_string((k%5+1)*1000000);
			
			string side="BID";
			
			output<<price<<","<<quantity<<","<<side<<",";
		}
		for(int k=0;k<5;++k){
			int a=bidPriceA[k];
			int b=bidPriceB[k];
			int c=bidPriceC[k];
			c+=(k+1);
			if(c>7){
				c-=8;
				b+=1;
			}
			if(b>31){
				b-=32;
				a+=1;
			}
			
			string A=to_string(a);
			string B=to_string(b);
			string C=to_string(c);
			if(b<10)B="0"+B;
			if(c==4)C="+";
			string price=A+"-"+B+C;
			
			string quantity=to_string((k%5+1)*1000000);
			
			string side="OFFER";
			
			output<<price<<","<<quantity<<","<<side<<",";
		}
		output<<endl;
	}
	output.close();
}
#endif