#!/bin/bash

for pid in "$@"		#For hvert argument gjør dette
do

if [ "$(grep Name /proc/"$pid"/status | awk '{print $2}')" != "chrome" ];
then
	echo "Prosess-ID $pid finnes ikke eller er ikke tilknyttet chrome!"
	#If løkken sjekker om prosessen finnes og om den tilhører chrome
else	
#Hvis prosess-id finnes og har Name = "chrome" skjer dette:

echo "******** Minne info om prosess med PID $pid ********" > "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Oppretter fil med Prosess ID, Dato og klokkeslett, skriver og første linje i filen


argsize=$(grep VmSize /proc/"$pid"/status | awk '{print $2}');
echo "Totalt bruk av virtuelt minne(VmSize): $argsize kB" >> "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Henter argumentets Vmsize og skriver det inn i rett format og legger det inn rett fil


argdata=$(grep VmData /proc/"$pid"/status | awk '{print $2}');
argstk=$(grep Vmstk /proc/"$pid"/status | awk '{print $2}');
argexe=$(grep VmExe /proc/"$pid"/status | awk '{print $2}');
argpvm=$((argdata + argstk + argexe));
echo "Mengde privat virtuelt minne(VmData+VmStk+VmExe): $argpvm kB" >> "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Henter Data, Stk, Exe. Plusser det sammen og skriver inn i rett fil


arglib=$(grep VmLib /proc/"$pid"/status | awk '{print $2}');
echo "Mengde shared virtuelt minne(VmLib): $arglib kB" >> "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Henter VmLib og skriver det inn i rett fil


argrss=$(grep VmRSS /proc/"$pid"/status | awk '{print $2}');
echo "Total bruk av fysisk minne(VmRSS): $argrss kB" >> "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Henter VmRSS og skriver det i rett fil

argpte=$(grep VmPTE /proc/"$pid"/status | awk '{print $2}');
echo "Mengde fysisk minne om benyttes til page table(VmPTE): $argpte kB" >> "$pid-$(date +"%d%m%y")-$(date | awk '{print $4}').meminfo";
#Henter VmPTE og skriver det i rett fil

fi
done	#Slutt på loop
