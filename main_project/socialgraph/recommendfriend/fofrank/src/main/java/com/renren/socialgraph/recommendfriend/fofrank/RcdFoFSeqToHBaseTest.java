/**************************************************************************************************
 * Function:  Unit test case of RcdFoFSeqToHBase class
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Date:  2012-12-10
 *********************************************************************************************************************/

package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.regionserver.StoreFile.BloomType;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class RcdFoFSeqToHBaseTest {	
	public static final long kRecordUnit = 1000; // unit of count for number of records
	private final static int kFriendFeatureUnitSize = 2 * (Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT);
	private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
			.getLog(RcdFoFSeqToHBaseTest.class);	
	
	public static class ReadExtendRelationMapper extends Mapper<IntWritable, FriendFeatureStructureList, NullWritable, NullWritable> {
		private static long MapperCount = 0;
		public static HTable desTable;
		public static CreateFoFTable  putter = null;
		
		protected void setup(
				Mapper<IntWritable, FriendFeatureStructureList, NullWritable, NullWritable>.Context context) 
				throws IOException, InterruptedException {
			desTable = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
			desTable.setWriteBufferSize(10 * 1024 * 1024);
			desTable.setAutoFlush(false);
			
			putter = new CreateFoFTableOneToAllGroup(Bytes.toBytes("f"), 1000, 100, desTable);
		}
		
		protected void cleanup (
				Mapper<IntWritable, FriendFeatureStructureList, NullWritable, NullWritable>.Context context) 
				throws IOException, InterruptedException{
			System.out.println(new Date() + " cleaning up");
			desTable.flushCommits();
		}	
		
		public Map<Integer, Integer> getFriendRankValue(List<FriendFeatureStructure> friendFeatureStructureList) {
			Map<Integer, Integer> friendRankMap = new HashMap<Integer, Integer>();
			if (friendFeatureStructureList.isEmpty()) {
				return friendRankMap;
			}
			float minScore = 10000;
			float maxScore = 0;
			float tmpScore = 0;
			int weight = 10;
			float staticFactor = (float) 0.3;
			float activeFactor = (float) 0.7;
			float staticWeight = 0;
			float activeWeight = 0;
			for (FriendFeatureStructure item : friendFeatureStructureList) {
				staticWeight = Bytes.toFloat(item.getStaticWeight());
				activeWeight = Bytes.toFloat(item.getActiveWeight());
		 		tmpScore = staticFactor * staticWeight + activeFactor * activeWeight;                
				if (maxScore < tmpScore) {
					maxScore = tmpScore;
				} 
				if (minScore > tmpScore) {
					minScore = tmpScore;
				}
			}
			for (FriendFeatureStructure item : friendFeatureStructureList) {
				staticWeight = Bytes.toFloat(item.getStaticWeight());
				activeWeight = Bytes.toFloat(item.getActiveWeight());
				tmpScore = staticFactor * staticWeight + activeFactor * activeWeight; 
				weight = 10 + (int)(90 * (0.001 + tmpScore - minScore) / (0.001 + maxScore - minScore));
				logger.info("[getFriendRankValue]: friend Id " + Bytes.toInt(item.getFriendId()) + 
						" hostFrdNum " + friendFeatureStructureList.size()
						+ " static weight " + staticWeight + " active weight " + activeWeight + 
						" tmpScore " + tmpScore + " weight " + weight);
				if (staticWeight == 0  && activeWeight == 0)
					weight = 10;
				friendRankMap.put(Bytes.toInt(item.getFriendId()), weight);
			}
     	    return friendRankMap;
		}
		
		public Map<Integer, Integer> getFoFRankValue(FriendFeatureStructure item) {
			Map<Integer, Integer> friendsOfFriendsRankMap = new HashMap<Integer, Integer>();			
			float minScore = 10000;
			float maxScore = 0;
			float tmpScore = 0;
			int weight = 10;
			float staticFactor = (float) 0.3;
			float activeFactor = (float) 0.7;
			float staticWeight = 0;
			float activeWeight = 0;
			int fiFrdNum = item.getFriendFriendNumber();
			logger.info("friend Id" + Bytes.toInt(item.getFriendId()) + "fiFrdNum " + fiFrdNum);
			byte[] bytes = item.getBytes();
			logger.info("bytes length " + bytes.length);
			for (int count = 0; count < fiFrdNum; ++count) {
				byte[] staticWeightByte = new byte[Bytes.SIZEOF_FLOAT];
				System.arraycopy(bytes,  count * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, staticWeightByte, 0, Bytes.SIZEOF_FLOAT);		 
				byte[] activeWeightByte = new byte[Bytes.SIZEOF_FLOAT];
				System.arraycopy(bytes,  count * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, activeWeightByte, 0,
						Bytes.SIZEOF_FLOAT);
				staticWeight = Bytes.toFloat(staticWeightByte);
				activeWeight = Bytes.toFloat(activeWeightByte);
		 		tmpScore = staticFactor * staticWeight + activeFactor * activeWeight;                
				if (maxScore < tmpScore) {
					maxScore = tmpScore;
				} 
				if (minScore > tmpScore) {
					minScore = tmpScore;
				}
			}			
			for (int count = 0; count < fiFrdNum; ++count) {
				int fofId = Bytes.toInt(bytes, count * kFriendFeatureUnitSize, Bytes.SIZEOF_INT);
				byte[] staticWeightByte = new byte[Bytes.SIZEOF_FLOAT];
				System.arraycopy(bytes,  count * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT, staticWeightByte, 0, Bytes.SIZEOF_FLOAT);		 
				byte[] activeWeightByte = new byte[Bytes.SIZEOF_FLOAT];
				System.arraycopy(bytes,  count * kFriendFeatureUnitSize + 2 * Bytes.SIZEOF_INT + Bytes.SIZEOF_FLOAT, activeWeightByte, 0,
						Bytes.SIZEOF_FLOAT);
				staticWeight = Bytes.toFloat(staticWeightByte);
				activeWeight = Bytes.toFloat(activeWeightByte);
		 		tmpScore = staticFactor * staticWeight + activeFactor * activeWeight;   
				weight = 10 + (int)(90 * (0.001 + tmpScore - minScore) / (0.001 + maxScore - minScore));
				logger.info("[getFoFRankValue]: friend Id " + Bytes.toInt(item.getFriendId()) + 
						" fiFrdNum " + fiFrdNum + " fofId " + fofId 
						+ " static weight "	+ staticWeight + 
						" active weight " + activeWeight + 
						" tmpScore " + tmpScore + 
						" weight " + weight);
				if (staticWeight == 0  && activeWeight == 0)
					weight = 10;
				friendsOfFriendsRankMap.put(fofId, weight);				
			}
     	    return friendsOfFriendsRankMap;
		}
		
    	public void map(IntWritable key, FriendFeatureStructureList values, Context context) throws IOException, InterruptedException {
    		if (values.getFriendFeatureStructureList().size() > 8000)
    			return;
    		int hostId = key.get();
    		FriendFeatureStructureList friendFeatureStructureListTest = values;
    		System.out.println("map: hostId " + key.get() + " friendNumber "
    				+ friendFeatureStructureListTest.getFriendNumber());
    		for (FriendFeatureStructure item : friendFeatureStructureListTest.getFriendFeatureStructureList()) {
    			int friendsNumOfFriend = item.getFriendFriendNumber();
    			System.out.print(" Friend Id " + Bytes.toInt(item.getFriendId()) + 
    					" time " + Bytes.toInt(item.getTime()) + 
    					" friend number " + friendsNumOfFriend + 
    					" static weight " + Bytes.toFloat(item.getStaticWeight()) + 
    					" active weight " + Bytes.toFloat(item.getActiveWeight()));
    			byte[] bytes = item.getBytes(); 
    			int friendNumber = bytes.length / kFriendFeatureUnitSize;
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
    				logger.info("map: friendId " + Bytes.toInt(friendId) + " time " + Bytes.toInt(time) + 
    						" staticWeight " + Bytes.toFloat(staticWeight) + " activeWeight " + Bytes.toFloat(activeWeight));
    			}			
    			
    			System.out.println("");
    		}
    		System.out.println("\n\n\n");
 		
			Set<Integer> blockSet = new HashSet<Integer>();
			Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >(); //to store all fofs id with time			
			//Prepare blocking set for fof recommendation, including hostId and friend Id 
			blockSet.add(hostId);			
			//Prepare formula items needed by the fof ranking model					
			List<FriendFeatureStructure> friendFeatureStructureList = values.getFriendFeatureStructureList();
			Map<Integer, Integer> friendRankMap = getFriendRankValue(friendFeatureStructureList);
			Map<Integer, Integer> friendsOfFriendsRankMap = new HashMap<Integer, Integer>();
			for (FriendFeatureStructure item : friendFeatureStructureList) {	
				int fiId = Bytes.toInt(item.getFriendId());
				blockSet.add(fiId);
				int fiFrdNum = item.getFriendFriendNumber();
				int timeHostFi = Bytes.toInt(item.getTime());				
				byte[] bytes = item.getBytes();		
				friendsOfFriendsRankMap = getFoFRankValue(item);
				for (int count = 0; count < fiFrdNum; ++count) {
					FoFRankFormula formula = new FoFRankFormula();
					formula.fiId = fiId;
					formula.timeHostFi = timeHostFi;
					formula.fiFrdNum = fiFrdNum;					
					int fofId = Bytes.toInt(bytes, count * kFriendFeatureUnitSize, Bytes.SIZEOF_INT);
					formula.timeFiFoF = Bytes.toInt(bytes, count * kFriendFeatureUnitSize + Bytes.SIZEOF_INT, 
							Bytes.SIZEOF_INT);			
					logger.info("hostId " + hostId + " fiId " + fiId + " timeHostFi " + formula.timeHostFi + " timeFiFoF " + formula.timeFiFoF);
					formula.rankHostFi = friendRankMap.get(fiId);
					formula.rankFiFoF = friendsOfFriendsRankMap.get(fofId);
					List<FoFRankFormula> foFRankFormulaList = fofId2FormulaListMap.get(fofId);
					logger.info("rankHostFi " + formula.rankHostFi + " rankFiFoF " + formula.rankFiFoF);
					if (foFRankFormulaList == null) {
						foFRankFormulaList = new ArrayList<FoFRankFormula>();
					    fofId2FormulaListMap.put(fofId, foFRankFormulaList);
					}
					foFRankFormulaList.add(formula);
				}						
			}						
		
			if (!fofId2FormulaListMap.isEmpty()) {
				
			    //FoFRankModel implementation
				List<FoFRank> fofRankList = FoFRankModel.getFoFRankList(fofId2FormulaListMap, blockSet);
				
				//Store ranking result to fof Table in HBase 
				if (!fofRankList.isEmpty())
				  putter.AddReocrd(hostId, fofRankList);
			}
			
			// Progress every RECORDNUM records to avoid map task timeout
			if ((++MapperCount % kRecordUnit) == 0) {
				context.setStatus("Finished processing " + MapperCount + " mapper records");
				context.progress();
			}
		}			
	}
	
	private final String inPath;
	private final String outTable;
	private final boolean isCreate;
	private final Configuration conf;
	
	private RcdFoFSeqToHBaseTest(String inPath, String outTable, boolean isCreate, Configuration conf) {
		this.inPath = inPath;
		this.outTable = outTable;
		this.conf = conf;
		this.isCreate = isCreate;
	}
	
	public void doIt() throws Exception {			
        HBaseAdmin hAdmin = new HBaseAdmin(this.conf);
	    boolean exists = hAdmin.tableExists(this.outTable);
	    if (!exists && !isCreate) {
	    	System.out.println("There is no such table: " + this.outTable + ", try run with -c");
	    	return;
	    }
	    else if (exists && isCreate) {
	    	hAdmin.disableTable(this.outTable);
	    	hAdmin.deleteTable(this.outTable);
	    }
	    if (isCreate) {
	    	HTableDescriptor htableDesc = new HTableDescriptor(this.outTable);
	    	HColumnDescriptor hcolumnDesc = new HColumnDescriptor("f");
	    	hcolumnDesc.setMaxVersions(1);
	    	hcolumnDesc.setBloomFilterType(BloomType.ROW);
//	    	hcolumnDesc.setCompressionType(Algorithm.SNAPPY);
//	    	hcolumnDesc.setCompactionCompressionType(Algorithm.SNAPPY);
		    htableDesc.addFamily(hcolumnDesc);	    
		    int numRegions = 30;	    
		    byte[] startKey = Bytes.toBytes(String.valueOf("00000000000000000000")); 
		    byte[] endKey = Bytes.toBytes(String.valueOf("FFFFFFFFFF9999999999")); 
		    System.out.println("Creating table with startkey=" + Bytes.toString(startKey) 
		                       + " endKey=" + Bytes.toString(endKey) + " with numRegions=" + numRegions);
	        hAdmin.createTable(htableDesc, startKey, endKey, numRegions);	   
	        hAdmin.close();	
	    }
		
	    Job job = new Job(this.conf);
	    job.setJarByClass(RcdFoFSeqToHBaseTest.class);
	    FileInputFormat.addInputPath(job, new Path(this.inPath));
	    job.setMapperClass(ReadExtendRelationMapper.class);
	    job.setNumReduceTasks(0);
	    job.setInputFormatClass(SequenceFileInputFormat.class);
	    job.setOutputFormatClass(NullOutputFormat.class);
        job.getConfiguration().set("OUTPUT_RTABLE", this.outTable);

	    System.exit(job.waitForCompletion(true) ? 0:1);
	}
	
	@SuppressWarnings("static-access")
	private static RcdFoFSeqToHBaseTest parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table name in HBase").withLongOpt("in")
				.create("i"));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output table in HBase").withLongOpt("out")
				.create("o"));
		opts.addOption(OptionBuilder.hasArg(false).isRequired(false)
				.withDescription("Create HBase table").withLongOpt("create")
				.create("c"));
	    opts.addOption(OptionBuilder.hasArg(true).isRequired(false)
	            .withDescription("Config file").withLongOpt("conf")
	            .create('f'));
		Configuration conf = new Configuration();
		conf = HBaseConfiguration.addHbaseResources(conf);
		
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String inPath = cmd.getOptionValue('i');
		String outTable = cmd.getOptionValue('o');
		boolean isCreate = cmd.hasOption('c');
		
	    if (cmd.hasOption('f')) {
	        String confname = cmd.getOptionValue('f');
	        Path p = new Path(confname);
	        System.out.println("Config file is: " + p.toString());
	        conf.addResource(p);
	    }
		return new RcdFoFSeqToHBaseTest(inPath, outTable, isCreate, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RcdFoFSeqToHBaseTest rff = parseArgs(args);
		rff.doIt();
	}
	
}