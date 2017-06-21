./waf --run "scratch/ahsimulation/ahsimulation"\
" --Nsta=20"\
" --NGroup=4"\
" --NRawSlotNum=5"\
" --DataMode=\"MCS2_0\""\
" --Rho=\"500\""\
" --ContentionPerRAWSlot=-1"\
" --PropagationLossExponent=3.67"\
" --MaxTimeOfPacketsInQueue=10000"\
" --SimulationTime=200"\
" --TrafficPacketSize=112"\
" --TrafficInterval=30000"\
" --TrafficIntervalDeviation=1000"\
" --TrafficType=\"tcpsensor\""\
" --SensorMeasurementSize=54"\
" --IpCameraMotionPercentage=1"\
" --IpCameraMotionDuration=10"\
" --IpCameraDataRate=8"\
" --BeaconInterval=102400"\
" --MinRTO=81920000"\
" --TCPConnectionTimeout=60000000"\
" --TCPSegmentSize=536"\
" --APAlwaysSchedulesForNextSlot=false"\
" --APScheduleTransmissionForNextSlotIfLessThan=5000"\
" --NRawSta=96"\
" --VisualizerIP=\"10.0.2.15\""\
" --VisualizerPort=7708"\
" --VisualizerSamplingInterval=1"\
" --APPcapFile=\"appcap\""\
" --NSSFile=\"test.nss\""\
" --Name=\"test\""
