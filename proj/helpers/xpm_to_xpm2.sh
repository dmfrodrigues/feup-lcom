cd $1
echo "#include \"xpm_utils.h\"" > convert.c
for f in xpm/*.xpm; do
    echo "#include \"$f\"" >> convert.c 
done
echo "int main(){" >> convert.c
for f in xpm/*.xpm; do
    s=${f:4:-4}
    echo "    xpm_save_as_xpm2($s,\"xpm2/$s.xpm2\");" >> convert.c
done
echo "    return 0;" >> convert.c
echo "}" >> convert.c
gcc -I../../include ../../src/xpm_utils.c convert.c -o convert.app
rm -rf xpm2
mkdir xpm2
./convert.app
rm convert.c convert.app
