/**
 * Generate Trades
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef TRADESGENERATOR_HPP
#define TRADESGENERATOR_HPP

#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

class TradesGenerator {
public:
	// constructor
	TradesGenerator();

	void operator()(int n=88);
};

TradesGenerator::TradesGenerator() {
}

void TradesGenerator::operator()(int n) {
	ofstream output;
	output.open("trades.txt");
	
	vector<int> cnt={0,0,0,0,0,0};
	
	for(int i=0;i<6*n;i++)
	{
		int j=rand()%6;
		while(cnt[j]==n)
		{
			j=rand()%6;
		}
		string cusip=bondCusip[j]; ++cnt[j];
		
		j=rand()%6; string trader;
		switch(j){
			case 0:trader ="Karl";break;
			case 1:trader ="Bin";break;
			case 2:trader ="Lu";break;
			case 3:trader ="Zhu";break;
			case 4:trader = "Wang"; break;
			case 5:trader = "John"; break;
		}
		output<<cusip<<","<< trader <<",";
		
		int a=rand()%2+99;
		int b=rand()%32;
		int c=rand()%8;		
		string A=to_string(a);
		string B=to_string(b);
		string C=to_string(c);
		if(b<10)B="0"+B;
		if(c==4)C="+";
		string price=A+"-"+B+C;
		output<<price<<",";
		
		output<<"Book"<<",";
		output<<to_string((rand()%5+1)*1000000)<<",";
		output<<(rand()%2==0?"BUY":"SELL");
		
		output <<endl;
	}
	output.close();
	return;
}
#endif