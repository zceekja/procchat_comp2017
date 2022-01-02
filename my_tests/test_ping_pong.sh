./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientA &
PID2=$!
sleep 1

kill $PID2

sleep 16
echo ""

ps -l | grep $PID1 | wc -l | xargs
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
echo ""
kill $PID1

rm gevent