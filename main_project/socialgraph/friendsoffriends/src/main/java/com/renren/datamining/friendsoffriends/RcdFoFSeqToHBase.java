/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  	   junwu.xiong@renren-inc.com
 * Function:   Read raw data(including from userId, friendId, id of friends of friends, building 
 * friends relation time among them) from extend_relation sequence file, compute them and then store
 * them to HBase 
 * Date:  2012-04-16
 *********************************************************************************************************************
 * Sequence file record format:
 *  key                                               value
 *  hostId         friendNumber IdTimeStructureList(IdTimeStructure: friendId time fofNumber friendIdList)
 *  HBase table format
 *  rowkey             columnfamily:quailfier:value
*   hostId             f:fofId|rank1|rank2(-1)|mutualfriendnumber|mutualfriendIdList 
**********************************************************************************************************************/



package com.renren.datamining.friendsoffriends;

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
import org.apache.hadoop.hbase.io.hfile.Compression.Algorithm;
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


public class RcdFoFSeqToHBase {	
	public static final long RECORDNUM = 800; // unit of count for number of records
	public static class ReadExtendRelationMapper extends Mapper<IntWritable, IdTimeStructureList, NullWritable, NullWritable> {
		private static long MapperCount = 0;
		public static HTable desTable;
		public static CreateFoFTable  putter = null;
		
		protected void setup(
				Mapper<IntWritable, IdTimeStructureList, NullWritable, NullWritable>.Context context) 
				throws IOException, InterruptedException {
			desTable = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
			desTable.setWriteBufferSize(10 * 1024 * 1024);
			desTable.setAutoFlush(false);
			
			//putter = new CreateFoFTable_OneToAll(Bytes.toBytes("f"), 1000, desTable);
			putter = new CreateFoFTableOneToAllGroup(Bytes.toBytes("f"), 1000, 100, desTable);
		}
		
		protected void cleanup (
				Mapper<IntWritable, IdTimeStructureList, NullWritable, NullWritable>.Context context) 
				throws IOException, InterruptedException{
			System.out.println(new Date() + " cleaning up");
			desTable.flushCommits();
		}			

    	public void map(IntWritable key, IdTimeStructureList values, Context context) throws IOException, InterruptedException {
    		if (values.getIdTimeStructureList().size() > 8000)
    			return;
    		int hostId = key.get();

			Set<Integer> blockSet = new HashSet<Integer>();
			Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >(); //to store all fofs id with time	
			
			//Prepare blocking set for fof recommendation, including hostId and friend Id 
			blockSet.add(hostId);
			
			//Prepare formula items needed by the fof ranking model				
			for (IdTimeStructure item : values.getIdTimeStructureList()) {	
				for (int count = 0; count < item.getFofNumber(); ++count) {
					FoFRankFormula formula = new FoFRankFormula();
					formula.fiId = Bytes.toInt(item.getFriendId());
					blockSet.add(formula.fiId);
					formula.timeHostFi = Bytes.toInt(item.getTime());
					formula.fiFrdNum = item.getFofNumber();
					int fofId = Bytes.toInt(item.getBytes(), count * 2 * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
					formula.timeFiFoF = Bytes.toInt(item.getBytes(), (count * 2 + 1) * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);

					List<FoFRankFormula> p = fofId2FormulaListMap.get(fofId);
					if (p == null) {
					  p = new ArrayList<FoFRankFormula>();
					  fofId2FormulaListMap.put(fofId, p);
					}
					p.add(formula);
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
			if ((++MapperCount % RECORDNUM) == 0) {
				context.setStatus("Finished processing " + MapperCount + " mapper records");
				context.progress();
			}
		}			
	}
	
	private final String inPath;
	private final String outTable;
	private final boolean isCreate;
	private final Configuration conf;
	
	private RcdFoFSeqToHBase(String inPath, String outTable, boolean isCreate, Configuration conf) {
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
	    job.setJarByClass(RcdFoFSeqToHBase.class);
	    FileInputFormat.addInputPath(job, new Path(this.inPath));
	    job.setMapperClass(ReadExtendRelationMapper.class);
	    job.setNumReduceTasks(0);
	    job.setInputFormatClass(SequenceFileInputFormat.class);
	    job.setOutputFormatClass(NullOutputFormat.class);
        job.getConfiguration().set("OUTPUT_RTABLE", this.outTable);

	    System.exit(job.waitForCompletion(true) ? 0:1);
	}
	
	@SuppressWarnings("static-access")
	private static RcdFoFSeqToHBase parseArgs(String[] args) throws IOException, ParseException {
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
		return new RcdFoFSeqToHBase(inPath, outTable, isCreate, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RcdFoFSeqToHBase rff = parseArgs(args);
		rff.doIt();
	}
	
}

