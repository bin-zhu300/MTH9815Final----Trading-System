/**
 * BONDHISTORICALDATASERVICE_HPP
 *
 * @author Bin Zhu
 */
#pragma once

#ifndef BONDHISTORICALDATASERVICE_HPP
#define BONDHISTORICALDATASERVICE_HPP

#include <iostream>
#include "historicaldataservice.hpp"
#include "BondStreamingService.hpp"
#include "BondRiskService.hpp"
#include "BondPositionService.hpp"
#include "BondExecutionService.hpp"
#include "BondInquiryService.hpp"

using namespace std;


class HistoricalPositionConnector;
class BondHistoricalPositionService: public HistoricalDataService<Position<Bond> > {
private:
	HistoricalPositionConnector* connector;
	map<string, Position<Bond> > dataMap;
public:
	// constructor
	BondHistoricalPositionService(HistoricalPositionConnector* _p):connector(_p){}

	// override base class
	virtual Position<Bond>  & GetData(string id) override {return dataMap.at(id);}

	virtual void OnMessage(Position<Bond>  & bond) override {}

	virtual void AddListener(ServiceListener<Position<Bond>  > *listener) override {}

	virtual const vector<ServiceListener<Position<Bond>  > *>& GetListeners() const override {}

	virtual void PersistData(string persistKey, Position<Bond>& data) override;
};

class HistoricalRiskConnector;
class BondHistoricalRiskService: public HistoricalDataService<PV01<Bond> > {
private:
	HistoricalRiskConnector* connector;
	map<string, PV01<Bond> > dataMap;
public:
	// constructor
	BondHistoricalRiskService(HistoricalRiskConnector* _p):connector(_p){}

	virtual PV01<Bond>  & GetData(string id) override {return dataMap.at(id);}

	virtual void OnMessage(PV01<Bond>  & bond) override {}

	virtual void AddListener(ServiceListener<PV01<Bond>  > *listener) override {}

	virtual const vector<ServiceListener<PV01<Bond>  > *>& GetListeners() const override {}

	virtual void PersistData(string persistKey, PV01<Bond>& data) override;
};

class HistoricalExecutionConnector;
class BondHistoricalExecutionService: public HistoricalDataService<ExecutionOrder<Bond> > {
private:
	HistoricalExecutionConnector* connector;
	map<string, ExecutionOrder<Bond> > dataMap;
public:
	// constructor
	BondHistoricalExecutionService(HistoricalExecutionConnector* _p):connector(_p){}

	// override base class
	virtual ExecutionOrder<Bond> & GetData(string id) override {return dataMap.at(id);}

	virtual void OnMessage(ExecutionOrder<Bond> & bond) override {}

	virtual void AddListener(ServiceListener<ExecutionOrder<Bond>  > *listener) override {}

	virtual const vector<ServiceListener<ExecutionOrder<Bond>  > *>& GetListeners() const override {}

	virtual void PersistData(string persistKey, ExecutionOrder<Bond>& data) override;
};

class HistoricalStreamingConnector;
class BondHistoricalStreamingService: public HistoricalDataService<PriceStream<Bond> > {
private:
	HistoricalStreamingConnector* connector;
	map<string, PriceStream<Bond> > dataMap;
public:
	// constructor
	BondHistoricalStreamingService(HistoricalStreamingConnector* _p):connector(_p){}

	// override base class
	virtual PriceStream<Bond> & GetData(string id) override {return dataMap.at(id);}

	virtual void OnMessage(PriceStream<Bond> & bond) override {}

	virtual void AddListener(ServiceListener<PriceStream<Bond>  > *listener) override {}

	virtual const vector<ServiceListener<PriceStream<Bond>  > *>& GetListeners() const override {};

	virtual void PersistData(string persistKey, PriceStream<Bond>& data) override;
};

class HistoricalAllInquiriesConnector;
class BondHistoricalAllInquiriesService: public HistoricalDataService<Inquiry<Bond> > {
private:
	HistoricalAllInquiriesConnector* connector;
	map<string, Inquiry<Bond> > dataMap;
public:
	// constructor
	BondHistoricalAllInquiriesService(HistoricalAllInquiriesConnector* _p):connector(_p){}

	// override base class
	virtual Inquiry<Bond> & GetData(string id) override {return dataMap.at(id);}

	virtual void OnMessage(Inquiry<Bond> & bond) override {}

	virtual void AddListener(ServiceListener<Inquiry<Bond>  > *listener) override {}

	virtual const vector<ServiceListener<Inquiry<Bond> > *>& GetListeners() const override {}

	virtual void PersistData(string persistKey, Inquiry<Bond>& data) override;
};


#endif
