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

sed -i "s/PROJECT_NUMBER.*/PROJECT_NUMBER=$(<VERSION)/g" Doxyfile
doxygen Doxyfile

if [[ $push = 1 ]]; then
  cd doc/html &&
  git add * &&
  git commit -a -m "generated docs" &&
  git push -f origin HEAD:gh-pages
fi
