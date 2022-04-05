#!/bin/sh


#THIS SCRIPT MUST BE ACCOMPANIED BY CARDAMOM_ECDF_EXECUTABLES_LIST.txt
#arguments are start and end lines!
TARGET_LINE_NUM=1
[[ -z "${PBS_ARRAYID}" ]] || TARGET_LINE_NUM="${PBS_ARRAYID}"
[[ -z "${SGE_TASK_ID}" ]] || TARGET_LINE_NUM="${SGE_TASK_ID}"
[[ -z "${PBS_ARRAY_INDEX}" ]] || TARGET_LINE_NUM="${PBS_ARRAY_INDEX}"
[[ -z "${SLURM_ARRAY_TASK_ID}" ]] || TARGET_LINE_NUM="${SLURM_ARRAY_TASK_ID}"


task=$(sed "${TARGET_LINE_NUM}q;d" ${CARDAMOM_ECDF_EXECUTABLES_LIST})
echo "running " ${task}
command ${task}
