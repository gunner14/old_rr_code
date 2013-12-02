#!/bin/sh

database="fb"
username="work@RR"
password="Geeker4ZolZ"
nummapper=10
tablelist=$1
hbasetable=$2
lasttime=$3
cfamily="f"
queuename="sg_sqoop"
scriptprefix="rr_"

while read address startnum stopnum
do
#    echo $address $startnum $stopnum

    if [ "$address" = "" ]
    then
        break
    fi
   
    rm "$scriptprefix$startnum.sh"

    for i in $( seq $startnum $stopnum )
    do
       echo "./sqoop import -libjars sqooptools.jar -D sqoop.hbase.insert.put.transformer.class=RelationPutTransformer -D pool.name=\"$queuename\" -D mapred.map.tasks.speculative.execution=false --connect jdbc:mysql://$address/$database --username $username --password $password --table relation_$i --split-by guest --hbase-table $hbasetable --column-family $cfamily --hbase-row-key host --num-mappers $nummapper" --check-column time --incremental append --last-value \"$lasttime\" >> "$scriptprefix$startnum.sh"
    done

done < $tablelist
