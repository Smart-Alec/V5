import sys
import vex
from vex import *

brain = vex.Brain()

class Button:
    def __init__(self, x, y, width, height, callback, fill = None, border = None):
        self.x, self.y, self.width, self.height, self.callback, self.fill, self.border = x, y, width, height, callback, fill, border
        self.prev = False
    def render(self):
        brain.screen.set_pen_color(self.border)
        brain.screen.draw_rectangle(self.x, self.y, self.width, self.height, self.fill)
    def inside(self, x, y):
        if((x <= (self.x+self.width)) and (x >= self.x) and (y >= self.y) and (y <= (self.y+self.height))):
            return True
        else:
            return False
    def set_fill(self, color):
        self.fill = color;
    def event(self):
        self.callback()

def cali(self):
    first.set_fill("#FF0000")
toggled = False
def fornia(self):
    global toggled
    if toggled:
        toggled = False
        second.set_fill("#DDDDDD")
    else:
        toggled = True
        second.set_fill("#0000FF")
buttons = []
first = Button(100, 100, 100, 100, cali, "#DDDDDD", "#AAAAAA")
second = Button(200, 100, 100, 100, fornia, "#DDDDDD", "#00FF00")
buttons.append(first)
buttons.append(second)

just_pressed = False
while True:
    brain.screen.clear_screen("#FFFFFF")
    if brain.screen.pressing():
        just_pressed = True
    else:
        if just_pressed:
            for button in buttons:
                if button.inside(brain.screen.x_position(), brain.screen.y_position()):
                    button.event()
        just_pressed = False
    for button in buttons:
        button.render()
    sys.sleep(0.0166666666667)
    
