#!/bin/bash

for file in find * ; do
	if [[ "$file" != `echo "$file" | tr "[AĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ]" "[aąbcćdeęfghijklłmnńoóprsśtuwyzźż]"` && -f "$file" ]]; then
    	loName=`echo "${file}" | tr "[AĄBCĆDEĘFGHIJKLŁMNŃOÓPRSŚTUWYZŹŻ]" "[aąbcćdeęfghijklłmnńoóprsśtuwyzźż]"`;
	mv -i -- "$file" "$loName"; 
fi
done
