#!/bin/bash

file=include-files
base=Isetta/IsettaEngine/
include=Includes/

find ${base} -name "*.h" | xargs grep -lE "(ISETTA_API|BEGIN_COMPONENT)" > ${file}
rsync -a --prune-empty-dirs --files-from=${file} --include="Util.h" . ${include}
mv ${include}${base}* ${include}
rm -rf ${include}Isetta
rm ${file}
