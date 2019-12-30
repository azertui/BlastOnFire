#!/bin/sh

valgrind --tool=memcheck --undef-value-errors=yes --leak-resolution=high --leak-check=full --log-file=valgrind-out.txt ./parser -q 
test=$(cat valgrind-out.txt | grep "All heap blocks were freed" | wc -l)
rm -f valgrind-out.txt
if [ $test = "1" ]
then
return 0
else
echo "############################"
echo "Fuites de mémoires détectées"
echo "############################"
return 1
fi