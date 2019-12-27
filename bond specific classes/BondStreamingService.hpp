/**
 * BONDSTREAMINGSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDSTREAMINGSERVICE_HPP
#define BONDSTREAMINGSERVICE_HPP

#include "products.hpp"
#include "streamingservice.hpp"
#include "BondAlgoStreamingService.hpp"
#include "soa.hpp"
#include <string>
#include <iostream>

using namespace std;

//Connector
class BondStreamingServiceConnector : public Connector<PriceStream<Bond> > {
public:
	// constructor
	BondStreamingServiceConnector();

	virtual void Subscribe() override;

	virtual void Publish(PriceStream<Bond>& data) override;
};

BondStreamingServiceConnector::BondStreamingServiceConnector() {
}

void BondStreamingServiceConnector::Publish(PriceStream<Bond>& data) {
	auto bond = data.GetProduct();
	auto bidOrder = data.GetBidOrder();
	auto askOrder = data.GetOfferOrder();

	cout << bond.GetProductId() << "\n"
		<< "    BidOrder: " << "Price: " << bidOrder.GetPrice() << "  VisibleQuantity: " << bidOrder.GetVisibleQuantity()
		<< "  HiddenQuantity: " << bidOrder.GetHiddenQuantity() << "\n"
		<< "    offerOrder: " << "Price: " << askOrder.GetPrice() << "  VisibleQuantity: " << askOrder.GetVisibleQuantity()
		<< "  HiddenQuantity: " << askOrder.GetHiddenQuantity() << "\n"
		<< endl;

	return;
}

void BondStreamingServiceConnector::Subscribe() {
}



//Service
class BondStreamingService: public StreamingService<Bond>{
private:
	map<string, PriceStream<Bond> > streamMap;
	vector<ServiceListener<PriceStream<Bond> >*> listenerList;
	BondStreamingServiceConnector* bondStreamingServiceConnector;
public:
	// constructor
    BondStreamingService(BondStreamingServiceConnector* _bondStreamingServiceConnector);

    // override base class
    virtual PriceStream<Bond>& GetData(string id) override;   

    virtual void OnMessage(PriceStream<Bond>&) override;  
	
    virtual void AddListener(ServiceListener<PriceStream<Bond> > *listener) override;  
	
    virtual const vector<ServiceListener<PriceStream<Bond> > *>& GetListeners() const override;  
	
    virtual void PublishPrice(PriceStream<Bond>& priceStream) override;  
	
    void AddAlgo(const AlgoStream& algo);          
};

class BondStreamingService;
BondStreamingService::BondStreamingService(BondStreamingServiceConnector*_bondStreamingServiceConnector){
	bondStreamingServiceConnector=_bondStreamingServiceConnector;
	streamMap=map<string, PriceStream<Bond> >();
}

PriceStream<Bond>& BondStreamingService::GetData(string id){
	return streamMap.at(id);
}

void BondStreamingService::OnMessage(PriceStream<Bond>& stream){
	return;
}

void BondStreamingService::AddListener(ServiceListener<PriceStream<Bond> > *listener){
	listenerList.push_back(listener);
	return;
}

const vector<ServiceListener<PriceStream<Bond> > *>& BondStreamingService::GetListeners() const{
	return listenerList;
}

void BondStreamingService::PublishPrice(PriceStream<Bond>& priceStream) {
	bondStreamingServiceConnector->Publish(priceStream);
	return;
}
void BondStreamingService::AddAlgo(const AlgoStream& algo){
	auto stream=algo.GetPriceStream();
	auto id =stream.GetProduct().GetProductId();
	if(streamMap.find(id)!=streamMap.end())
		streamMap.erase(id);
	streamMap.insert(pair<string,PriceStream<Bond> >(id,stream));
	for(auto& listener:listenerList){
		listener->ProcessAdd(stream);
	}

	return;
}

//Listener
class BondStreamingServiceListener : public ServiceListener<AlgoStream> {
private:
	BondStreamingService* bondStreamingService;

public:
	// constructor
	BondStreamingServiceListener(BondStreamingService* _bondStreamingService);

	// override class
	virtual void ProcessAdd(AlgoStream& algoStream) override;

	virtual void ProcessRemove(AlgoStream &) override;

	virtual void ProcessUpdate(AlgoStream &) override;
};


BondStreamingServiceListener::BondStreamingServiceListener(BondStreamingService* _bondStreamingService) {
	bondStreamingService = _bondStreamingService;
}

void BondStreamingServiceListener::ProcessAdd(AlgoStream& algoStream) {
	bondStreamingService->AddAlgo(algoStream);
	auto priceStream = algoStream.GetPriceStream();
	bondStreamingService->PublishPrice(priceStream);
	return;
}

void BondStreamingServiceListener::ProcessRemove(AlgoStream& data) {
	return;
}

void BondStreamingServiceListener::ProcessUpdate(AlgoStream& data) {
	return;
}
#endif 