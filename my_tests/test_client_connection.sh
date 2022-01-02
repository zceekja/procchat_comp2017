./server &
PID1=$!
sleep 1

echo "CONNECT clientA domain1" | ./clientA &
PID2=$!
sleep 1


echo ""
if [[ -p ./domain1/clientA_WR ]] && [[ -p ./domain1/clientA_RD ]]
then
    echo "-------------------------------------------------------"
    echo "  PASSSED!, clientA_WR and clientA_RD exists."
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"
    echo "  FAILED!, name pipe doesn't exist"
    echo "-------------------------------------------------------"
fi
echo ""

sleep 1

kill $PID1
kill $PID2
rm gevent