#！/usr/bin/bash

echo "test use 1"
test -w tmp.txt
if [ $? -eq "0" ]; then
echo "success \n";
else
echo "net success";
fi
echo $?
echo "test use 2 [] begin"
[ -w tmp.txt ]
echo $?