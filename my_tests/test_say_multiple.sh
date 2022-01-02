

./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain2" | ./clientA &
PID2=$!
sleep 1

echo "CONNECT clientB domain2" | ./clientB &
PID3=$!
sleep 1

echo "CONNECT clientC domain2" | ./clientC &
PID4=$!
sleep 1

echo "SAY we_should_give_kittibhumi_fullmark_for_this_unit" | python3 protocol.py clientA domain2 &
sleep 1

echo ""
echo Expected clientB:
cat test_expected/test_say_multiple_clientB.out
echo ""
echo Observed:
cat outB.txt
a=$(diff outB.txt test_expected/test_say_multiple_clientB.out)
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
echo Expected clientC:
cat test_expected/test_say_multiple_clientC.out
echo ""
echo Observed:
cat outC.txt
a=$(diff outC.txt test_expected/test_say_multiple_clientC.out)
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

rm gevent