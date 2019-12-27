/**
 * BONDPOSITIONSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDPOSITIONSERVICE_HPP
#define BONDPOSITIONSERVICE_HPP

#include "BondTradeBookingService.hpp"
#include "positionservice.hpp"
#include "soa.hpp"

using namespace std;

class BondPositionService: public PositionService<Bond> {
private:
	map<string,Position<Bond> >positionMap;
	vector<ServiceListener<Position<Bond> >*> listenerList;      
public:
	// constructor
	BondPositionService();

	// override base class
	virtual Position<Bond>& GetData(string id) override;
	
	virtual void OnMessage(Position<Bond> &position) override;                   
	
	virtual void AddListener(ServiceListener<Position<Bond> > *listener) override;  
	
	virtual const vector<ServiceListener<Position<Bond> > *>& GetListeners() const override;  
	
	virtual void AddTrade(const Trade<Bond> &trade) override;        
};

BondPositionService::BondPositionService()
{
	positionMap=map<string,Position<Bond> >();
}

Position<Bond>& BondPositionService::GetData(string id)
{
	return positionMap.at(id);
}

void BondPositionService::OnMessage(Position<Bond> &position)
{
	return;
}

void BondPositionService::AddListener(ServiceListener<Position<Bond> > *listener)
{
	listenerList.push_back(listener);
	return;
}

const vector<ServiceListener<Position<Bond> > *>& BondPositionService::GetListeners() const{
	return listenerList;
}  
	
void BondPositionService::AddTrade(const Trade<Bond> &trade)
{
	long quantity=trade.GetQuantity();
	if(trade.GetSide()==SELL)quantity=-quantity;
	auto bond=trade.GetProduct();
	auto id=bond.GetProductId();
	
	auto it=positionMap.find(id);
	if(it!=positionMap.end())
	{
		(it->second).AddPosition(trade.GetBook(),quantity);
	}
	else{
		positionMap.insert(pair<string,Position<Bond> >(id,Position<Bond>(bond)));
		AddTrade(trade);
		return;
	}
	
	for(auto& listener : listenerList) 
	{
        listener->ProcessAdd(it->second);  
    }
	return;
}

//Listener
class BondPositionServiceListener : public ServiceListener<Trade<Bond> > 
{
private:
	BondPositionService* bondPositionService;

public:
	// constructor
	BondPositionServiceListener(BondPositionService* _bondPositionService);

	// override base class
	virtual void ProcessAdd(Trade<Bond> &data);

	virtual void ProcessRemove(Trade<Bond> &data) override {}

	virtual void ProcessUpdate(Trade<Bond> &data) override {}
};

BondPositionServiceListener::BondPositionServiceListener(BondPositionService* _bondPositionService) 
{
	bondPositionService = _bondPositionService;
}

void BondPositionServiceListener::ProcessAdd(Trade<Bond> &data) 
{
	bondPositionService->AddTrade(data);
	return;
}
#endif