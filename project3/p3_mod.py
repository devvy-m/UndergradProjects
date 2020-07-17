#!/usr/bin/env python3

"""P3MOD"""


def find_starts():
    i = 0
    line = yield
    print("recvd: ", line)
    # out = []

    while True:
        # line = yield
        # print("recvd: ", line)
        out = []
        for i in range(len(line)):
            if line[i:i+3] in (["atg", "gtg"]):
                out.insert(i, i)
        val = yield tuple(out)
        # out.clear()
        print(val)

# def find_starts():
#     line = yield
#     print("Recvd: ", line)
#     # while True:
#     #     x = set(["atg", "gtg"])
#     count = " "
#     for i in line:
#         if ("atg" or "gtg") in line:
#             count += str(i)
#             # val = yield str(count)
#     # return count
#     yield count
