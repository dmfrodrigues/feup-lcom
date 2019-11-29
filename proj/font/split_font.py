import os
import sys
import glob
from PIL import Image

f = sys.argv[1]
WCHAR = int(sys.argv[2])
HCHAR = int(sys.argv[3])
Nw    = int(sys.argv[4])
Nh    = int(sys.argv[5])
i     = int(sys.argv[6])

os.system("mkdir -p %s/png"%f)
im = Image.open("%s/%s.png"%(f,f))
W, H = im.size
for h in range(Nh):
    t = HCHAR*h
    b = HCHAR*(h+1)
    for w in range(Nw):
        l = WCHAR*w
        r = WCHAR*(w+1)
        im1 = im.crop((l,t,r,b))
        im1.save("%s/png/ascii%03d.png"%(f,i))
        i += 1
