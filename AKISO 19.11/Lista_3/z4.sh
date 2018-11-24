#!/bin/bash


mkdir -p Repo
cd Repo

lynx -dump -force_html $1 > site.html

git config --global user.name "Michael"
git config --global user.email dtenduro@gmail.com

git init
git add site.html
git commit -m "First commit"

while(true); do
	sleep $2
	lynx -dump -force_html $1> temporary.html
	diff -q site.html temporary.html	
	CHECK=$?
	if [[ $CHECK -eq 0 ]]; then
		printf "\e[1;;32mNo Changes on site %s\n" $1;			
	else
		printf "\e[1;;31mChange on the site! Saving new site in Git rep.\n"
		mv temporary.html site.html
		git add site.html	
		git commit -m "Change"			
	fi


done

