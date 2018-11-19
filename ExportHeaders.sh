#!/bin/bash

file=include-files
base=Isetta/IsettaEngine/
include=Includes/

find ${base} -name "*.h" | xargs grep -lE "(ISETTA_API|BEGIN_COMPONENT)" > ${file}
rsync -a --prune-empty-dirs --files-from=${file} --exclude="ISETTA_API.h" . ${include}
mv ${include}${base}* ${include}
rm -rf ${include}${base}
rm ${file}
