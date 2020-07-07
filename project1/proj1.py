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

window = tk.Tk()
main_frame = tk.Frame(window, borderwidth = 10, relief="flat",width=500, height=500, bg="linen")
side_bar = tk.Frame(window, borderwidth=10, relief="raised", bg="grey19")
w = tk.Canvas(main_frame, width=500, height=500, bg="burlywood3")
w.pack()

def circle(canvas, x, y, r,f_clr, b_clr, wdth):
    return canvas.create_oval(x-r, y-r, x+r, y+r, fill=f_clr, outline=b_clr, width=wdth)
 

def throw_darts():
    """Throw darts and calculate an estimate for pi"""
    N = int(
        sys.argv[1])                # command line args are strings, convert to integer
    inside = 0                          # count the number of darts inside the circle
    for i in range(0, N, 1):
        x = random.random()             # generate new random x & y coordinates
        y = random.random()
        # calculate coordinate of the dart thrown
        coords = math.sqrt(x*x + y*y)
        
        if(coords <= 1.0):              # include values that land on the circle's border
            inside += 1
            circle(w,x*500,y*500,6,"yellow", "snow", 2)

    # (darts inside/total thrown) * area of board
    piEstimate = (inside * 4) / N
    pi_label["text"] = f"Pi: { + piEstimate}"
    print("pi estimate = ", piEstimate)  # print result


# Set up the window
# window = tk.Tk()
window.title("Dart Game")
window.minsize(150,250)          #responsive resizing
window.rowconfigure(0, minsize=800, weight=1)
window.columnconfigure(1, minsize=800, weight=1)

piEstimate = 0

# Set up widgets
# main_frame = tk.Frame(window, borderwidth = 10, relief="flat",width=500, height=500, bg="linen")
# side_bar = tk.Frame(window, borderwidth=10, relief="raised", bg="grey19")

start_btn = tk.Button(side_bar, text="Throw Darts", width=10,
                     height=5, command=throw_darts)
pi_label = tk.Label(side_bar, text="Pi: " + str(piEstimate), width=10, height= 5)


# w = tk.Canvas(main_frame, width=500, height=500, bg="burlywood4")
# w = tk.Canvas(main_frame, width=500, height=500)

# w.pack()

circle(w, 250,250,235,"grey12", "SlateGray4", 6 )
circle(w, 250,250,35,"red4", "PaleGreen4", 3 )


w.create_rectangle(0, 250, 500, 250, fill="yellow")
# w.create_rectangle(250, 0, 250, 500, fill="yellow")
w.create_line(250, 0, 250, 500, fill="#476042", width=3)
w.create_line(0, 250, 500, 250, fill="#476042", width=3)
w.create_line(0,0, 500, 500, fill="#476042", width=3)
w.create_line(500,0, 0, 500, fill="#476042", width=3)

# w.create_line(150, 80, 200, 100, fill="#476042", width=3)

#Set up grid
main_frame.grid(row=0, column=1, sticky="nsew")
side_bar.grid(row=0, column=0, sticky="ns")

start_btn.grid(row=0, column=0, sticky="ew", padx=5, pady=5)
pi_label.grid(row=1, column=0, sticky="nsw",padx=5, pady=10)


window.mainloop()
