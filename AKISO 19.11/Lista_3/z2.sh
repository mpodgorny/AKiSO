#!/bin/bash

PROCS=($( find /proc -maxdepth 1 -type d  | grep [0-9] | awk '{print $1}' ));
printf "\tName: \t\t pid: \tPpid: \t OpFiles:\n";
for d in ${PROCS[@]}; 
do
	if [[ -d "$d" ]]; then
		printf "%15s \t %s \t %s \t %s\n" $(grep -E '^Name|^Pid|^PPid' $d/status | awk '{print $2}') $(sudo ls $d/fd | wc -l); 
	else
		printf "procces no longer exists, pid: %s\n" $d;
	fi
done
