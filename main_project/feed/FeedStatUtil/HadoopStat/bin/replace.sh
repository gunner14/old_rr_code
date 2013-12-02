#!/bin/bash
source ~/.bashrc
source ./vars.sh
if [ $# -ne 2 ] ; then
	echo "USAGE:$0 rootuid round "
	exit 1;
fi

rootuid=$1
round=$2
TASKNAME="distance/"$rootuid

SCRIPTDIR="../py/"

#LOCAL_OUTPUT="/home/jingwei.ye/data/${TASKNAME}"
#REMOTE_OUTPUT_DIR="/user/jingwei.ye/stat/task/${TASKNAME}/output/round${nextround}/"
RELATION_INPUT="/user/jingwei.ye/stat/task/${TASKNAME}/output/round${round}/part*"
FAKEID2UID="/user/jingwei.ye/stat/task/distance/input/uid2fakeid"
REMOTE_OUTPUT_DIR="/user/jingwei.ye/stat/task/${TASKNAME}/output/replace/"

#school info.  RELATION_INPUT="/user/yeyin.zhang/Cluster/usermatrix/univerFlagNum/part-r-00000"

MAPPER="../py/distance/replace.mapper.py"
REDUCER="../py/distance/replace.reducer.py"
AllInput=" -input $RELATION_INPUT -input $FAKEID2UID"

echo "rootuid:" $rootuid
echo "RELATION_INPUT: " $RELATION_INPUT
echo "REMOTE_OUTPUT_DIR: " $REMOTE_OUTPUT_DIR
echo "MAPPER: " $MAPPER
echo "REDUCER: " $REDUCER

#exit

#TMPDIR=${LOCAL_OUTPUT}/${TASKNAME}/round${nextround}.tmp
#FINAL=${LOCAL_OUTPUT}/${TASKNAME}/round${nextround}
#echo "TMPDIR:" $TMPDIR
#echo "FINAL:" $FINAL
#rm -r $TMPDIR
#mkdir -p $TMPDIR

${HADOOP} dfs -rmr ${REMOTE_OUTPUT_DIR}
${HADOOP} jar ${STREAM}  \
	-Dmapred.map.tasks=30 \
	-Dmapred.reduce.tasks=30 \
	-Dmapreduce.job.output.key.comparator.class=org.apache.hadoop.mapred.lib.KeyFieldBasedComparator \
	-Dmapreduce.partition.keycomparator.options=-n \
	${AllInput} \
	-output ${REMOTE_OUTPUT_DIR} \
	-mapper "$MAPPER"  \
	-reducer $REDUCER  -file $MAPPER -file $REDUCER
	#-mapper "${MAPPER}"  \

echo 'hadoop job OK!'

FINAL=/home/jingwei.ye/HadoopStat/data/landmark/$rootuid
TMPDIR=/home/jingwei.ye/HadoopStat/data/landmark/${rootuid}_hadoop
rm -r $TMPDIR
mkdir -p $TMPDIR

${HADOOP} dfs -get ${REMOTE_OUTPUT_DIR}/part-00*   $TMPDIR &&
#cat  ${TMPDIR}/part* > $FINAL &&
#cat  ${TMPDIR}/part* | sort -n |awk 'BEGIN{ORS="\t"}{if($2 > 1000){print 9} else {print $2}}'> $FINAL &&
echo "FINAL:" $FINAL

echo "===================END============$runtimestamp============="
