#!/bin/bash

Sizing() 
{	
	COUNTER=0;
	VAR=$1;
	while [[ ${VAR%.*} -ge 1024 ]]; do
		((COUNTER++));
		VAR=$(echo "scale=2; $VAR/1024" | bc);

	done;
	echo "$VAR"; 
	return $COUNTER;
}

VAL=(0 0 0 0 0 0 0 0 0 0); #INICJALIZACJA TABLICY 
MAX=92;
RESCALE=0;
SCALE=$(($MAX/10));
REFRESH=0; #SPR CZY MAX NIE ZMIENIL SIE OD 11S 
SLP=1; #ODSTEP CZASOWY
TIME=1; #LICZNIK DO DZIELENIA AVERAGE
LINE=`grep wlp3s0 /proc/net/dev`; #NAZWA SIECI
FIRST_DWNL=`echo $LINE | awk '{print $2}'`; 
FIRST_UPL=`echo $LINE | awk '{print $10}'`;

while true; do #POBIERANIE DANCH Z /PROC/
	
	LINE=`grep wlp3s0 /proc/net/dev`;
	DWNL1=`echo $LINE | awk '{print $2}'`;
	UPL1=`echo $LINE | awk '{print $10}'`;
	
	sleep $SLP;
	
	LINE=`grep wlp3s0 /proc/net/dev`;
	DWNL2=`echo $LINE | awk '{print $2}'`;
	UPL2=`echo $LINE | awk '{print $10}'`;
	
	AVG_UPL=$(echo "scale=2; ($UPL2-$FIRST_UPL)/$TIME" | bc); 
	AVG_DWNL=$(echo "scale=2; ($DWNL2 - $FIRST_DWNL)/$TIME" | bc);
	DWNL=$(( $DWNL2 - $DWNL1 ));
	UPL=$(( $UPL2 - $UPL1 ));
	DATA=("B/s" "KB/s" "MB/s" "GB/s");
	#Zamiana na odopowiednio B, kB, MB, GB funkcją Sizing wartosci Uploadu, downloadu, AVG:

	FDWNL="$(Sizing "${DWNL}")";
	COUNT=$?;

	FUPL="$(Sizing "${UPL}")";
	COUNTUP=$?;		

	FAVG_DWNL="$(Sizing "${AVG_DWNL}")";	
	AVG_DWNL_COUNT=$?;
	
	FAVG_UPL="$(Sizing "${AVG_UPL}")";
	AVG_UPL_COUNT=$?;
	
	#RUNTIME
	LINE=`cat /proc/uptime`; #Getting name of my connection
	UPTIME=`echo $LINE | awk '{print $1}'`;
	DAYS=$(( ${UPTIME%.*} / 86400 ));
	HOURS=$((( ${UPTIME%.*} / 3600 ) - ( $DAYS * 24 )));
	MINUTES=$((( ${UPTIME%.*} / 60 ) - ( $HOURS * 60 ) - ( $DAYS * 60 * 24 ) ));
	SECONDS=$(( ${UPTIME%.*} - ( $MINUTES * 60 ) - ( $HOURS * 60 * 60 ) - ( $DAYS * 60 * 60 * 24 ) ));

	#BATERIA
	LINE=` grep POWER_SUPPLY_CAPACITY= /sys/class/power_supply/BAT0/uevent`;
	BATTERY=${LINE##*=};
	
	#CPU UTLIZATION
	UTICPU=` cat /proc/loadavg | awk '{print $1}'`;
	RATIO=92;
	#>>>>>>MATEMATYKA WYKRESU<<<<<<<
	#SPRAWDZENIE CZY JEST NOWY MAX
	if [ $DWNL -gt $MAX ]; then
		MAX=$DWNL;
		REFRESH=0;
		OLDSCALE=$SCALE		
		SCALE=$(( $MAX / 10 ));
		RESCALE=1;
	fi
	#CIALO FUNKCJI SPRAWDZAJACE PRZEDAWNIENIE MAXA //WYMAGANY REFRESH++ NA KONIEC
	if [[ $REFRESH -ge 10 ]]; then
		MX_HT=${VAL[0]};	
		for (( i=0; $i < 10; i++ )); do
			if [ $MX_HT -lt ${VAL[$i]} ]; then
				MX_HT=$((VAL[$i]));
			fi
		done
		MAX=$(( $MX_HT * $SCALE ));		
		OLDSCALE=$SCALE	# wys=tf/scale tf=wys*scale nwys = (wys*scale)/nscale nscale=nmax/10
		SCALE=$(( $MAX / 10 ));
		RESCALE=1;
		REFRESH=0;

	fi
	#PRZERYSOWANIE GO JEŚLI ZMIENILISMY SCALE (PRREZ REFRESH ALBO NOWEGO MAXA)
	if [[ $RESCALE -ge 1 ]]; then
		for (( i=0; $i <9; i++ )); do
			NEWVAL=$((( ${VAL[$i]}*$OLDSCALE) / $SCALE  ));
			VAL[$i]=$NEWVAL;
		done
		RESCALE=0;
	fi
	#PRZESUNIECIE W LEWO WYKRESU
	for (( i=9; $i > 0; i-- )); do

		AL=$i;
		AL=$(($AL-1));
		VAL[$i]=$((VAL[$AL]));
	done
	
	#KROK WYKRESU
	if [[ $DWNL -le 10 ]]; then
		VAL[0]=0;				
	else
		VAL[0]=$((($DWNL/$SCALE)+1));
	fi
	# STEROWANIE POLOZENIEM WYKRESU: !(X>=41 i Y>=19)! 	
	X=41;
	Y=19;	
	tput sc;
	printf "Speed:\n Download:%8s%4s⇓\tAverage:%8s%4s⇓\n" $FDWNL ${DATA[$COUNT]} $FAVG_DWNL ${DATA[$AVG_DWNL_COUNT]};		
	printf " Upload:  %8s%4s⇑\tAverage:%8s%4s⇑\n" $FUPL ${DATA[$COUNTUP]} $FAVG_UPL ${DATA[$AVG_UPL_COUNT]};		
	printf " PC runtime: %2s days,%2sh,%2smin,%2ss.\n" $DAYS $HOURS $MINUTES $SECONDS;
	printf " Battery level: %2s%% CPU Usage: %3s.\n" $BATTERY $UTICPU;
	
	#CZYSZCZENIE WYKRESU	
	tput cup $(($Y-12)) $(($X-41));
	tput ed;

	#RAMKA 
	printf 	"\e[94m╔══════════DOWNLOAD SPEED CHART════════════╗"
	for (( i=0; $i < 13; i++ )); do
		tput cup $((($Y+1)-$i)) $(($X-41));			
		printf "\e[94m║";
		tput cup $((($Y+1)-$i)) $((X+2));
		printf "\e[94m║";
	done
	tput cup $(($Y+1)) $(($X-41));
	printf 	"\e[94m╚═10s═9s══8s══7s══6s══5s══4s══3s══2s══1s═══╝"
	
	BLOCK="███";
	
	#SKURCZENIE WSKAZNIKOW OSI Y Z BYTES=>KB MB
	NMAX="$(Sizing "${MAX}")";
	NMAX_COUNT=$?;
	
	#UMIEJSCOWIENIE TRZECH WYZNACZNIKOW OSI Y
	tput cup $(($Y-11)) $(($X+3));
	printf " <-%3s%2s" $NMAX ${DATA[$NMAX_COUNT]};
	tput cup $(($Y-6)) $(($X+3));
	printf " <-%5s%2s" $(echo "scale=2; $NMAX/2" | bc) ${DATA[$NMAX_COUNT]};
	tput cup $(($Y-1)) $(($X+4));
	printf "<-less than %3s%2s" $(echo "scale=2; $NMAX/10" | bc) ${DATA[$NMAX_COUNT]};
	
	#WYPISANIE WYKRESU
	for (( i=0; $i < 10; i++ )); do
	
		for (( j=0 ; $j < ${VAL[$i]} ; j++  )); do		
			tput cup $(($Y-j)) $(($X-3));
			printf "\e[1;34m%s" $BLOCK;
			if [ $j -gt 10 ]; then #ZABEZPIECZENIE PRZED WYSYPANIEM PRZEZ KONFLIKT Z NIEPOPRAWNYM SCALE
				break;
			fi
		done
		X=$(($X-4));
		tput cup $Y $X;	
	done

	tput rc;

	TIME=$(( TIME+=1 ));
	REFRESH=$(( REFRESH+=1 ));	
done;
