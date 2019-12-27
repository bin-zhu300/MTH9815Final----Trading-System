/**
 * BONDINQUIRYSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once 

#ifndef BONDINQUIRYSERVICE_HPP
#define BONDINQUIRYSERVICE_HPP

#include "inquiryservice.hpp"
#include "soa.hpp"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

//Service
class BondInquiryService : public InquiryService<Bond> {
private:
	map<string, Inquiry<Bond> > inquiryMap;
	vector<ServiceListener<Inquiry<Bond> >*> listenerList;

public:
	// constructor
	BondInquiryService();

	// override virtual functions
	virtual Inquiry<Bond> & GetData(string id) override;

	virtual void OnMessage(Inquiry<Bond> &inquiry) override;

	virtual void AddListener(ServiceListener<Inquiry<Bond> > *listener) override;

	virtual const vector<ServiceListener<Inquiry<Bond> > *>& GetListeners() const override;

	virtual void SendQuote(const string& inquiryId, double price) override {}

	virtual void RejectInquiry(const string &inquiryId) override {}

};

BondInquiryService::BondInquiryService() {
	inquiryMap = map<string, Inquiry<Bond> >();
}

Inquiry<Bond> & BondInquiryService::GetData(string id) {
	return inquiryMap.at(id);
}

void BondInquiryService::OnMessage(Inquiry<Bond> &inquiry) {
	inquiry.SetPnS(inquiry.GetPrice(), DONE);
	inquiryMap.insert(pair<string, Inquiry<Bond> >(inquiry.GetInquiryId(), inquiry));
	for (auto& listener : listenerList) {
		listener->ProcessAdd(inquiry);
	}

	return;
}

void BondInquiryService::AddListener(ServiceListener<Inquiry<Bond> > *listener) {
	listenerList.push_back(listener);
	return;
}

const vector<ServiceListener<Inquiry<Bond> > *>& BondInquiryService::GetListeners() const {
	return listenerList;
}

//Connector
class BondInquiryService;
class BondInquiryServiceConnector : public Connector<Inquiry<Bond> > {
private:
	BondInquiryService* bondInquiryService;
	BondProductService* bondProductService;

	float TransformStringToPrice(string& s);
public:
	// constructor
	BondInquiryServiceConnector(BondInquiryService* _bondInquiryService, BondProductService* _bondProductService);

	// override class
	virtual void Publish(Inquiry<Bond>& data) override {}

	virtual void Subscribe() override;
};

float BondInquiryServiceConnector::TransformStringToPrice(string& s) {
	int n = s.size();
	if (s[n - 1] == '+')s[n - 1] = '4';
	return stoi(s.substr(0, n - 4)) + stoi(s.substr(n - 3, 2)) / 32.0 + stoi(s.substr(n - 1, 1)) / 256.0;
}


BondInquiryServiceConnector::BondInquiryServiceConnector(BondInquiryService* _bondInquiryService, BondProductService* _bondProductService) {
	bondInquiryService = _bondInquiryService;
	bondProductService = _bondProductService;
}

void BondInquiryServiceConnector::Subscribe(){
	ifstream input;
	input.open("inquiries.txt");
	string row;
	
	while (getline(input, row)) {
		stringstream line(row);
		string temp;
		
		getline(line, temp, ',');   
		string inquiry_ID = temp;
		
		getline(line, temp, ',');
		auto bond = bondProductService->GetData(temp);
		
		getline(line, temp, ',');
		Side side = (temp=="BUY" ? BUY:SELL);
            
		getline(line, temp, ',');
		long quantity = stol(temp);
		
		getline(line, temp, ',');
		double quote = TransformStringToPrice(temp);
            
		getline(line, temp, ',');
		InquiryState state_ = RECEIVED;
		
		Inquiry<Bond> inquiry(inquiry_ID, bond, side, quantity, quote, state_);
		bondInquiryService -> OnMessage(inquiry);
	}
	return;
}

#endif