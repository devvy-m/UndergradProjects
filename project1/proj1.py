#!/usr/bin/env python3
# Author: Devon Layton, Due: 07/09/20
# Program calculates a value of pi for when a dart is thrown at a dart board, prints
# the estimated pi value. 1 command line argument: # of darts to throw
# Area of the circle is pi: calculated by the ratio of darts inside circle over total darts thrown
# times the area of the board(square) which is 4. Radius of circle is 1
import sys
import random
import math

import tkinter as tk
from tkinter import messagebox

global dart_count
dart_count = 0
# Set up the window
window = tk.Tk()
window.title("Dart Game")
window.minsize(1000, 800)  # responsive resizing
window.rowconfigure(0, minsize=800, weight=1)
window.columnconfigure(1, minsize=800, weight=1)

main_frame = tk.Frame(window, borderwidth=25, relief="raised",
                      width=900, height=900, bg="burlywood3")
side_bar = tk.Frame(window, borderwidth=10, relief="raised", bg="grey19")
canvas = tk.Canvas(main_frame, width=800, height=800,
                   highlightthickness=0, bg="burlywood3")
canvas.pack()

########################## CANVAS FUNCTIONS #####################################


def circle(coord, f_clr, b_clr, wdth):
    canvas.create_oval(coord[0]-coord[2], coord[1]-coord[2], coord[0]+coord[2], coord[1]+coord[2], fill=f_clr,
                       outline=b_clr, width=wdth)


def circ_regions(coord):
    r = 310                                             # new radius for circle regions
    # loop over the 8 regions, use d to get new degree for arcs
    for d in range(8):
        r_or_b = "red" if d % 2 == 0 else "black"       # change color of each region
        canvas.create_arc(coord[0]-r, coord[1]-r, coord[0]+r, coord[1]+r,
                          style=tk.PIESLICE, start=d*45, extent=45, fill=r_or_b, outline="white", width=5)


def point_values():
    values = [5, 10, 20, 15, 25]                        # point values

    # q#'s represent 4 quadrants, t value +/- position's
    for t in range(2):
        q0 = canvas.create_text(
            600-(t*125), 325-(t*100), text=values[1+(2*t)], font=('bold', 40))
        canvas.itemconfig(q0, angle=0, fill="white")
        q1 = canvas.create_text(
            200+(t*125), 325-(t*100), text=values[2-(2*t)], font=('bold', 40))
        canvas.itemconfig(q1, angle=0, fill="white")
        q2 = canvas.create_text(
            200+(t*125), 475+(t*100), text=values[1+(2*t)], font=("bold", 40))
        canvas.itemconfig(q2, angle=0, fill="white")
        q3 = canvas.create_text(
            600-(t*125), 475+(t*100), text=values[2-(2*t)], font=("bold", 40))
        canvas.itemconfig(q3, angle=0, fill="white")
        bulls_i = canvas.create_text(
            400, 400, text=values[4], font=('bold', 30))
        canvas.itemconfig(bulls_i, angle=0, fill="white")

########################## Functionality ########################################


def incr_dart_count():
    global dart_count
    dart_count += 1
    dart_count_label["text"] = f"Throw  { + dart_count} \n Darts?"

def decr_dart_count():
    global dart_count
    dart_count= dart_count- 1 if dart_count != 0 else 0
    dart_count_label["text"] = f"Throw  { + dart_count} \n Darts?"


def throw_darts():
    """Throw darts and calculate an estimate for pi"""
    try:                                        # divide by zero error handling
        N = dart_count
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
                circle([x*500, y*500, 6], "yellow", "snow", 2)
                    # (darts inside/total thrown) * area of board
        score_total = (inside * 4) / N
        score_label["text"] = f"Total Score:\n { + score_total}"
    except ZeroDivisionError as err:
        messagebox.showwarning("Error: 0 darts","Cannot throw 0 darts")
        print(err)




########################### Main ###################################
# dart_count = 0
score_total = 0

# Set up widgets

decr_btn = tk.Button(side_bar, text="-", width=5,
                     height=2, command= decr_dart_count)
incr_btn = tk.Button(side_bar, text="+", width=5,
                     height=2, command= incr_dart_count)

start_btn = tk.Button(side_bar, text="Start", width=10,
                      height=3, command=throw_darts)
dart_count_label = tk.Label(side_bar, text=f"Throw  { + dart_count} \n Darts?", width=10, height=5)
score_label = tk.Label(side_bar, text="Score: \n\n" +
                       str(score_total), width=10, height=6)

# Create dartboard
canvas.create_rectangle(50, 50, 750, 750, fill="forest green")
coords = [[400, 400, 325], [400, 400, 32.5]]            # coordinates for circles to be used in func calls
circle(coords[0], "grey12", "SlateGray4", 25)           # outer circle
circ_regions(coords[0])
circle(coords[1], "red4", "PaleGreen4", 3)              # inner circle
point_values()

# Set up grid areas
main_frame.grid(row=0, column=1, sticky="nsew")
side_bar.grid(row=0, column=0, sticky="ns")

# Set number of darts to throw
incr_btn.grid(row=0, column=0, sticky="ne", padx=2, pady=15)
decr_btn.grid(row=0, column=0, sticky="nw", padx=2, pady=15)
dart_count_label.grid(row=1, column=0, sticky="nesw", padx=5,pady=5)

# Display start button and score after thrown
start_btn.grid(row=2, column=0, sticky="new", padx=5, pady=50)
score_label.grid(row=3, column=0, sticky="sw", padx=5, pady=150)

window.mainloop()
