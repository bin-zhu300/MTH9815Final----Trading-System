/**
 * Generate Inquries
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef INQURIESGENERATOR_HPP
#define INQURIESGENERATOR_HPP

#include <fstream>
#include <random>
#include <vector>
#include <string>

using namespace std;

class InquriesGenerator {
	string FillCast(string v, int width, char c);
public:
	// constructor
	InquriesGenerator();

	void operator()(int n=88);
};

string InquriesGenerator::FillCast(string a, int w, char b){
	string result;std::stringstream inter;
	inter << std::setw(w) << std::setfill(b) << a; inter >> result;
	return result;
}

InquriesGenerator::InquriesGenerator() {
}

void InquriesGenerator::operator()(int n) {
	ofstream output;
	output.open("inquiries.txt");
	
	vector<int> count={0,0,0,0,0,0};
	
	for(int i=0;i<6*n;i++){
		int j=rand()%6;
		while(count[j]==n){
			j=rand()%6;
		}
		string cusip=bondCusip[j]; ++count[j];
		
		auto id=FillCast(to_string(i),8,'0');
		output<<id<<","<<cusip<<",";
		output<<(rand()%2==0?"BUY":"SELL")<<",";
		output<<to_string((rand()%5+1)*1000000)<<",";
		
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
		
		output<<"QUOTED"<<",";
		output <<endl;
	}
	output.close();
	return;
}
#endif