#!/bin/sh
# used to compile .qrc resource files in the same folder to corresponding .rcc files

dir="."
find $dir -name "*.qrc" | while read file; do
    echo "processing: "${file##*/} echo " output as: " "$(basename "$file" .qrc).rcc"
    rcc -binary ${file##*/} -o "$(basename "$file" .qrc).rcc"
done
