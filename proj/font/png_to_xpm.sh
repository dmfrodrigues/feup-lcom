cd $1
mkdir -p xpm
for f in png/*.png; do
    a=`expr "$f" : 'png/\([a-zA-Z0-9]*\).png'`
    convert $f xpm/$a.xpm
done
sed -i "s/black/#000000/g" xpm/*.xpm