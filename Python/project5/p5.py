#!/usr/bin/env python3
# client file: P5CLI.py, make builds libp5.so
# TCP: use the reliable TCP protocol & spin up a server
# threadedTCP: threaded so it can handle multiple clients, requests
import ctypes, socketserver, sys

class MSGHandler(socketserver.BaseRequestHandler):
    def handle(self):
        while True:
            line = self.request.recv(16)  # recieve x amount of bytes
            if not line:                  # if nothing recieved, terminate the request
                self.request.close()
                break
            in_data = line.decode().strip().split() # store data in list of strings
            lib = ctypes.cdll.LoadLibrary(
                "/nfshome/rbutler/public/courses/cs/p5testfiles/libp5.so")  # import local c_library: SYSTEM^$
            # lib = ctypes.cdll.LoadLibrary("./libp5.so")    # import local c_library: LOCAL MACHINE
            msg_to_send = ""                                 # empty message

            try:                                             
                func_ptr = lib.__getitem__(in_data[0])       # C function pointer
                ret_value = func_ptr(int(in_data[1]))        # pass int argument to C function
                msg_to_send = "{0:} {1:} {2:} ".format(str(in_data[0]), str(
                    in_data[1]), ret_value)                  # include return value in message

            except AttributeError:        #throw exception err when a func doesn't exist in loaded library
                msg_to_send = "{0:9}{1:7}".format("**failed:", str(in_data[0]))
                print("**failed:", in_data[0], in_data[1], "function does not exist")
            self.request.sendall(msg_to_send.encode())       # send message back to client


#  listening on sever interface, sys.argv[1] is your port number
server = socketserver.ThreadingTCPServer(("", int(sys.argv[1])), MSGHandler)
print("serving at port", int(sys.argv[1]))      # print message in server
server.serve_forever()
