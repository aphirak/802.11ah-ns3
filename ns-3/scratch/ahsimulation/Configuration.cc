#include "Configuration.h"

Configuration::Configuration() {
    
}

Configuration::Configuration(int argc, char** argv) {
    CommandLine cmd;
    cmd.AddValue("useIpv6", "Use Ipv6 (true/false)", useV6);
    cmd.AddValue("nControlLoops", "Number of control loops, number of stations is at least double of that", nControlLoops);
    cmd.AddValue("CoapPayloadSize", "Size of CoAP payload",coapPayloadSize);

    cmd.AddValue("Seed", "random seed", seed);
    cmd.AddValue("SimulationTime", "Simulation time in seconds", simulationTime);

    cmd.AddValue("Nsta", "number of total stations", Nsta);
    cmd.AddValue("NRawSta", "number of stations supporting RAW. If -1 it will be based on NSta, should be divisible by NGroup", NRawSta);
    cmd.AddValue("SlotFormat", "format of NRawSlotCount, -1 will auto calculate based on raw slot num", SlotFormat);
    cmd.AddValue("NRawSlotCount", "RAW slot duration, , -1 will auto calculate based on raw slot num", NRawSlotCount);
    cmd.AddValue("NRawSlotNum", "number of slots per RAW", NRawSlotNum);
    cmd.AddValue("NGroup", "number of RAW groups", NGroup);

    cmd.AddValue("ContentionPerRAWSlot", "Calculate the NSta and NRawSta based on the amount of contention there has to be in a RAW slot. A contention of 0 means stations are uncontended. If -1 then the specified NSta will be used", ContentionPerRAWSlot);
    cmd.AddValue("ContentionPerRAWSlotOnlyInFirstGroup", "If true only the first TIM group will contain stations, this is to prevent huge simulation times while every TIM group behaves the same (true/false)", ContentionPerRAWSlotOnlyInFirstGroup);

    cmd.AddValue("MaxTimeOfPacketsInQueue", "Max nr of seconds packets can remain in the DCA queue", MaxTimeOfPacketsInQueue);

    cmd.AddValue("BeaconInterval", "Beacon interval time in us", BeaconInterval);
    cmd.AddValue("APAlwaysSchedulesForNextSlot", "AP Always schedules for next slot (true/false)", APAlwaysSchedulesForNextSlot);
    cmd.AddValue("APScheduleTransmissionForNextSlotIfLessThan", "AP schedules transmission for next slot if slot time is less than (microseconds)", APScheduleTransmissionForNextSlotIfLessThan);

    cmd.AddValue("TrafficInterval", "Traffic interval time in ms", trafficInterval);
    cmd.AddValue("TrafficIntervalDeviation", "Traffic interval deviation time in ms, each interval will have a random deviation between - dev/2 and + dev/2", trafficIntervalDeviation);

    cmd.AddValue("TrafficPacketSize", "Size of packets to send in bytes. Default of -1 means the TCP Segment size - 100 bytes will be used", trafficPacketSize);
    cmd.AddValue("TrafficType", "Kind of traffic (udp, udpecho, tcpecho, tcpipcamera, tcpfirmware, tcpsensor, coap)", trafficType);

    cmd.AddValue("MinRTO", "Minimum retransmission timeout for TCP sockets in microseconds", MinRTO);
    cmd.AddValue("TCPConnectionTimeout", "TCP Connection timeout to use for all Tcp Sockets", TCPConnectionTimeout);

    cmd.AddValue("TCPSegmentSize", "TCP Segment size in bytes", TCPSegmentSize);
    cmd.AddValue("TCPInitialSlowStartThreshold", "TCP Initial slow start threshold in segments", TCPInitialSlowStartThreshold);
    cmd.AddValue("TCPInitialCwnd", "TCP Initial congestion window in segments", TCPInitialCwnd);


    cmd.AddValue("IpCameraMotionPercentage", "Probability the ip camera detects motion each second [0-1]", ipcameraMotionPercentage);
    cmd.AddValue("IpCameraMotionDuration", "Time in seconds to stream data when motion was detected", ipcameraMotionDuration);
    cmd.AddValue("IpCameraDataRate", "Data rate of the captured stream in kbps", ipcameraDataRate);

    cmd.AddValue("FirmwareSize", "Size of the firmware that will be sent to clients for update", firmwareSize);
    cmd.AddValue("FirmwareBlockSize", "The chunk size of a piece of firmware. The client has to acknowledge each chunk before the next will be sent", firmwareBlockSize);
    cmd.AddValue("FirmwareCorruptionProbability", "The probability that a firmware chunk gets corrupted and the client has to ask for it again", firmwareNewUpdateProbability);
    cmd.AddValue("FirmwareNewUpdateProbability", "The probability that a newer firmware version is available when the client polls the server (every trafficinterval)", firmwareNewUpdateProbability);

    cmd.AddValue("SensorMeasurementSize", "The size of the measurements taken by the sensor", sensorMeasurementSize);


    cmd.AddValue("DataMode", "Date mode (check MCStoWifiMode for more details) (format: MCSbw_mcs, e.g. MCS1_0 is OfdmRate300KbpsBW1Mhz)", DataMode);
//    cmd.AddValue("Datarate", "data rate in Mbps", datarate);
//cmd.AddValue("BandWidth", "bandwidth in MHz", bandWidth);

    cmd.AddValue("Rho", "maximal distance between AP and stations", rho);

    cmd.AddValue("PropagationLossExponent", "exponent gamma of the log propagation model, default values are outdoors", propagationLossExponent);
    cmd.AddValue("PropagationLossReferenceLoss", "reference loss of the log propagation model", propagationLossReferenceLoss);

    cmd.AddValue("VisualizerIP", "IP or hostname for the visualizer server, leave empty to not send data", visualizerIP);
    cmd.AddValue("VisualizerPort", "Port for the visualizer server", visualizerPort);
    cmd.AddValue("VisualizerSamplingInterval", "Sampling interval of statistics in seconds", visualizerSamplingInterval);


    cmd.AddValue("APPcapFile", "Name of the pcap file to generate at the AP, leave empty to omit generation", APPcapFile);

    cmd.AddValue("NSSFile", "Path of the nss file to write. Note: if a visualizer is active it will also save the nss file", NSSFile);
    cmd.AddValue("Name", "Name of the simulation", name);

    cmd.AddValue("CoolDownPeriod", "Period of no more traffic generation after simulation time (to allow queues to be processed) in seconds", CoolDownPeriod);


    cmd.Parse(argc, argv);
}
