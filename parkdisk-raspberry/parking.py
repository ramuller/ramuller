#!/usr/bin/env python


from datetime import datetime
from datetime import timedelta
from time import sleep
import epd1in54
from PIL import Image,ImageDraw,ImageFont

font = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 72)

interval = 30

class inkDisplay():
    def __init__(self):
        print('In init')
        self.epd = epd1in54.EPD()
        self.epd.init(self.epd.lut_full_update)
        self.epd.Clear(0xFF)
        self.time_image = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)
        self.time_draw = ImageDraw.Draw(self.time_image)
        self.das_state = '/run/dash.stat'
        
    def show_time(self, t):
        self.time_draw.rectangle((10, 50, epd1in54.EPD_WIDTH, 120), fill = 255)
        self.time_draw.text((10, 50), '%02d:%02d' % (t.hour, t.minute), font = font, fill = 0)
        with open('') as f:
            s = f.read()
        self.epd.display(self.epd.getbuffer(self.time_image.rotate(180)))

def normalize(t, iv = interval):
    nt = t.replace(minute = (t.minute - t.minute % iv), second = 0, microsecond = 0)
    nt = nt + timedelta(minutes = iv)
    return nt  

def main():
    ink = inkDisplay()
    last_time = datetime.now()
    last_min = last_time.minute
    while True:
        t = datetime.now()
        nt = normalize(t)
        if last_time != nt:
            last_time = nt
            ink.show_time(nt)
            print('Display : %s' % str(t.second))
        else:
            if last_min != t.minute:
                print('Nodisplay : %s' % str(t))
                last_min = t.minute
        sleep(1)

if __name__== "__main__":
  main()
