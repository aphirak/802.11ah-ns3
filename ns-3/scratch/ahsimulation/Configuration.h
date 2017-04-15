#pragma once

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/extension-headers.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <sys/stat.h>
#include <string>

using namespace ns3;
using namespace std;

struct Configuration {
	/*
	 * CoAP configuration parameters
	 * */
	bool useV6 = false; //false
	uint32_t nControlLoops = 1;
	uint32_t coapPayloadSize = 20;

	double simulationTime = 200;
	uint32_t seed = 1;
	uint32_t Nsta = 4; //1
	int NRawSta = -1; //-1
	int SlotFormat = -1; //0;
	int NRawSlotCount = -1; //162;
	uint32_t NRawSlotNum = 4;
	uint32_t NGroup = 1;
	uint32_t BeaconInterval = 20480; //102400 25600

	uint32_t MinRTO = 81920000; //819200
	uint32_t TCPConnectionTimeout = 6000000;
	uint32_t TCPSegmentSize = 536; //536
	uint32_t TCPInitialSlowStartThreshold = 0xffff;
	uint32_t TCPInitialCwnd = 1;

	int ContentionPerRAWSlot = -1; //-1
	bool ContentionPerRAWSlotOnlyInFirstGroup = true; //false

	double propagationLossExponent = 3.67; //3.76
	double propagationLossReferenceLoss = 8;

	bool APAlwaysSchedulesForNextSlot = false;
	uint32_t APScheduleTransmissionForNextSlotIfLessThan = 5000;

	string DataMode = "MCS2_0"; //MCS2_8

	string visualizerIP = "10.0.2.15"; /// prayan string ""
	int visualizerPort = 7707;
	double visualizerSamplingInterval = 1;

	string rho = "500.0"; //100

	string name = "test"; //payan string

	string APPcapFile = "appcap"; //prayan string
	string NSSFile = "test.nss";

	uint32_t trafficInterval = 215; //ms
	uint32_t trafficIntervalDeviation = 10; //1000 discuss with Jeroen

	int trafficPacketSize = -1; //-1
	string trafficType = "coap"; //tcpfirmware

	double ipcameraMotionPercentage = 1; //0.1
	uint16_t ipcameraMotionDuration = 10; //60
	uint16_t ipcameraDataRate = 8; //20

	uint32_t firmwareSize = 1024 * 500;
	uint16_t firmwareBlockSize = 1024;
	double firmwareNewUpdateProbability = 0.01;
	double firmwareCorruptionProbability = 0.01;
	uint32_t firmwareVersionCheckInterval = 1000;

	uint16_t sensorMeasurementSize = 54; //1024

	uint16_t MaxTimeOfPacketsInQueue = 1000; //100
	UintegerValue maxNumberOfPackets = 4294967295u; ///4294967295u //ami

	uint16_t CoolDownPeriod = 0; //60

	Configuration();
	Configuration(int argc, char** argv);

};