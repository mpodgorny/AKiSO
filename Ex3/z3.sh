 #!/bin/bash

LINK=$((curl --request GET \
  --url 'https://api.thecatapi.com/v1/images/search?format=json&limit=1' \
  --header 'Content-Type: application/json' \
  --header 'x-api-key: DEMO-API-KEY' ) | jq --raw-output '.[].url')

wget $LINK -O img.jpg
img2txt f utf8 -W 200 img.jpg

((curl -s http://api.icndb.com/jokes/random) |  jq  -S '.value.joke' | sed 's/&nbsp;/ /g; s/&amp;/\&/g; s/&lt;/\</g; s/&gt;/\>/g; s/&quot;/\"/g; s/#&#39;/\'"'"'/g; s/&ldquo;/\"/g; s/&rdquo;/\"/g;')
