#!/usr/bin/env python3
import sys
import random
import math

import tkinter as tk
from tkinter import messagebox, Frame, Tk, Button, Label


class Init_Game(Tk):

    #
    dart_count = 0
    score_total = 0

    def __init__(self):
        super().__init__()
        self.init_Layout()

    # initialize the main window layout
    # main_frame will be the parent to the canvas representing the board
    def init_Layout(self):

        self.title('Play Darts')
        self.minsize(1000, 800)  # responsive resizing
        self.rowconfigure(0, minsize=800, weight=1)
        self.columnconfigure(1, minsize=800, weight=1)

        main_frame = Frame(self, borderwidth=25, relief="raised",
                            width=900, height=900, bg="burlywood3")
        side_bar = Frame(self, borderwidth=10, relief="raised", bg="grey19")


        decr_btn = Button(side_bar, text="-", width=5,
                     height=2, command= self.decr_dart_count)
        incr_btn = Button(side_bar, text="+", width=5,
                            height=2, command= self.incr_dart_count)

        start_btn = Button(side_bar, text="Start", width=10,
                            height=3, command=self.throw_darts)
        # start_btn = Button(side_bar, text="Start", width=10,
        #                     height=3)
        self.dart_count_label = Label(side_bar, text=f"Throw  { + self.dart_count} \n Darts?", width=10, height=5)
        self.score_label = Label(side_bar, text="Score: \n\n" +
                            str(self.score_total), width=10, height=6)

        side_bar.grid(row=0, column=0, sticky="ns")
        main_frame.grid(row=0, column=1, sticky="nsew")
        # Set number of darts to throw
        incr_btn.grid(row=0, column=0, sticky="ne", padx=2, pady=15)
        decr_btn.grid(row=0, column=0, sticky="nw", padx=2, pady=15)
        self.dart_count_label.grid(row=1, column=0, sticky="nesw", padx=5,pady=5)

        # Display start button and score after thrown
        start_btn.grid(row=2, column=0, sticky="new", padx=5, pady=50)
        self.score_label.grid(row=3, column=0, sticky="sw", padx=5, pady=150)



    
    def incr_dart_count(self):
        self.dart_count += 1
        self.dart_count_label["text"] = f"Throw  { + self.dart_count} \n Darts?"

    def decr_dart_count(self):
        self.dart_count= self.dart_count- 1 if self.dart_count != 0 else 0
        self.dart_count_label["text"] = f"Throw  { + self.dart_count} \n Darts?"

    def throw_darts(self):
    # """Throw darts and calculate an estimate for pi"""

        try:                                        # divide by zero error handling
            N = self.dart_count
            # bd_wdth = 700
            # bd_hgt = 700
            # rad = 325
            inside = 0                              # dart_count the number of darts inside the circle
            for _ in range(0, N, 1):
                x = random.random()                 # generate new random x & y coordinates
                y = random.random()
                coords = math.sqrt(x*x + y*y)       # calculate coordinate of the dart thrown

                if(coords <= 1.0):                  # include values that land on the circle's border
                    inside += 1
                    # circle([x*500, y*500, 6], "yellow", "snow", 2)
                        # (darts inside/total thrown) * area of board
            self.score_total = (inside * 4) / N
            self.score_label["text"] = "Total Score:\n {0:5}".format(self.score_total)
        except ZeroDivisionError as err:
            messagebox.showwarning("Error: 0 darts","Cannot throw 0 darts")
            print(err)


class Board(Init_Game):
    def __init__(self):
        super().__init__()



def main():
    # root = tk.Tk()
    game = Init_Game()
    game.mainloop()

if __name__ == '__main__':
    main()