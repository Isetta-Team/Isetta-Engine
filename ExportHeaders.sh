#!/bin/bash

file=include-files
base=Isetta/IsettaEngine/
include=Includes/

rm -rf ${include}
find ${base} -name "*.h" | xargs grep -lE "(ISETTA_API|BEGIN_COMPONENT|Util)" > ${file}
rsync -a --prune-empty-dirs --files-from=${file} . ${include}
mv ${include}${base}* ${include}
rm -rf ${include}Isetta
rm ${file}
