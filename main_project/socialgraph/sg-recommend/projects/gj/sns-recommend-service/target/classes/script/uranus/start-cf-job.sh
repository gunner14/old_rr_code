#!/bin/bash

ft=`date +%Y-%m-%d-%H-%M-%S`
base_dir="/home/xiang.zhang/renrengj-cf"
db_export_file="$base_dir/data/user-pref.dat"
db_export_file_prev="$base_dir/data/user-pref.dat.prev"
db_export_file_tmp="$base_dir/data/user-pref.dat.tmp"

hdfs_base_dir="/user/xiang.zhang/mahout-cf"
hdfs_user_pref_file="$hdfs_base_dir/user-pref.dat"
hdfs_cf_tmp_dir="$hdfs_base_dir/temp.$ft"
hdfs_cf_out_dir="$hdfs_base_dir/recommendations.$ft"

cf_out_dir="$base_dir/data/recommendations.$ft"
cf_out_success_file="$cf_out_dir/_SUCCESS"
rec_comb_file="$base_dir/data/cf-recommendation.dat"
rec_comb_file_tmp="$base_dir/data/cf-recommendation.dat.tmp"
rec_comb_file_prev="$base_dir/data/cf-recommendation.dat.prev"

cf_job_stdout_file="$base_dir/logs/cf-job-stdout.log.$ft"
cf_job_stderr_file="$base_dir/logs/cf-job-stderr.log.$ft"



cd $base_dir
#export newest user-pref data from DB
echo "[`date +'%Y-%m-%d %H:%M:%S'`] offline update start... "
echo "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] export user-pref data from database."
/opt/java/bin/java -cp dependency/*:renrengj-cf-offline-1.0-SNAPSHOT.jar com.renren.ad.cf.DBExporter $db_export_file_tmp
exit_code=$?
if [ "$exit_code" -eq "0" ]; then
	mv $db_export_file $db_export_file_prev
	mv $db_export_file_tmp $db_export_file
	echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] export user-pref data from database."
else
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [error] export user-pref data failed, program exit."
	exit 1
fi

#put data to HDFS
if [ -f "$db_export_file" ]; then
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] put user-pref data to HDFS."
	/opt/hadoop/bin/hadoop fs -rm $hdfs_user_pref_file
	/opt/hadoop/bin/hadoop fs -put $db_export_file $hdfs_user_pref_file
	exit_code=$?
	echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] put user-pref data to HDFS."
else
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [error] user-pref data file not exist, program exit."
	exit 1
fi

#run recommendation hadoop job
if [ "$exit_code" -eq "0" ]; then
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] run recommend hadoop job."
	/opt/hadoop/bin/hadoop jar mahout-core-0.6-job.jar org.apache.mahout.cf.taste.hadoop.item.RecommenderJob --input $hdfs_user_pref_file --output $hdfs_cf_out_dir --numRecommendations 100  --maxPrefsPerUser 500 --minPrefsPerUser 10 --maxSimilaritiesPerItem 1000  --maxPrefsPerUserInItemSimilarity 500  --similarityClassname SIMILARITY_TANIMOTO_COEFFICIENT --tempDir $hdfs_cf_tmp_dir >> $cf_job_stdout_file 2>> $cf_job_stderr_file
	exit_code=$?
	#/opt/hadoop/bin/hadoop fs -rmr $hdfs_cf_tmp_dir
	echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] run recommend hadoop job."
else
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [error] put user-pref data to HDFS failed, program exit."
	exit 1
fi

#combine the final results
if [ "$exit_code" -eq "0" ]; then
	/opt/hadoop/bin/hadoop fs -get $hdfs_cf_out_dir $cf_out_dir
	if [ -f "$cf_out_success_file" ]; then
		echo "[`date +'%Y-%m-%d %H:%M:%S'`] [begin] combine the final result."
		cat $cf_out_dir/part-r* > $rec_comb_file_tmp
		mv $rec_comb_file $rec_comb_file_prev
		mv $rec_comb_file_tmp $rec_comb_file
		echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] [end] combine the final result."
	else
		echo "[`date +'%Y-%m-%d %H:%M:%S'`] [error] recommendation hadoop job failed."
	fi
else
	echo "[`date +'%Y-%m-%d %H:%M:%S'`] [error] recommendation hadoop job failed."
fi

#clean out result before 3 days
ft_clean=`date +%Y-%m-%d -d"3 days ago"`
hdfs_cf_tmp_dir="$hdfs_base_dir/temp.$ft_clean*"
hdfs_cf_out_dir="$hdfs_base_dir/recommendations.$ft_clean*"
/opt/hadoop/bin/hadoop fs -rmr $hdfs_cf_tmp_dir $hdfs_cf_out_dir
cf_out_dir="$base_dir/data/recommendations.$ft_clean*"
rm -rf $cf_out_dir 

echo -e "[`date +'%Y-%m-%d %H:%M:%S'`] offline update successfully."
