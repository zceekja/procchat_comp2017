./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientA &
PID2=$!
sleep 1

echo "CONNECT clientB domain1" | ./clientB &
PID3=$!
sleep 1

echo "SAY helloworld" | python3 protocol.py clientA domain1 &
sleep 1
echo ""
echo Expected:
cat test_expected/test_say_clientB.out
echo ""
echo Observed:
cat outB.txt
a=$(diff outB.txt test_expected/test_say_clientB.out)
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

rm gevent
