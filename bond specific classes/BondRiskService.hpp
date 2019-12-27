/**
 * BONDRISKSERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDRISKSERVICE_HPP
#define BONDRISKSERVICE_HPP

#include "BondPositionService.hpp"
#include "riskservice.hpp"
#include "soa.hpp"

using namespace std;

class BondRiskService: public RiskService<Bond> {
private:
	map<string, PV01<Bond> > pv01Map;   
	vector<ServiceListener<PV01<Bond> >*> listenerList;    
public:
	// constructor
	BondRiskService();

	// override base class
	virtual PV01<Bond> & GetData(string id) override;  

	virtual double GetBucketedRisk(const BucketedSector<Bond> &sector) const override;

	virtual void AddListener(ServiceListener<PV01<Bond> > *listener) override;

	virtual void OnMessage(PV01<Bond> & pv01) override{return;}
	
	virtual void AddPosition(Position<Bond> &position) override;
	
	virtual const vector<ServiceListener<PV01<Bond> > *>& GetListeners() const override; 
};


BondRiskService::BondRiskService()
{
	pv01Map=map<string, PV01<Bond> >();
}

PV01<Bond> & BondRiskService::GetData(string id)
{
	return pv01Map.at(id);
}             

double BondRiskService::GetBucketedRisk(const BucketedSector<Bond> &sector) const {
	double res = 0;
	for (auto& product : sector.GetProducts()) {
		res += pv01Map.at(product.GetProductId()).GetPV01();
	}
	return res;
}

void BondRiskService::AddListener(ServiceListener<PV01<Bond> > *listener){
	listenerList.push_back(listener);
	return;
}

void BondRiskService::AddPosition(Position<Bond> &position) 
{
	auto bond = position.GetProduct();
	auto id = bond.GetProductId();
	auto it = pv01Map.find(id);
	long quantity = position.GetAggregatePosition();
	if (it != pv01Map.end()) 
	{
		(it->second).AddQuantity(quantity);
	}
	else {
		pv01Map.insert(pair<string, PV01<Bond> >(id, PV01<Bond>(bond, bondRisk[id], quantity)));
	}
	it = pv01Map.find(id);

	for (auto& listener : listenerList) 
	{
		listener->ProcessAdd(it->second);
	}
	return;
}

const vector<ServiceListener<PV01<Bond> > *>& BondRiskService::GetListeners() const{
	return listenerList;
}
	

//Listener
class BondRiskServiceListener : public ServiceListener<Position<Bond> > {
private:
	BondRiskService* bondRiskService;
public:
	//constructor
	BondRiskServiceListener(BondRiskService* _bondRiskService);

	// override base class
	virtual void ProcessAdd(Position<Bond> &data) override;

	virtual void ProcessRemove(Position<Bond> &data) override {};

	virtual void ProcessUpdate(Position<Bond> &data) override {};
};

void BondRiskServiceListener::ProcessAdd(Position<Bond> &data) 
{
	bondRiskService->AddPosition(data);
	return;
}

BondRiskServiceListener::BondRiskServiceListener(BondRiskService* _bondRiskService) 
{
	bondRiskService = _bondRiskService;
}

#endif