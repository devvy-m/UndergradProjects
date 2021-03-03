#!/usr/bin/env python3

import sys, time
from socket import socket, AF_INET, SOCK_STREAM

sock = socket(AF_INET,SOCK_STREAM)
sock.connect( (sys.argv[1],int(sys.argv[2])) )  # note double parens
    
# print('sock=', sock)
while True:
    msgToSend = sys.stdin.readline()
    if not msgToSend:
        break
    print('client sending:', msgToSend)
    sock.sendall(msgToSend.encode("utf8"))   ##  utf8 is default; UTF-8 or bytes(msg,"UTF-8")
    msgRecvd = sock.recv(16)  ## add ,MSG_WAITALL) if you know 16 are coming
    if not msgRecvd:
        break
    msgRecvd = msgRecvd.decode()
    print('client recvd:', msgRecvd)
sock.close()