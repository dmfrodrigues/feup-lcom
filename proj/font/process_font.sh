echo Processing $1
echo "    Splitting font"
python3 split_font.py $1 $2 $3 $4 $5 $6
echo "    Converting from png to xpm"
./png_to_xpm.sh $1
echo "    Converting from xpm to xpm2"
./xpm_to_xpm2.sh $1
echo "    Cleaning up"
rm -rf $1/png
rm -rf $1/xpm
#rm -rf $1/xpm2