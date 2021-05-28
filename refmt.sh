#!/bin/sh
filenames=$(find . -iname '*.ml' | grep -v node_modules | grep -v _opam/ | grep -v _build)
len=${#filenames[@]}
echo "$len"
echo "$filenames"
for filename in $filenames; do refmt --parse ml --print re "$filename" > "${filename%.ml}.re"; done;


filenames=$(find . -iname '*.mli' | grep -v node_modules | grep -v _opam/ | grep -v _build)
len=${#filenames[@]}
echo "$len"
echo "$filenames"
for filename in $filenames; do refmt --parse ml --print re "$filename" > "${filename%.mli}.rei"; done;
# filenames=$(find "$(pwd)" -name "*.rei")
# len=${#filenames[@]}
# echo "$len"
# echo "$filenames"
# for filename in $filenames; do rm "$filename"; done;
# exit 0
filenames=$(find . -iname '*.ml'| grep -v node_modules | grep -v _opam/ | grep -v _build && find . -iname '*.mli'| grep -v node_modules | grep -v _opam | grep -v _build)
len=${#filenames[@]}
echo "$len"
echo "$filenames"
for filename in $filenames; do rm "$filename"; done;

