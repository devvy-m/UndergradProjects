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


def circle(coord, f_clr, b_clr, wdth):
    canvas.create_oval(coord[0]-coord[2], coord[1]-coord[2], coord[0]+coord[2], coord[1]+coord[2], fill=f_clr,
                       outline=b_clr, width=wdth)


def circ_regions(coord):
    r = 310                                             # new radius for circle regions
    for d in range(8):                                  # loop over the 8 regions, use d to get new degree for arcs
        r_or_b = "red" if d % 2 == 0 else "black"       # change color of each region
        canvas.create_arc(coord[0]-r, coord[1]-r, coord[0]+r, coord[1]+r,
                          style=tk.PIESLICE, start=d*45, extent=45, fill=r_or_b, outline="white", width=5)



def point_values():
    values = [5,10,20,15,25]                        # point values

    for t in range(2):                              # q#'s represent 4 quadrants, t value +/- position's
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

def throw_darts():
    """Throw darts and calculate an estimate for pi"""
    N = int(
        sys.argv[1])                        # command line args are strings, convert to integer
    inside = 0                              # count the number of darts inside the circle
    for i in range(0, N, 1):
        x = random.random()                 # generate new random x & y coordinates
        y = random.random()
        # calculate coordinate of the dart thrown
        coords = math.sqrt(x*x + y*y)

        if(coords <= 1.0):                  # include values that land on the circle's border
            inside += 1
            circle([x*900, y*900, 6], "yellow", "snow", 2)

    # (darts inside/total thrown) * area of board
    piEstimate = (inside * 4) / N
    pi_label["text"] = f"Pi: { + piEstimate}"
    print("pi estimate = ", piEstimate)  # print result


piEstimate = 0

# Set up widgets
start_btn = tk.Button(side_bar, text="Throw Darts", width=10,
                      height=5, command=throw_darts)
pi_label = tk.Label(side_bar, text="Pi: " +
                    str(piEstimate), width=10, height=5)

canvas.create_rectangle(50, 50, 750, 750, fill="firebrick3")

# coordinates for circles to be used in func calls
coords = [[400, 400, 325], [400, 400, 32.5]]
circle(coords[0], "grey12", "SlateGray4", 25)  # outer circle
circ_regions(coords[0])
circle(coords[1], "red4", "PaleGreen4", 3)  # inner circle
point_values()


# Set up grid
main_frame.grid(row=0, column=1, sticky="nsew")
side_bar.grid(row=0, column=0, sticky="ns")

start_btn.grid(row=0, column=0, sticky="ew", padx=5, pady=5)
pi_label.grid(row=1, column=0, sticky="nsw", padx=5, pady=10)


window.mainloop()
