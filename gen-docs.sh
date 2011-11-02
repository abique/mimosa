#! /bin/bash

push=0

for arg in $@
do
  if [[ "$arg" = push ]] ; then
    push=1
  fi
done

if test -d doc/html/.git; then
  cd doc/html &&
  git checkout $(git log --max-parents=0 | head -n 1 | cut -f 2 -d ' ') &&
  cd -
else
  push=0
fi &&

cp Doxyfile Doxyfile.tmp &&
sed -i "s/PROJECT_NUMBER.*/PROJECT_NUMBER=$(<VERSION)/g" Doxyfile.tmp
doxygen Doxyfile.tmp &&
rm Doxyfile.tmp &&
sed -i "s,</body>,<script type=\"text/javascript\">var gaJsHost = ((\"https:\" == document.location.protocol) ? \"https://ssl.\" : \"http://www.\");document.write(unescape(\"%3Cscript src='\" + gaJsHost + \"google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E\"));</script><script type=\"text/javascript\">try{var pageTracker = _gat._getTracker(\"UA-24670421-1\");pageTracker._trackPageview();} catch(err) {}</script></body>,g" doc/html/*.html &&

if [[ $push = 1 ]]; then
  cd doc/html &&
  git add * &&
  git commit -a -m "generated docs" &&
  git push -f origin HEAD:gh-pages
fi
