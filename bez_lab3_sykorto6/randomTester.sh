#!/bin/bash
for i in {1..100}
do
curl -sL "https://unsplash.it/200/300/?random" -o $i.jpg
convert $i.jpg $i.bmp;
rm -rf $i.jpg


./3.out -e $i.bmp $i-encrypted-cbc.bmp cbc;
./3.out -e $i.bmp $i-encrypted-ecb.bmp ecb;
./3.out -d $i-encrypted-cbc.bmp $i-decrypted-cbc.bmp cbc;
./3.out -d $i-encrypted-ecb.bmp $i-decrypted-ecb.bmp ecb;


diff $i.bmp $i-decrypted-cbc.bmp && echo "Test image $i for CBC succeded"
diff $i.bmp $i-decrypted-ecb.bmp && echo "Test image $i for ECB succeded"

done;