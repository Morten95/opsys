#!/bin/bash

echo "Velg et nummer fra menyen"
echo "1 - Hvem er jeg og hva er navnet på dette scriptet?"
echo "2 - Hvor lenge er det siden sist boot?"
echo "3 - Hvor mange prosesser og tråder finnes?"
echo "4 - Hvor mange context switch'er fant sted siste sekund?"
echo "5 - Hvor stor andel CPU-tid ble benyttet i kernelmode og i usermode siste sekund?"
echo "6 - Hvor mange interrupts fant sted siste sekund?"
echo "9 - Avslutt dette scriptet"

while read input

do

case $input in
	1)
		echo "Jeg er" "$(whoami)";	#skriver bruker
		echo "dette scriptet heter" "$(basename "$0")";
		;;	#Skriver filnavn

	2)
		echo "Siste reboot var" "$(uptime -s)";
		;;	#Skriver siste reboot

	3)
		echo "Antall prosesser" "$(ps aux --no-heading | wc -l)";
		echo "Antall tråder" "$(ps aux --no-heading -T | wc -l)";
		;;	#Denne kun ut antall prosesser og tråder

	4)
		echo "Antall context switcher siste sekund";
		ctxtvar1=$(grep ctxt /proc/stat | awk '{print $2}');
		sleep 1;
		ctxtvar2=$(grep ctxt /proc/stat | awk '{print $2}');
		ctxtdiff=$((ctxtvar2 - ctxtvar1));
		echo $ctxtdiff;
		;;	#greper contextswitcher fra /proc/stat
			#awker rett kolonne, sleeper 1 sekund for og
			#kun få contextswitcher for 1 sekund

	5)
		usrvar1=$(grep cpu0 /proc/stat | awk '{print $2}');
		kervar1=$(grep cpu0 /proc/stat | awk '{print $4}');
		sleep 1;
		usrvar2=$(grep cpu0 /proc/stat | awk '{print $2}');
		kervar2=$(grep cpu0 /proc/stat | awk '{print $4}');
		usrdiff=$((usrvar2 - usrvar1));
		kerdiff=$((kervar2 - kervar1));
		echo "Andel CPU-tid i usermode" $usrdiff;
		echo "andel CPU-tid i kernelmode" $kerdiff;
		;;	#henter ut cpu bruk fra usermode og kernel mode
			#skriver ut bruk for kun 1 sekund

	6)
		intrvar1=$(grep intr /proc/stat | awk '{print $2}');
		sleep 1;
		intrvar2=$(grep intr /proc/stat | awk '{print $2}');
		intrdiff=$((intrvar2 - intrvar1));
		echo "Antall interrupts siste sekund" $intrdiff;
		;;	#henter interrupts og skriver ut for kun 1 sekund

	9) exit;;			#avslutter program
	*) echo "Ugyldig input!";;	#hvis ingen av casene blir lest
esac
done
