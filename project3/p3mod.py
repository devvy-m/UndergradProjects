#!/usr/bin/env python3

"""P3MOD"""


def find_starts():
    # when primed, create empty string that be used it next gen calls
    in_str = ""
    while True:
        # prev data is remembered so update to an empty list
        out_data = []
        # list comp. finding start codons
        [out_data.insert(i, (in_str[i:i+3], i)) for i in range(len(in_str))
         if in_str[i:i+3] in (["atg", "gtg", "ATG", "GTG"])]        
        # if list is empty, return an empty tuple
        final_out = [()] if not out_data else out_data
        # yield data to main & in_str assigned new seq with main's next .send(seq)
        in_str = yield final_out
