#!/bin/bash
./server &
PID1=$!
sleep 1
echo ""
if [[ -p gevent ]]
then
    echo "-------------------------------------------------------"
    echo "  PASSSED!, gevent exists."
    echo "-------------------------------------------------------"
else
    echo "-------------------------------------------------------"
    echo "  FAILED!, gevent doesn't exists."
    echo "-------------------------------------------------------"
fi

kill $PID1

rm gevent