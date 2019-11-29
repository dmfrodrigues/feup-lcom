import os
import glob
from PIL import Image

im = Image.open("Consolas.png")

W, H = im.size

Nw = 16
Nh = 6

i = 32

includes = ""
lst = ""

for j in range(i):
    if lst != "": lst += ","
    lst += "NULL"


for h in range(Nh):
    t = int(H/Nh)*h
    b = int(H/Nh)*(h+1)
    for w in range(Nw):
        #l = int((W*w)/Nw)
        #r = int((W*(w+1))/Nw)
        l = 59*w
        r = 59*(w+1)
        im1 = im.crop((l,t,r,b))
        im1.save("png/ascii%03d.png"%i)
        os.system("convert png/ascii%03d.png xpm/ascii%03d.xpm"%(i,i))
        
        includes += '#include "%03d.xpm"\n'%i
        if lst != "": lst += ","
        lst += "%03d_xpm"%i

        #DEV
        os.system("rm png/ascii%03d.png"%i)

        i += 1

os.system('sed -i "s/black/#000000/g" xpm/*.xpm')
