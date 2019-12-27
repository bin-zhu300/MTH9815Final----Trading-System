/**
 * HISTORICALALLINQUIRIESCONNECTOR_HPP
 *
 * @author Bin Zhu
 */
#pragma once 

#ifndef HISTORICALALLINQUIRIESCONNECTOR_HPP
#define HISTORICALALLINQUIRIESCONNECTOR_HPP

using namespace std;

#include "soa.hpp"
#include "BondHistoricalDataService.hpp"
#include "products.hpp"
#include <string>
#include <iostream>

//Historical Inquiries
//Connector
class HistoricalAllInquiriesConnector: public Connector<Inquiry<Bond> >{
public:
    // ctor
    HistoricalAllInquiriesConnector(){};

    // do nothing, publish only
	virtual void Subscribe() override{}
	
	// print position
	virtual void Publish(Inquiry<Bond> & data) override;
};


void HistoricalAllInquiriesConnector::Publish(Inquiry<Bond> & data) {
	auto inquiry_id=data.GetInquiryId();
	auto id=data.GetProduct().GetProductId();
	auto price=data.GetPrice();
	auto quantity=data.GetQuantity();
	
	string side=(data.GetSide()==BUY?"BUY":"SELL");
	auto state=data.GetState();
	string state_;
	switch(state){
		case RECEIVED:state_="RECEIVED";break;
		case QUOTED:state_="QUOTED";break;
		case DONE:state_="DONE";break;
		case REJECTED:state_="REJECTED";break;
		case CUSTOMER_REJECTED:state_="CUSTOMER_REJECTED";break;
	}

    ofstream out;
    out.open("allinquiries.txt",ios::app);
	out<<inquiry_id<<","<<id<<","<<price<<","<<quantity<<","<<side<<","<<state_
		<<endl;

    return;
}

void BondHistoricalAllInquiriesService::PersistData(string persistKey, Inquiry<Bond>& data){
	auto id=data.GetInquiryId();
	if(dataMap.find(id)!=dataMap.end())
		dataMap.erase(id);
	dataMap.insert(pair<string,Inquiry<Bond> >(id,data));

	connector->Publish(data);
	return;
}

//Listener
class HistoricalAllInquriesListener : public ServiceListener<Inquiry<Bond> > {
private:
	BondHistoricalAllInquiriesService* service;

public:
	// ctor
	HistoricalAllInquriesListener(BondHistoricalAllInquiriesService * p) :service(p) {}

	virtual void ProcessAdd(Inquiry<Bond> &data) override {
		service->PersistData(data.GetInquiryId(), data);
		return;
	}
	virtual void ProcessRemove(Inquiry<Bond> &data) override {}
	virtual void ProcessUpdate(Inquiry<Bond>  &data) override {}

};

//HistoricalRisk
class HistoricalRiskConnector : public Connector<PV01<Bond> > {
public:
	// ctor
	HistoricalRiskConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(PV01<Bond>& data) override;
};


void HistoricalRiskConnector::Publish(PV01<Bond>& data) {
	auto bond = data.GetProduct();
	auto risk = data.GetPV01();

	ofstream out;
	out.open("risk.txt", ios::app);
	out << bond.GetProductId() << "," << risk << endl;

	return;
}

void BondHistoricalRiskService::PersistData(string persistKey, PV01<Bond>& data) {
	auto id = data.GetProduct().GetProductId();
	if (dataMap.find(id) != dataMap.end())
		dataMap.erase(id);
	dataMap.insert(pair<string, PV01<Bond> >(id, data));
	connector->Publish(data);
	return;
}


class HistoricalRiskListener : public ServiceListener<PV01<Bond> > {
private:
	BondHistoricalRiskService* service;

public:
	// ctor
	HistoricalRiskListener(BondHistoricalRiskService * p) :service(p) {}

