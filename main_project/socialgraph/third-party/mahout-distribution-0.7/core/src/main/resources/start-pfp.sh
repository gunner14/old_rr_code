#!/bin/bash

baseDir=$(cd "$(dirname "$0")"; pwd)
hdfsBaseDir="association-rule-recommend"
pfpInput="$hdfsBaseDir/input"
pfpOutput="$hdfsBaseDir/output"
hadoopCmd="/opt/hadoop/bin/hadoop"
javaCmd="/home/xiang.zhang/opt/java/bin/java"

logMsg(){
    echo -e `date +"%Y/%m/%d %H:%M:%S "` "$1"
}


# update the active users
krbUser="xiang.zhang"
krbPwd="r13720301900R"
userIDFile="$baseDir/monthly-login.dat"
loadActiveUserIDs(){
	logMsg "[START] get the newest monthly login data..."
	
	if [ -f /etc/krb5.keytab ]; then
		export PATH="/usr/kerberos/bin:$PATH"
		export KRB5CCNAME=/tmp/krb5cc_pub_$$
		trap kdestroy 0 1 2 3 5 15
		echo $krbPwd | kinit $krbUser
	fi
	sg1Host="xce@sg1.xce.d.xiaonei.com"
	sg1Dir="/data/xce/SGProject/files/daily_login"
	scriptGene=". ~/.bash_profile; cd $sg1Dir && ls login_ids* | tail -30 | xargs cat | awk '!x[\$0]++' > monthly-login.dat"
	ssh $sg1Host "$scriptGene" && scp $sg1Host:$sg1Dir/monthly-login.dat $userIDFile
	
	success=$?
	if [ $success -eq 0 ]; then
		logMsg "[OK] get the newest monthly login data: $userIDFile"
	else
		logMsg "[ERROR] get the newest monthly login data failed."
	fi
	return $success
}


# update the friendlist data
pfpJobJar="$baseDir/mahout-core-0.7-job.jar"
friendListFile="$baseDir/friend-list.dat"
hdfsFile="$pfpInput/friend-list.dat"
libjarPath="$baseDir/pfpgrowth-libjars"
loadFriendList(){
	logMsg "[START] load the newest friend list data..."

	for jar in $libjarPath/*; do
		libjars="$libjars$sep$jar"
		sep=","
	done
	sep=
	for jar in $HADOOP_HOME/*.jar; do
		classpath="$classpath$sep$jar"
		sep=":"
	done
	sep=
	for jar in $HADOOP_HOME/lib/*.jar; do
		classpath="$classpath$sep$jar"
		sep=":"
	done
	sep=
	for jar in $libjarPath/*.jar; do
		classpath="$classpath$sep$jar"
		sep=":"
	done
	if [ -f $friendListFile ]; then
		rm $friendListFile
	fi
	totalRecords=`cat $userIDFile | wc -l`
	classpath="$classpath:$pfpJobJar"
	$javaCmd -cp $classpath org.apache.mahout.fpm.pfpgrowth.extra.ExportFriendList \
	--userIDFile $userIDFile \
	--friendListFile $friendListFile \
	--numThreads 20 \
	--totalRecords $totalRecords \
	--tableName relation
	
	success=$?
	if [ $success -eq 0 ]; then
		$hadoopCmd fs -put $friendListFile "$hdfsFile.tmp"
		$hadoopCmd fs -mv "$hdfsFile.tmp" $hdfsFile 
		logMsg "[OK] load friend list data to HDFS: $hdfsFile"
	else
		logMsg "[ERROR] load friend list data failed."
	fi
	return $success
}


# generate association rules from friend-list data
runPFPJob(){
    logMsg "[START] starting running PFP job..."

    $hadoopCmd jar $pfpJobJar org.apache.mahout.fpm.pfpgrowth.FPGrowthDriver \
    --input $pfpInput \
    --output $pfpOutput \
    --maxHeapSize 1000 \
    --numGroups 100000000 \
	--maxRulesPerUserGeneFromFP 1000 \
    --splitterPattern " " \
    --method mapreduce \
    --minSupport 20 \
	--minConfidence 80 \
    --minFPLen 2 \
    --startPhase 4 \
    --endPhase 4 \
	--maxRulesPerUser 150

    succeeded=$?
    if [ $succeeded -eq 0 ]; then
        logMsg "[OK] PFP job succeed."
    else
        logMsg "[ERROR] PFP job failed."
    fi
    return $succeeded
}

# upload association rules to cache
uploadToCache(){
	logMsg "[START] upload rule data to tripod cache..."

	$hadoopCmd jar $pfpJobJar org.apache.mahout.fpm.pfpgrowth.extra.UploadAssociationRules \
	--input "$pfpOutput/aggregatingRules"

	success=$?
	if [ $success -eq 0 ]; then
		logMsg "[ok] upload rule data to tripod cache succeed."
	else
		logMsg "[ERROR] upload rule data to tripod cache failed."
	fi
	return $success
}

# the overall flow of the offline update
main(){
	loadActiveUserIDs
	loadFriendList
    runPFPJob
	#uploadToCache
}

main

