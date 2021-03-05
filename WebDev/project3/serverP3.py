#!/usr/bin/env python3

import asyncio
import websockets


async def handle_connection(ws, path):
    while True:
        try:
            charMsg = await ws.recv()
        except:
            charMsg = None
        print("recvd:", charMsg)

        if charMsg == "cand1":
            count=[2]
            with open("votecnts.txt", "r") as inhandler:
                data = inhandler.read()
                count = data.split(" ") 
                count[0] = str(int(count[0])+1)
            with open("votecnts.txt", "w") as outhandler:
                outhandler.writelines([count[0], " ", count[1]])
            await ws.send("Candidate 1: " + str(int(count[0])))


        elif charMsg == "cand2":
            count=[2]
            with open("votecnts.txt", "r") as inhandler:
                data = inhandler.read()
                count = data.split(" ") 
                count[1] = str(int(count[1])+1)
            with open("votecnts.txt", "w") as outhandler:
                outhandler.writelines([count[0], " ", count[1]])
            await ws.send("Candidate 2: " + str(int(count[1])))
            
        elif charMsg == "counts":
            count=[2]
            with open("votecnts.txt", "r") as inhandler:
                data = inhandler.read()
                count = data.split(" ") 
                count[0] = str(int(count[0]))
                count[1] = str(int(count[1]))
            await ws.send("Candidate 1: " + str(count[0]) + "   Candidate 2: " + str(count[1]))

        else:
            await ws.send("I don't understand")
        print("replied to",charMsg)

start_server = websockets.serve(handle_connection, "localhost", 3000)
print("SERVER STARTED")
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
print("SERVER ENDING")