	virtual void ProcessAdd(PV01<Bond> &data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(PV01<Bond> &data) override {}
	virtual void ProcessUpdate(PV01<Bond> &data) override {}

};


//Historical Execusion
class HistoricalExecutionConnector : public Connector<ExecutionOrder<Bond> > {
public:
	// ctor
	HistoricalExecutionConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(ExecutionOrder<Bond>& data) override;
};


void HistoricalExecutionConnector::Publish(ExecutionOrder<Bond>& data) {
	auto bond = data.GetProduct();
	string oderType;
	switch (data.GetOrderType()) {
	case FOK: oderType = "FOK"; break;
	case MARKET: oderType = "MARKET"; break;
	case LIMIT: oderType = "LIMIT"; break;
	case STOP: oderType = "STOP"; break;
	case IOC: oderType = "IOC"; break;
	}

	ofstream out;
	out.open("executions.txt", ios::app);
	out << bond.GetProductId() << "," << data.GetOrderId()
		<< "," << (data.GetSide() == BID ? "Bid" : "Ask")
		<< "," << oderType << "," << (data.IsChildOrder() ? "True" : "False")
		<< "," << data.GetPrice()
		<< "," << data.GetVisibleQuantity()
		<< "," << data.GetHiddenQuantity() << endl;

	return;
}

void BondHistoricalExecutionService::PersistData(string persistKey, ExecutionOrder<Bond>& data) {
	auto id = data.GetOrderId();
	if (dataMap.find(id) != dataMap.end())
		dataMap.erase(id);
	dataMap.insert(pair<string, ExecutionOrder<Bond> >(id, data));
	connector->Publish(data);
	return;
}

class HistoricalExecutionListener : public ServiceListener<ExecutionOrder<Bond> > {
private:
	BondHistoricalExecutionService* service;

public:
	// ctor
	HistoricalExecutionListener(BondHistoricalExecutionService * p) :service(p) {}

	virtual void ProcessAdd(ExecutionOrder<Bond> &data) override {
		service->PersistData(data.GetOrderId(), data);
		return;
	}
	virtual void ProcessRemove(ExecutionOrder<Bond> &data) override {}
	virtual void ProcessUpdate(ExecutionOrder<Bond> &data) override {}

};

//Historical Position
class HistoricalPositionConnector : public Connector<Position<Bond> > {
public:
	// ctor
	HistoricalPositionConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(Position<Bond>& data) override;
};


void HistoricalPositionConnector::Publish(Position<Bond>& data) {
	auto bond = data.GetProduct();
	auto position = data.GetAggregatePosition();

	ofstream out;
	out.open("positions.txt", ios::app);
	out << bond.GetProductId() << "," << position << endl;

	return;
}


void BondHistoricalPositionService::PersistData(string persistKey, Position<Bond>& data) {
	auto id = data.GetProduct().GetProductId();
	if (dataMap.find(id) != dataMap.end())
		dataMap.erase(id);
	dataMap.insert(pair<string, Position<Bond> >(id, data));
	connector->Publish(data);
	return;
}

class HistoricalPositionListener : public ServiceListener<Position<Bond> > {
private:
	BondHistoricalPositionService* service;

public:
	// ctor
	HistoricalPositionListener(BondHistoricalPositionService * p) :service(p) {}

	virtual void ProcessAdd(Position<Bond> &data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(Position<Bond> &data) override {}
	virtual void ProcessUpdate(Position<Bond>&data) override {}

};

//Historical Streaming
class HistoricalStreamingConnector : public Connector<PriceStream<Bond> > {
public:
	// ctor
	HistoricalStreamingConnector() {};

	// do nothing, publish only
	virtual void Subscribe() override {}

	// print position
	virtual void Publish(PriceStream<Bond> & data) override;
};


void HistoricalStreamingConnector::Publish(PriceStream<Bond> & data) {
	auto bond = data.GetProduct();
	auto bidOrder = data.GetBidOrder();
	auto askOrder = data.GetOfferOrder();

	ofstream out;
	out.open("streaming.txt", ios::app);
	out << bond.GetProductId() << ","
		<< bidOrder.GetPrice() << "," << bidOrder.GetVisibleQuantity()
		<< "," << bidOrder.GetHiddenQuantity()
		<< "," << askOrder.GetPrice() << "," << askOrder.GetVisibleQuantity()
		<< "," << askOrder.GetHiddenQuantity()
		<< endl;

	return;
}


void BondHistoricalStreamingService::PersistData(string persistKey, PriceStream<Bond>& data) {
	auto id = data.GetProduct().GetProductId();
	if (dataMap.find(id) != dataMap.end())
		dataMap.erase(id);
	dataMap.insert(pair<string, PriceStream<Bond> >(id, data));
	connector->Publish(data);
	return;
}

class HistoricalStreamingListener : public ServiceListener<PriceStream<Bond>  > {
private:
	BondHistoricalStreamingService* service;

public:
	// ctor
	HistoricalStreamingListener(BondHistoricalStreamingService * p) :service(p) {}

	virtual void ProcessAdd(PriceStream<Bond>  &data) override {
		service->PersistData(data.GetProduct().GetProductId(), data);
		return;
	}
	virtual void ProcessRemove(PriceStream<Bond>  &data) override {}
	virtual void ProcessUpdate(PriceStream<Bond>  &data) override {}

};

#endif