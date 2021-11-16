#!/bin/sh


#THIS SCRIPT MUST BE ACCOMPANIED BY CARDAMOM_ECDF_EXECUTABLES_LIST.txt
#arguments are start and end lines!
if [[ -z "${PBS_ARRAYID}" ]]; then
  TARGET_LINE_NUM=1
else
  TARGET_LINE_NUM="${PBS_ARRAYID}"
fi

task=$(sed "${PBS_ARRAYID}q;d" ${CARDAMOM_ECDF_EXECUTABLES_LIST})
echo "running " ${task}
command ${task}
