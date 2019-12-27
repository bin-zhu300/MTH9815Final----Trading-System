/**
 * BONDPRODUCTSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDPRODUCTSERVICE_HPP
#define BONDPRODUCTSERVICE_HPP

#include "boost/date_time/gregorian/gregorian.hpp"
#include "products.hpp"
#include "soa.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace boost::gregorian;
using namespace std;


class BondProductService : public Service<string,Bond> {

public:
	// constructor
	BondProductService();

	Bond& GetData(string productId);

	void Add(Bond &bond);

	vector<Bond> GetBonds(string& _ticker);

	// override 
	virtual void OnMessage(Bond &data) override {};

	virtual void AddListener(ServiceListener<Bond > *listener) override {};
	
	virtual const vector< ServiceListener<Bond >* >& GetListeners() const override {};


private:
	map<string,Bond> bondMap; 

};

BondProductService::BondProductService() {
	bondMap = map<string,Bond>();
}

Bond& BondProductService::GetData(string productId) {
	return bondMap[productId];
}

void BondProductService::Add(Bond &bond) {
	bondMap.insert(pair<string,Bond>(bond.GetProductId(), bond));
}

vector<Bond> BondProductService::GetBonds(string& _ticker){
	vector<Bond> res;
	for(auto it=bondMap.begin();it!=bondMap.end();++it){
		if((it->second).GetTicker()==_ticker)
			res.push_back(it->second);
	}
	return res;
}

//Data Source: https://www.treasurydirect.gov/instit/instit.htm
vector<string> bondCusip = { "912828YT1", // 2Years
"912828YW4", // 3Years
"912828YV6", // 5Years
"912828YU8", // 7Years
"912828YS3", // 10Years
"912810SK5" }; // 30Years

vector<float> bondCoupon = { 0.015, // 2Years
0.01625, // 3Years
0.015, // 5Years
0.01625, // 7Years
0.0175, // 10Years
0.02375 }; // 30Years

vector<date> bondMaturity = { date(2021,Nov,30), // 2Years
date(2022,Dec,15), // 3Years
date(2024,Nov,30), // 5Years
date(2026,Nov,30), // 7Years
date(2029,Nov,15), // 10Years
date(2049,Nov,15) }; // 30Years

map<string, float> bondRisk = { {"912828YT1",0.018}, // 2Years
{"912828YW4",0.024}, // 3Years
{"912828YV6",0.048}, // 5Years
{"912828YU8",0.056}, // 7Years
{"912828YS3",0.088}, // 10Years
{"912810SK5",0.18} }; // 30Years

#endif