/**
 * BONDEXECUTIONSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDEXECUTIONSERVICE_HPP
#define BONDEXECUTIONSERVICE_HPP

#include "BondAlgoExecutionService.hpp"
#include "executionservice.hpp"
#include "soa.hpp"
#include "products.hpp"
#include <string>
#include <iostream>

using namespace std;

//Connector
class BondExecutionServiceConnector :public Connector<ExecutionOrder<Bond> > {
public:
	// connector
	BondExecutionServiceConnector();

	// override base class
	virtual void Publish(ExecutionOrder<Bond>& data) override;

	virtual void Subscribe() override;
};

BondExecutionServiceConnector::BondExecutionServiceConnector() {}

void BondExecutionServiceConnector::Publish(ExecutionOrder<Bond>& data) {
	auto bond = data.GetProduct();
	string oderType;
	switch (data.GetOrderType()) {
	case FOK: oderType = "FOK"; break;
	case MARKET: oderType = "MARKET"; break;
	case LIMIT: oderType = "LIMIT"; break;
	case STOP: oderType = "STOP"; break;
	case IOC: oderType = "IOC"; break;
	}
	cout << bond.GetProductId() << " OrderId: " << data.GetOrderId() << "\n"
		<< "    PricingSide: " << (data.GetSide() == BID ? "Bid" : "Ask")
		<< " OrderType: " << oderType << " IsChildOrder: " << (data.IsChildOrder() ? "True" : "False")
		<< "\n"
		<< "    Price: " << data.GetPrice() << " VisibleQuantity: " << data.GetVisibleQuantity()
		<< " HiddenQuantity: " << data.GetHiddenQuantity() << "\n" << endl;
	return;
}

void BondExecutionServiceConnector::Subscribe() {
	return;
}

//Service
class BondExecutionService: public ExecutionService<Bond> {
private:
	map<string, ExecutionOrder<Bond> > executionMap;   
	vector<ServiceListener<ExecutionOrder<Bond> >*> listenerList;      
    BondExecutionServiceConnector* bondExecutionServiceConnector;  
public:
    // constructor
    BondExecutionService(BondExecutionServiceConnector* _bondExecutionServiceConnector);

    // override base class
    virtual ExecutionOrder<Bond> & GetData(string id) override;     
	
    virtual void OnMessage(ExecutionOrder<Bond> &) override;      
	
    virtual void AddListener(ServiceListener<ExecutionOrder<Bond> > *listener) override;  
	
    virtual const vector<ServiceListener<ExecutionOrder<Bond> > *>& GetListeners() const override; 
	
    virtual void ExecuteOrder(ExecutionOrder<Bond> &order, Market market) override;   
	
    void AddAlgoExecution(const AlgoExecution& algo);	
};

class BondExecutionService;
BondExecutionService::BondExecutionService(BondExecutionServiceConnector* _bondExecutionServiceConnector){
	bondExecutionServiceConnector=_bondExecutionServiceConnector;
}

ExecutionOrder<Bond> & BondExecutionService::GetData(string id){
	return executionMap.at(id);
} 
	
void BondExecutionService::OnMessage(ExecutionOrder<Bond> &){
	return;
}     

void BondExecutionService::AddListener(ServiceListener<ExecutionOrder<Bond> > *listener){
	listenerList.push_back(listener);
	return;
} 
	
const vector<ServiceListener<ExecutionOrder<Bond> > *>& BondExecutionService::GetListeners() const{
	return listenerList;
}

	
void BondExecutionService::ExecuteOrder(ExecutionOrder<Bond> &order, Market market) {
	bondExecutionServiceConnector->Publish(order);
}

	
void BondExecutionService::AddAlgoExecution(const AlgoExecution& algo){
	auto executionOrder = algo.GetExecutionOrder();
    string id = executionOrder.GetProduct().GetProductId();
	
	if(executionMap.find(id)!=executionMap.end())
		executionMap.erase(id);
	executionMap.insert(pair<string,ExecutionOrder<Bond> >(id,executionOrder));

    for(auto& listener :listenerList) {
        listener->ProcessAdd(executionOrder );
    }
	return;
}

//Listener
class BondExecutionServiceListener : public ServiceListener<AlgoExecution> {
private:
	BondExecutionService* bondExecutionService;

public:
	// Constructor
	BondExecutionServiceListener(BondExecutionService* _bondExecutionService);

	// override base class
	virtual void ProcessAdd(AlgoExecution &data) override;

	virtual void ProcessRemove(AlgoExecution &data) override {};

	virtual void ProcessUpdate(AlgoExecution &data) override {};
};

BondExecutionServiceListener::BondExecutionServiceListener(BondExecutionService* _bondExecutionService) {
	bondExecutionService = _bondExecutionService;
}

void BondExecutionServiceListener::ProcessAdd(AlgoExecution &data) {
	auto order = data.GetExecutionOrder();
	bondExecutionService->AddAlgoExecution(data);
	bondExecutionService->ExecuteOrder(order, BROKERTEC);
	return;
}

#endif