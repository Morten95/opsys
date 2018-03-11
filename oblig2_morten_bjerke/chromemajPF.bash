#!/bin/bash

for i in $( pgrep chrome );	#Går gjennom prosesser tilknyttet til chrome
do
	faults=$(ps --no-headers -o maj_flt "$i"); #Finner ID-enes page faults
	echo "Prosess ID:" "$i" " - " "Page faults:" "$faults";
	
	if [ "$faults" -gt 1000 ]	#Hvis ID-ens pagefault antall er > 1000
	then
		echo "(MER ENN 1000!)";
	fi
done
