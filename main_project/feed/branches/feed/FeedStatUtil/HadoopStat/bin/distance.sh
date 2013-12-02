#!/bin/bash
source ~/.bashrc
source ./vars.sh
if [ $# -ne 2 ] ; then
	echo "USAGE:$0 rootuid round "
	exit 1;
fi

rootuid=$1
round=$2
nextround=`expr $round + 1`
TASKNAME="distance/"$rootuid

SCRIPTDIR="../py/"

LOCAL_OUTPUT="/home/jingwei.ye/data/${TASKNAME}"
REMOTE_OUTPUT_DIR="/user/jingwei.ye/stat/task/${TASKNAME}/output/round${nextround}/"
#RELATION_INPUT="/user/jingwei.ye/stat/task/${TASKNAME}/input/school"
RELATION_INPUT="/user/yeyin.zhang/Cluster/usermatrix/usermatrix_data/part-r-00000"
if [ $round -ne 0 ] ; then
	RELATION_INPUT="/user/jingwei.ye/stat/task/${TASKNAME}/output/round${round}/part*"
fi
#school info.  RELATION_INPUT="/user/yeyin.zhang/Cluster/usermatrix/univerFlagNum/part-r-00000"

MAPPER="../py/distance/distance.mapper.py"
REDUCER="../py/distance/distance.reducer.py"
AllInput=" -input $RELATION_INPUT "

echo "rootuid:" $rootuid
echo "RELATION_INPUT: " $RELATION_INPUT
echo "REMOTE_OUTPUT_DIR: " $REMOTE_OUTPUT_DIR
echo "MAPPER: " $MAPPER
echo "REDUCER: " $REDUCER

#exit

TMPDIR=${LOCAL_OUTPUT}/${TASKNAME}/round${nextround}.tmp
FINAL=${LOCAL_OUTPUT}/${TASKNAME}/round${nextround}
echo "TMPDIR:" $TMPDIR
echo "FINAL:" $FINAL
rm -r $TMPDIR
mkdir -p $TMPDIR

#exit
${HADOOP} dfs -rmr ${REMOTE_OUTPUT_DIR}
${HADOOP} jar ${STREAM}  \
	-Dmapred.map.tasks=30 \
	-Dmapred.reduce.tasks=20 \
	${AllInput} \
	-output ${REMOTE_OUTPUT_DIR} \
	-mapper "$MAPPER $rootuid $round"  \
	-reducer $REDUCER  -file $MAPPER -file $REDUCER
	#-mapper "${MAPPER}"  \

echo 'hadoop job OK!'


if [ $round -ne 0 ] ; then
	${HADOOP} dfs -rmr $RELATION_INPUT
fi

#${HADOOP} dfs -get ${REMOTE_OUTPUT_DIR}/part-00*   $TMPDIR &&
#echo "TMPDIR:" $TMPDIR
#cat  ${TMPDIR}/part* > $FINAL &&
#echo "FINAL:" $FINAL

echo "===================END============$runtimestamp============="
