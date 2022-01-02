./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientB &
echo ""
echo "This PING test will finish in 15 seconds, Please wait!"
PID2=$!
sleep 16


echo ""
echo Expected:
cat test_expected/test_ping.out
echo ""
echo Observed:
cat outB.txt
a=$(diff outB.txt test_expected/test_ping.out)
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

rm gevent