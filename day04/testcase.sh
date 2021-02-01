#！/usr/bin/bash
#case select
echo -n "enter a number from 1 to 3:"
read num
case $num in
1)
echo "you select 1"
;;
2)
echo "you select 2"
;;
3)
echo "you select 3"
;;
y|Y)
echo “you select y”
;;
*)
echo "`basename $0`:this is not between 1 and 3">&2
exit;
;;
esac
