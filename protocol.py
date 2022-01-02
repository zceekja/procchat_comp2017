import os
import sys

bdata = b''
data = input()
command = data.split()
if len(command) == 2 :
    if command[0] == "SAY":
        if len(command[1]) <=2000:
            #print("say")
            bdata += b'\x01\x00'
            bdata += command[1].encode()
            bdata += ((2048 - len(bdata))*b'\x00')
    elif command[0] == "SAYCONT":
        if len(command[1]) <=2000:
            #print("saycont")

            bdata += b'\x02\x00'
            bdata += command[1].encode()
            bdata += ((2048 - len(bdata))*b'\x00')
    elif command[0] == "SAYCONT_E":
        if len(command[1]) <=2000:
            #print("saycont_e")
            bdata += b'\x02\x00'
            bdata += command[1].encode()
            bdata += ((2047 - len(bdata))*b'\x00')
            bdata += b'\xFF'
elif len(command) == 1:
    if command[0] == "KILL":
        #print("kill")
        bdata += b'\x09\x00'
        bdata += ((2047 - len(bdata))*b'\x00')
        bdata += b'\xFF'
    elif command[0] == "DISCONNECT":
        #print("kill")
        bdata += b'\x07\x00'
        bdata += ((2047 - len(bdata))*b'\x00')
        bdata += b'\xFF'

pipe_rd = sys.argv[2]+"/" + sys.argv[1] +"_RD"

file_to_write = open(pipe_rd,'wb')
file_to_write.write(bdata)
#print(len(bdata))
#print("wrote data")
file_to_write.close()

    