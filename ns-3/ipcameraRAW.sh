./waf --run "scratch/ahsimulation/ahsimulation"\
" --Nsta=20"\
" --NGroup=2"\
" --NRawSlotNum=5"\
" --DataMode=\"MCS2_8\""\
" --Rho=\"100\""\
" --ContentionPerRAWSlot=0"\
" --ContentionPerRAWSlotOnlyInFirstGroup=false"\
" --PropagationLossExponent=3.67"\
" --MaxTimeOfPacketsInQueue=10000"\
" --SimulationTime=200"\
" --TrafficPacketSize=4096"\
" --TrafficInterval=10000"\
" --TrafficIntervalDeviation=1000"\
" --TrafficType=\"tcpipcamera\""\
" --SensorMeasurementSize=54"\
" --IpCameraMotionPercentage=1"\
" --IpCameraMotionDuration=10"\
" --IpCameraDataRate=128"\
" --BeaconInterval=102400"\
" --MinRTO=81920000"\
" --TCPConnectionTimeout=60000000"\
" --TCPSegmentSize=3216"\
" --APAlwaysSchedulesForNextSlot=false"\
" --APScheduleTransmissionForNextSlotIfLessThan=5000"\
" --NRawSta=96"\
" --VisualizerIP=\"10.0.2.15\""\
" --VisualizerPort=7708"\
" --VisualizerSamplingInterval=1"\
" --APPcapFile=\"appcap\""\
" --NSSFile=\"test.nss\""\
" --Name=\"test\""
