./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientA &
PID2=$!
sleep 1

echo "CONNECT clientB domain1" | ./clientB &
PID3=$!
sleep 1

echo "CONNECT clientC domain1" | ./clientC &
PID4=$!
sleep 1

echo "SAYCONT hello" | python3 protocol.py clientA domain1 &
sleep 1

echo ""
echo Expected clientB message 1:
cat test_expected/test_say_con_multiple_clientB_out1.out
echo ""
echo Observed clientB message 1:
cat outB.txt
a=$(diff outB.txt test_expected/test_say_con_multiple_clientB_out1.out)
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi

echo ""
echo Expected clientC  message 1:
cat test_expected/test_say_con_multiple_clientC_out1.out
echo ""
echo Observed clientC message 1:
cat outC.txt
a=$(diff outC.txt test_expected/test_say_con_multiple_clientC_out1.out)
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi

echo "SAYCONT_E world" | python3 protocol.py clientA domain1 &
sleep 1

echo ""
echo Expected clientB message 2:
cat test_expected/test_say_con_multiple_clientB_out2.out
echo ""
echo Observed clientB message 2:
cat outB.txt
a=$(diff outB.txt test_expected/test_say_con_multiple_clientB_out2.out)
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi

echo ""
echo Expected clientC message 2:
cat test_expected/test_say_con_multiple_clientC_out2.out
echo ""
echo Observed clientC message 2:
cat outC.txt
a=$(diff outC.txt test_expected/test_say_con_multiple_clientC_out2.out)
if [[ $a == "" ]]
then
    echo "-------------------------------------------------------"
    echo "	Passed!, result matched!"
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"	
    echo "	Failed!, Output not matched"
    echo "-------------------------------------------------------"
fi
kill $PID1
kill $PID2
kill $PID3
kill $PID4

rm outA.txt
rm outB.txt
rm outC.txt
rm gevent
