/**************************************************************************************************
 * Function:  Unit test case of ExtendFriendstoFriendsOfFriends class 
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Date:  2012-12-10
***************************************************************************************************/

package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map.Entry;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ExtendFriendsToFriendsOfFriendsTest {
	public static final long kReorcdNum = 1000000; //unit indexing number of records processed
	public static final int kReducePartitionNumber = 500;
	private static HTable friendRankHTable = null;
	private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
			.getLog(ExtendFriendsToFriendsOfFriendsTest.class);	
	
	public static class ExMapper extends TableMapper<IntWritable, FriendFeatureStructure> {
		private final static byte[] kFriendRankHTableColumnFamily = Bytes.toBytes("f");
		private long MapperCount = 0;
		@SuppressWarnings("rawtypes")
		@Override
		protected void setup(org.apache.hadoop.mapreduce.Mapper.Context context)
				throws IOException, InterruptedException {
			friendRankHTable = new HTable(context.getConfiguration(), context.getConfiguration().get("Input_FriendRankHTable"));
		}
		
		public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
			
			if (values.raw().length > 8000)
				return;			
			
			byte[] hostIdByte = new byte[Bytes.SIZEOF_INT];
			int hostId = Integer.parseInt(Bytes.toString(row.get()));
			System.arraycopy(Bytes.toBytes(hostId), 0, hostIdByte, 0, Bytes.SIZEOF_INT);
			int friendNumber = values.size();
			/* Getting users' friends' raw data(including friendId, both static and active ranking values) 
			 * from friend rank htables */
			HashMap<Integer, FriendRankItem> friendRankItemMap = new HashMap<Integer, FriendRankItem>();	
            try {
            	Get get = new Get(Bytes.toBytes(String.valueOf(hostId)));
    			get.addFamily(kFriendRankHTableColumnFamily);
    			Result rankValues = friendRankHTable.get(get);	
    			final int kFriendRankUnitSize = Bytes.SIZEOF_INT + 2 * Bytes.SIZEOF_FLOAT; 
    			if ( rankValues.size() > 0 ) {
    				//cell unit size storing one friend's id, static weight and active weight values of the user 
    				KeyValue[] friendRankListKeyValues = rankValues.raw();
    				int friendRankListSize = friendRankListKeyValues[0].getValueLength() / kFriendRankUnitSize;
    				logger.info("hostId " + hostId + " friendRankListSize " +  friendRankListSize);
    				byte[] rankValueBytes = friendRankListKeyValues[0].getValue();
    				for (int i = 0; i < friendRankListSize; ++i) {
    					byte[] friendId = new byte[Bytes.SIZEOF_INT];
    			    	FriendRankItem friendRankItem = new FriendRankItem();
    			    	System.arraycopy(rankValueBytes, i * kFriendRankUnitSize, friendId, 0, Bytes.SIZEOF_INT);
    			    	friendId = Tools.ReversEndian(friendId, Bytes.SIZEOF_INT, false);
    			    	friendRankItem.setFriendId(friendId);
    			    	System.arraycopy(rankValueBytes, i * kFriendRankUnitSize + Bytes.SIZEOF_INT, friendRankItem.getStaticWeight(), 0, Bytes.SIZEOF_FLOAT);
    			    	friendRankItem.setStaticWeight(Tools.ReversEndian(friendRankItem.getStaticWeight(), Bytes.SIZEOF_FLOAT, false));
    			    	System.arraycopy(rankValueBytes, i * kFriendRankUnitSize + Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, friendRankItem.getActiveWeight(), 0, Bytes.SIZEOF_FLOAT);
    			    	friendRankItem.setActiveWeight(Tools.ReversEndian(friendRankItem.getActiveWeight(), Bytes.SIZEOF_FLOAT, false));
    			    	logger.info("friendId " + Bytes.toInt(friendRankItem.getFriendId()) + 
    			    			" staticWeight " + Bytes.toFloat(friendRankItem.getStaticWeight()) + " activeWeight " + Bytes.toFloat(friendRankItem.getActiveWeight()));
    			    	friendRankItemMap.put(Bytes.toInt(friendId), friendRankItem);    			    	
    				}		
    			}  
			} catch (Exception e) {
				// TODO: handle exception
    		}	

          //logger.info("hostId " + hostId + " friendRankItemMap size " + friendRankItemMap.size());
          for (Entry<Integer, FriendRankItem> item : friendRankItemMap.entrySet()) {
          	logger.info("friendId " + item.getKey());
          	FriendRankItem friendRankItem = new FriendRankItem();
          	friendRankItem = item.getValue();
          	logger.info(" out friendId " + Bytes.toInt(friendRankItem.getFriendId()) + 
          			" staticWeight " + Bytes.toFloat(friendRankItem.getStaticWeight()) + 
          			" activeWeight " + Bytes.toFloat(friendRankItem.getActiveWeight()));
          }
            
			/*
			 * Adding users' friends' raw data(including friendId, adding friends time among them) 
			 * from friend relation and extend them to users' friends' of friends 
			*/			
			final int kFriendFeatureUnitSize = 2 * (Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT); 
			byte[] bytes = new byte[friendNumber * kFriendFeatureUnitSize];
			int pos = 0;
			for (KeyValue kv : values.raw()) {
				byte[] friendId = new byte[Bytes.SIZEOF_INT];
				friendId = kv.getQualifier();
				System.arraycopy(friendId, 0, bytes, pos * kFriendFeatureUnitSize, Bytes.SIZEOF_INT);
				byte[] time = new byte[Bytes.SIZEOF_INT];  
				time = kv.getValue();
				System.arraycopy(time, 0, bytes, pos * kFriendFeatureUnitSize + Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				FriendRankItem friendRankItem = new FriendRankItem();
				int key = Bytes.toInt(friendId);
				if (friendRankItemMap.containsKey(key)) {
					friendRankItem = friendRankItemMap.get(key);
				} else {
					friendRankItem.setStaticWeight(Bytes.toBytes(0));
					friendRankItem.setActiveWeight(Bytes.toBytes(0));
					logger.info("else friendId " + Bytes.toInt(friendId) + 
			    			" staticWeight " + Bytes.toFloat(friendRankItem.getStaticWeight()) + " activeWeight " + Bytes.toFloat(friendRankItem.getActiveWeight()));					
			    }
				System.arraycopy(friendRankItem.getStaticWeight(), 0, bytes, pos * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, Bytes.SIZEOF_FLOAT);
				System.arraycopy(friendRankItem.getActiveWeight(), 0, bytes, pos * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT,
						Bytes.SIZEOF_FLOAT);				
				++pos;
			}				
			
			for (int i = 0; i < friendNumber; ++i) {
				byte[] friendId = new byte[Bytes.SIZEOF_INT];
				byte[] time = new byte[Bytes.SIZEOF_INT];  
				byte[] staticWeight = new byte[Bytes.SIZEOF_FLOAT];
				byte[] activeWeight = new byte[Bytes.SIZEOF_FLOAT];
				System.arraycopy(bytes, i * kFriendFeatureUnitSize, friendId, 0, Bytes.SIZEOF_INT);
				System.arraycopy(bytes, i * kFriendFeatureUnitSize + Bytes.SIZEOF_INT, time, 0, Bytes.SIZEOF_INT);
				System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, staticWeight, 0, Bytes.SIZEOF_FLOAT);
				System.arraycopy(bytes, i * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, activeWeight, 0, 
						Bytes.SIZEOF_FLOAT);	
				logger.info("friendId " + Bytes.toInt(friendId) + " hostId " + Bytes.toInt(hostIdByte) + " time " +
						Bytes.toInt(time) + " staticWeight " + Bytes.toFloat(staticWeight) + " activeWeight " + Bytes.toFloat(activeWeight));
				context.write(new IntWritable(Bytes.toInt(friendId)), 
					          new FriendFeatureStructure(hostIdByte, time, staticWeight, activeWeight, friendNumber, bytes));
				
			}			
			context.getCounter("FRTMapper", "USER_COUNT").increment(1);
			if ((++MapperCount % kReorcdNum) == 0) {
				context.setStatus("Finished processing " + MapperCount	+ " mapper records");
			}
		}		
	}

	public static class ExReducer extends Reducer<IntWritable, FriendFeatureStructure, IntWritable, FriendFeatureStructureList> {
		private long ReducerCount = 0;

		@Override	
		protected void reduce(IntWritable key, Iterable<FriendFeatureStructure> values, Context context)
				              throws IOException, InterruptedException {
		  
			List<FriendFeatureStructure> friendFeatureStructureList = new ArrayList<FriendFeatureStructure>();
			for (FriendFeatureStructure item : values) {
				friendFeatureStructureList.add(new FriendFeatureStructure(item));
			}
			
 		    logger.info("[getFriendRankValue]: hostId " + key.get() + " friendNumber " + friendFeatureStructureList.size());
			for (FriendFeatureStructure item : friendFeatureStructureList) {
				int friendsNumOfFriend = item.getFriendFriendNumber();
				logger.info("Friend Id " + Bytes.toInt(item.getFriendId()) + 
						" time " + Bytes.toInt(item.getTime()) + 
						" friend number " + friendsNumOfFriend + 
						" static weight " + Bytes.toFloat(item.getStaticWeight()) + 
						" active weight " + Bytes.toFloat(item.getActiveWeight()));
				byte[] mutualFriendsIdBytes = item.getBytes(); 
				for (int count = 0; count < friendsNumOfFriend; ++count) {
					int fofId = Bytes.toInt(mutualFriendsIdBytes, count * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
					logger.info("fofId " + fofId);
				}
			}        
			
  		    context.write(key, new FriendFeatureStructureList(friendFeatureStructureList, friendFeatureStructureList.size()));

			context.getCounter("FRTReducer", "USER_COUNT").increment(1);
			if ((++ReducerCount % kReorcdNum) == 0)
				context.setStatus("Finished processing " + ReducerCount + " reducer records");
		}
	}
	
	public final String friendRelationHTable;
	public final String friendRankHTableStringName;
	public final String extendedFoFsOutput;
	public final Configuration conf;

	@SuppressWarnings("deprecation")
	public int doIt() throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
		System.out.println("[ExtendFriendsToFriendsOfFriends] doIt");
		
		Path p = new Path(this.extendedFoFsOutput);
		FileSystem fs = FileSystem.get(new URI(this.extendedFoFsOutput), conf);
		if (fs.exists(p) && fs.isDirectory(p))
			fs.delete(p);
		
		Job job = new Job(this.conf);
		
		job.getConfiguration().set("Input_FriendRelationHTable", this.friendRelationHTable);
		job.getConfiguration().set("Input_FriendRankHTable", this.friendRankHTableStringName);
		job.getConfiguration().set("output", this.extendedFoFsOutput);
		
    	job.setJarByClass(ExtendFriendsToFriendsOfFriendsTest.class);
		job.setMapperClass(ExMapper.class);
	    job.setReducerClass(ExReducer.class);
	    job.setNumReduceTasks(kReducePartitionNumber);
	    
	    Scan scan = new Scan();
	    TableMapReduceUtil.initTableMapperJob(this.friendRelationHTable, scan, ExMapper.class, IntWritable.class, FriendFeatureStructure.class, job);
	    TableMapReduceUtil.setScannerCaching(job, 20000);		
	    job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(FriendFeatureStructureList.class);
		FileOutputFormat.setOutputPath(job, new Path(this.extendedFoFsOutput));
		job.setOutputFormatClass(SequenceFileOutputFormat.class);	
		
		int result = job.waitForCompletion(true) ? 0 : 1;
		return result;
	}

	private ExtendFriendsToFriendsOfFriendsTest(String friendRelationHTable, String friendRankHTableStringName, String extendedFoFsOutput, 
			Configuration conf) {
		this.friendRelationHTable = friendRelationHTable;
		this.friendRankHTableStringName = friendRankHTableStringName;
		this.extendedFoFsOutput = extendedFoFsOutput;
		this.conf = conf;
	}

	@SuppressWarnings("static-access")
	private static ExtendFriendsToFriendsOfFriendsTest parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input friend relation HTable").withLongOpt("friendRelation").create("a"));
		
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input friend rank HTable").withLongOpt("friendRank").create("b"));		
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Extended FoFs output diretory").withLongOpt("extendedFoFsOutput")
				.create('o'));
	    opts.addOption(OptionBuilder.hasArg(true).isRequired(false)
	            .withDescription("Config file").withLongOpt("conf")
	            .create('f'));

		Configuration conf = new Configuration();
		conf = HBaseConfiguration.addHbaseResources(conf);

		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();

		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String friendRelationTable = cmd.getOptionValue('a');
		String friendRankHTableStringName = cmd.getOptionValue('b');
		String extendedFoFsOutput = cmd.getOptionValue('o');
        if (cmd.hasOption('f')) {
            String confname = cmd.getOptionValue('f');
            Path p = new Path(confname);
            System.out.println("Config file is: " + p.toString());
            conf.addResource(p);
        }
		return new ExtendFriendsToFriendsOfFriendsTest(friendRelationTable, friendRankHTableStringName, extendedFoFsOutput, conf);
	}

	public static void main(String[] args) throws Exception {
		ExtendFriendsToFriendsOfFriendsTest efr = parseArgs(args);
		efr.doIt();
	}
}







