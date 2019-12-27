/**
 * Main 
 *
 * @author Bin Zhu
 */
#pragma warning( disable : 4716)

#include <iostream>
#include "BondAlgoStreamingService.hpp"
#include "BondPricingService.hpp"
#include "BondProductService.hpp"
#include "BondStreamingService.hpp"
#include "BondAlgoExecutionService.hpp"
#include "BondExecutionService.hpp"
#include "BondMarketDataService.hpp"
#include "MarketdataGenerator.hpp"
#include "PricesGenerator.hpp"
#include "BondPositionService.hpp"
#include "BondRiskService.hpp"
#include "BondTradeBookingService.hpp"
#include "TradesGenerator.hpp"
#include "GUIService.hpp"
#include "BondInquiryService.hpp"
#include "Historical.hpp"
#include "BondHistoricalDataService.hpp"
#include "InquriesGenerator.hpp"


int main() {
	// Generator
	PricesGenerator PG; 
	PG();
	MarketdataGenerator MG; 
	MG();
	TradesGenerator TG; 
	TG();
	InquriesGenerator IG; 
	IG();
	
	vector<Bond> bondsType;
	for(int i=0; i<6; i++)
		bondsType.push_back(Bond(bondCusip[i],CUSIP,"T",bondCoupon[i], bondMaturity[i]));

	BondProductService* bondProductService = new BondProductService();
	for(int i=0; i<6; i++) bondProductService->Add(bondsType[i]);

	BondPricingService* bondPricingService=new BondPricingService();
	BondPricingServiceConnector* bondPricingServiceConnector=new BondPricingServiceConnector(bondPricingService,bondProductService);
	BondAlgoStreamingService* bondAlgoStreamingService=new BondAlgoStreamingService();
	BondAlgoStreamingServiceListener* bondAlgoStreamingServiceListener=new BondAlgoStreamingServiceListener(bondAlgoStreamingService);	
	
	bondPricingService->AddListener(bondAlgoStreamingServiceListener);
	
	BondStreamingServiceConnector* bondStreamingServiceConnector=new BondStreamingServiceConnector();
	BondStreamingService* bondStreamingService=new BondStreamingService(bondStreamingServiceConnector);
	BondStreamingServiceListener* bondStreamingServiceListener = new BondStreamingServiceListener(bondStreamingService);
	
	bondAlgoStreamingService->AddListener(bondStreamingServiceListener);
	
	BondMarketDataService* bondMarketDataService=new BondMarketDataService(); 
	BondMarketDataServiceConnector* bondMarketDataServiceConnector=new BondMarketDataServiceConnector(bondMarketDataService,bondProductService);
	BondAlgoExecutionService* bondAlgoExecutionService=new BondAlgoExecutionService();
	BondAlgoExecutionServiceListener* bondAlgoExecutionServiceListener=new BondAlgoExecutionServiceListener(bondAlgoExecutionService);
	
	bondMarketDataService->AddListener(bondAlgoExecutionServiceListener);
	
	BondExecutionServiceConnector* bondExecutionServiceConnector=new BondExecutionServiceConnector();
	BondExecutionService* bondExecutionService=new BondExecutionService(bondExecutionServiceConnector);
	BondExecutionServiceListener* bondExecutionServiceListener=new BondExecutionServiceListener(bondExecutionService);
	
	bondAlgoExecutionService->AddListener(bondExecutionServiceListener);
	
	BondTradeBookingService* bondTradeBookingService=new BondTradeBookingService();
	BondTradingBookingServiceConnector* bondTradingBookingServiceConnector=new BondTradingBookingServiceConnector(bondTradeBookingService,bondProductService);
	BondPositionService* bondPositionService=new BondPositionService();
	BondPositionServiceListener* bondPositionServiceListener=new BondPositionServiceListener(bondPositionService);
	
	bondTradeBookingService->AddListener(bondPositionServiceListener);
	
	BondRiskService* bondRiskService=new BondRiskService();
	BondRiskServiceListener* bondRiskServiceListener=new BondRiskServiceListener(bondRiskService);
	
	bondPositionService->AddListener(bondRiskServiceListener);
	
	BondTradeBookingServiceListener* bondTradeBookingServiceListener=new BondTradeBookingServiceListener(bondTradeBookingService);
	
	bondExecutionService->AddListener(bondTradeBookingServiceListener);
	
	GUIServiceConnector* gUIServiceConnector=new GUIServiceConnector();
	GUIService* gUIService=new GUIService(gUIServiceConnector);
	GUIServiceListener* gUIServiceListener=new GUIServiceListener(gUIService);
	
	bondPricingService->AddListener(gUIServiceListener);
	
	BondInquiryService* bondInquiryService=new BondInquiryService();
	BondInquiryServiceConnector* bondInquiryServiceConnector=new BondInquiryServiceConnector(bondInquiryService,bondProductService);
	HistoricalPositionConnector* historicalPositionConnector=new HistoricalPositionConnector();
	BondHistoricalPositionService* bondHistoricalPositionService=new BondHistoricalPositionService(historicalPositionConnector);
	HistoricalPositionListener* historicalPositionListener=new HistoricalPositionListener(bondHistoricalPositionService);
	
	bondPositionService->AddListener(historicalPositionListener);
	
	HistoricalRiskConnector* historicalRiskConnector=new HistoricalRiskConnector();
	BondHistoricalRiskService* bondHistoricalRiskService=new BondHistoricalRiskService(historicalRiskConnector);
	HistoricalRiskListener* historicalRiskListener=new HistoricalRiskListener(bondHistoricalRiskService);
	
	bondRiskService->AddListener( historicalRiskListener);
	
	HistoricalExecutionConnector* historicalExecutionConnector=new HistoricalExecutionConnector();
	BondHistoricalExecutionService* bondHistoricalExecutionService=new BondHistoricalExecutionService(historicalExecutionConnector);
	HistoricalExecutionListener* historicalExecutionListener=new HistoricalExecutionListener(bondHistoricalExecutionService);
	
	bondExecutionService->AddListener(historicalExecutionListener);
	
	HistoricalStreamingConnector* historicalStreamingConnector=new HistoricalStreamingConnector();
	BondHistoricalStreamingService* bondHistoricalStreamingService=new BondHistoricalStreamingService(historicalStreamingConnector);
	HistoricalStreamingListener* historicalStreamingListener=new HistoricalStreamingListener(bondHistoricalStreamingService);
	
	bondStreamingService->AddListener(historicalStreamingListener);
	
	HistoricalAllInquiriesConnector* historicalAllInquiriesConnector=new HistoricalAllInquiriesConnector();
	BondHistoricalAllInquiriesService* bondHistoricalAllInquiriesService=new BondHistoricalAllInquiriesService(historicalAllInquiriesConnector);
	HistoricalAllInquriesListener* historicalAllInquriesListener=new HistoricalAllInquriesListener(bondHistoricalAllInquiriesService);
	
	bondInquiryService->AddListener(historicalAllInquriesListener);
	bondPricingServiceConnector->Subscribe();
	bondMarketDataServiceConnector->Subscribe();
	bondTradingBookingServiceConnector->Subscribe();
	bondInquiryServiceConnector->Subscribe();
	
	return 0;
}
