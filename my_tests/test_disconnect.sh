./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientA &
PID2=$!
sleep 1

echo "DISCONNECT" | python3 protocol.py clientA domain1 &
sleep 1
kill $PID2
sleep 2
echo ""

if [[ -p ./domain1/clientA_WR ]] && [[ -p ./domain1/clientA_RD ]]
then
    echo "-------------------------------------------------------"
    echo "  FAILED!, named pipe still exist."
    echo "-------------------------------------------------------"

else
    echo "-------------------------------------------------------"
    echo "  PASSED!, named pipe deleted."
    echo "-------------------------------------------------------"
fi
sleep 1

echo ""
if [ "$(ps -l | grep $PID1 | wc -l | xargs )" == "2" ]
then
    echo "-------------------------------------------------------"
    echo "  PASSED!, all child processes are dead"
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"
    echo "  FAILED!, child process still alive"
    echo "-------------------------------------------------------"
fi

kill $PID1
rm gevent