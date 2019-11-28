import glob
import os

files = [s[4:12] for s in glob.glob("xpm/*.xpm")]
files.sort()

with open("convert.c", 'w') as f:
    f.write('#include "xpm_utils.h"\n')
    f.write("".join(['#include "%s.xpm"\n'%s for s in files]))
    f.write("int main(){\n")
    f.write("".join(['    xpm_save_as_xpm2(%s,"xpm2/%s.xpm2");\n'%(s,s) for s in files]))
    f.write('    char **r = xpm_load_xpm2("xpm2/ascii065.xpm2");\n')
    f.write("    return 0;\n}\n")

os.system("gcc -I./xpm -I../include convert.c ../src/xpm_utils.c -o convert.app")
os.system("./convert.app")

os.system("rm -f convert.c convert.app")
os.system("rm -f xpm/*.xpm")
