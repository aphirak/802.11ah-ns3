#ifndef NODESTATISTICS_H
#define NODESTATISTICS_H

#include "ns3/core-module.h"
#include "ns3/drop-reason.h"

using namespace std;
using namespace ns3;

class NodeStatistics {

public:
    Time TotalTransmitTime = Time();
    Time TotalReceiveTime = Time();
    Time TotalDozeTime = Time();
    Time TotalActiveTime = Time();
    
    Time interPacketDelayAtServer = Time(); ///ami
    Time interPacketDelayAtClient = Time(); ///ami
    std::vector<Time> m_interPacketDelayServer;
    std::vector<Time> m_interPacketDelayClient;
    std::vector<Time> m_time;
    long double GetInterPacketDelayDeviation(std::vector<Time>& delayVector);
    long double GetInterPacketDelayDeviationPercentage(std::vector<Time>& delayVector);
    Time GetAverageInterPacketDelay(std::vector<Time>& delayVector);
    float GetReliability (void);
    long double GetInterPacketDelayAtServer (void);
    long double GetInterPacketDelayAtClient (void);

    uint32_t m_prevPacketSeqServer;
    Time m_prevPacketTimeServer;
    uint32_t m_prevPacketSeqClient;
    Time m_prevPacketTimeClient;

    long NumberOfTransmissions = 0;
    long NumberOfTransmissionsDropped = 0;
    long NumberOfReceives = 0;
    long NumberOfReceivesDropped = 0;
    // the number of Rx that is dropped while STA was the destination
    long NumberOfReceiveDroppedByDestination = 0;
    
    // number of drops for any packets for between STA and AP by reason
    map<DropReason, long> NumberOfDropsByReason;
    // number of drops for any packets for between STA and AP by reason that occurred at AP
    map<DropReason, long> NumberOfDropsByReasonAtAP;

    long NumberOfSuccessfulPackets = 0;
    long NumberOfSuccessfulPacketsWithSeqHeader = 0;
    long NumberOfSentPackets = 0;
    
    long NumberOfSuccessfulRoundtripPackets = 0;
    long NumberOfSuccessfulRoundtripPacketsWithSeqHeader = 0;

    long getNumberOfDroppedPackets();

    Time TotalPacketSentReceiveTime = Time();

    // for jitter RMS - cumulative sum of abs differences
    uint64_t jitterAcc = 0;
    Time jitter = Time();
    long GetAverageJitter(void);

    long TotalPacketPayloadSize = 0;
    
    Time TotalPacketRoundtripTime = Time();
    long double getAveragePacketSentReceiveTime();
    long double getAveragePacketRoundTripTime(std::string trafficType);

    double getGoodputKbit(void);

    int EDCAQueueLength = 0;

    long TCPCongestionWindow = 0;
    Time TCPRTOValue = Time();
    bool TCPConnected = false;

    long NumberOfTCPRetransmissions = 0;
    long NumberOfTCPRetransmissionsFromAP = 0;

    long NumberOfMACTxRTSFailed = 0;
    long NumberOfMACTxMissedACK = 0;
    long NumberOfMACTxMissedACKAndDroppedPacket = 0;

    long NumberOfAPScheduledPacketForNodeInNextSlot = 0;
    long NumberOfAPSentPacketForNodeImmediately = 0;
    Time APTotalTimeRemainingWhenSendingPacketInSameSlot;

    Time getAverageRemainingWhenAPSendingPacketInSameSlot();

    long NumberOfCollisions = 0;
    long NumberOfTransmissionsCancelledDueToCrossingRAWBoundary = 0;

    long TotalNumberOfBackedOffSlots = 0;

    uint32_t TCPSlowStartThreshold = -1;

    double TCPEstimatedBandwidth = -1;

    Time TCPRTTValue = Time();

    long NumberOfBeaconsMissed = 0;

    uint16_t NumberOfTransmissionsDuringRAWSlot = 0;

    int getTotalDrops();

    Time FirmwareTransferTime;

    Time TimeStreamStarted = Time(0);
    long IPCameraTotalDataSent = 0;
    long IPCameraTotalDataReceivedAtAP = 0;
    Time IPCameraTotalTimeSent = Time(0);

    double getIPCameraSendingRate();
    double getIPCameraAPReceivingRate();

};

#endif /* NODESTATISTICS_H */

