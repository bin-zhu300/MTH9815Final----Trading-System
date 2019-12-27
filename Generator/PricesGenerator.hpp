/**
 * Generate Price
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef	PRICERSGENERATOR_HPP
#define PRICERSGENERATOR_HPP

#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

class PricesGenerator {
public:
	// Constructor
	PricesGenerator();

	void operator()(int n=88);
};

PricesGenerator::PricesGenerator() {
}

void PricesGenerator::operator()(int n) {
	ofstream output;
	output.open("prices.txt");
	
	vector<int> count={0,0,0,0,0,0};
	
	for(int i=0;i<6*n;i++){
		int j=rand()%6;
		while(count[j]==n){
			j=rand()%6;
		}
		string cusip=bondCusip[j];
		++count[j];
	
		int a=rand()%2+99;
		int b=rand()%32;
		int c=rand()%8;
		
		string A=to_string(a);
		string B=to_string(b);
		string C=to_string(c);
		if(b<10)B="0"+B;
		if(c==4)C="+";
		string price=A+"-"+B+C;
		
		int d=rand()%4;
		string spread;
		if(d==0)spread="0-001";
		else if(d==1)spread="0-002";
		else if(d==2)spread="0-003";
		else spread="0-00+";
		
		output << cusip << "," << price << "," << spread << endl;
	}
	output.close();
}

#endif