//logger.info("get friend id length 3 " + friendRankItem.getFriendId().length);
//if (null == friendRankItem.getFriendId()) {
//	friendRankItem.setFriendId(friendId);
//}
//System.arraycopy(friendId, 0, friendRankItem.getFriendId(), 0, Bytes.SIZEOF_INT);
//if (null == friendRankItem.getStaticWeight()) {
//	friendRankItem.setStaticWeight(Bytes.toBytes(0));
//}
//if (null == friendRankItem.getActiveWeight()) {
//	friendRankItem.setActiveWeight(Bytes.toBytes(0));
//}

//System.arraycopy(friendRankItemMap., 0, friendRankItem.getStaticWeight(), 0, Bytes.SIZEOF_FLOAT);
//System.arraycopy(friendId, 0, friendRankItem.getActiveWeight(), 0, Bytes.SIZEOF_FLOAT);
//logger.info("friendId " + Bytes.toInt(friendId));
//logger.info("friendRankItemMap.get(friendId)" + friendRankItemMap.get(friendId).getFriendId());
//
//
//
//logger.info("map friendId " + Bytes.toInt(friendRankItemMap.get(friendId).getFriendId()) + 
//		" staticWeight " + Bytes.toFloat(friendRankItemMap.get(friendId).getStaticWeight()) + 
//		" activeWeight " + Bytes.toFloat(friendRankItemMap.get(friendId).getActiveWeight()));			
//
//logger.info("in friendId " + Bytes.toInt(friendRankItem.getFriendId()) + " staticWeight " + 
//		Bytes.toFloat(friendRankItem.getStaticWeight()) + " activeWeight " + Bytes.toFloat(friendRankItem.getActiveWeight()));
//
//
//logger.info("in friendId " + Bytes.toInt(friendRankItem.getFriendId()) + " staticWeight " + 
//Bytes.toFloat(friendRankItem.getStaticWeight()) + " activeWeight " + Bytes.toFloat(friendRankItem.getActiveWeight()));

