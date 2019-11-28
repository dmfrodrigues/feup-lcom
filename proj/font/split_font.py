import os
import glob
from PIL import Image

im = Image.open("Consolas.png")

W, H = im.size

Nw = 16
Nh = 6

i = 32



for h in range(Nh):
    t = int(H/Nh)*h
    b = int(H/Nh)*(h+1)
    for w in range(Nw):
        #l = int((W*w)/Nw)
        #r = int((W*(w+1))/Nw)
        l = 59*w
        r = 59*(w+1)
        im1 = im.crop((l,t,r,b))
        im1.save("%03d.png"%i)
        os.system("convert %03d.png xpm/%03d.xpm"%(i,i))
        os.system("rm %03d.png"%i)
        i += 1
