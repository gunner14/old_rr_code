#!/bin/bash
source ~/.bashrc
source ./vars.sh
if [ $# -ne 1 ] ; then
	echo "USAGE:$0 remotefile"
	exit 1;
fi

remotefile=$1
taskname="countline"
REMOTE_OUTPUT_DIR="/user/jingwei.ye/stat/task/$taskname/output/"
MAPPER="../py/countline/countline.mapper.py"
REDUCER="../py/countline/countline.reducer.py"
AllInput=" -input $remotefile"

echo "RELATION_INPUT: " $RELATION_INPUT
echo "REMOTE_OUTPUT_DIR: " $REMOTE_OUTPUT_DIR
echo "MAPPER: " $MAPPER
echo "REDUCER: " $REDUCER

#exit
${HADOOP} dfs -rmr ${REMOTE_OUTPUT_DIR}
${HADOOP} jar ${STREAM}  \
	-Dmapred.map.tasks=30 \
	-Dmapred.reduce.tasks=1 \
	${AllInput} \
	-output ${REMOTE_OUTPUT_DIR} \
	-mapper "$MAPPER "  \
	-reducer $REDUCER  -file $MAPPER -file $REDUCER

echo 'hadoop job OK! result'

$HADOOP dfs -cat $REMOTE_OUTPUT_DIR/part*



#${HADOOP} dfs -get ${REMOTE_OUTPUT_DIR}/part-00*   $TMPDIR &&
#echo "TMPDIR:" $TMPDIR
#cat  ${TMPDIR}/part* > $FINAL &&
#echo "FINAL:" $FINAL

echo "===================END============$runtimestamp============="
