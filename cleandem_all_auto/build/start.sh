echo "Start simulaztion"

echo "-1100 600 500" > pos_detector
cp pos_detector pos_detector_A

./exampleB1 run_A.mac

sleep 2

echo "-1100 600 4500" > pos_detector
cp pos_detector pos_detector_B

./exampleB1 run_B.mac

sleep 2

echo "8500 600 2500" > pos_detector
cp pos_detector pos_detector_C

./exampleB1 run_C.mac

sleep 2

echo "8500 600 -2500" > pos_detector
cp pos_detector pos_detector_D_T1

./exampleB1 run_D_T1.mac

sleep 2

echo "8500 600 -2500" > pos_detector
cp pos_detector pos_detector_D_T2

./exampleB1 run_D_T2.mac
