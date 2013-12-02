/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function:  Read raw data(including from userId, friendId, id of friends of friends, 
 * adding friends time among them) from extend_relation table in HBase
 * Date:  2012-04-16
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class RcdFoFSeqToHDFS {	
//	public static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory.getLog(RcdFoF_SeqToHDFS.class);
	public static final long RECORDNUM = 1000000; // unit of counter for number of records 
 	static final int kMaxFoFNumber = 100;         //max number recommended for each hostId 
	public static class ReadExtendRelationMapper extends Mapper<IntWritable, IdTimeStructureList, Text, Text> {
		private static long MapperCount = 0;
		protected void setup(
				Mapper<IntWritable, IdTimeStructureList, Text, Text>.Context context) 
				throws IOException, InterruptedException{
		}
		
		protected void cleanup (
				Mapper<IntWritable, IdTimeStructureList, Text, Text>.Context context) 
				throws IOException, InterruptedException{
			System.out.println(new Date() + " cleaning up");

		}	
		
    	public void map(IntWritable key, IdTimeStructureList values, Context context) throws IOException, InterruptedException {
    		int hostId = key.get();
    		System.out.println("hostId " + hostId);
    		List<FoFRankFormula> fofRankFormulaList = new ArrayList<FoFRankFormula>();
			Set<Integer> blockSet = new HashSet<Integer>();
			Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >(); //to store all fofs id with time	
			//Prepare blocking set for fof recommendation, including hostId and friend Id 
			blockSet.add(hostId);

			//Prepare formula items needed by the fof ranking model				
			int fofId = 0;
			for (IdTimeStructure item : values.getIdTimeStructureList()) {		
				for (int count = 0; count < item.getFofNumber(); ++count) {
					FoFRankFormula formula = new FoFRankFormula();
					formula.fiId = Bytes.toInt(item.getFriendId());
//					System.out.println(" fiId: " + i++ + " " + formula.fiId);
					blockSet.add(formula.fiId);
					formula.timeHostFi = Bytes.toInt(item.getTime());
//					System.out.println("fiId " + formula.fiId + " timeHostFi " + formula.timeHostFi);	
					formula.fiFrdNum = item.getFofNumber();

					fofId = Bytes.toInt(item.getBytes(), count * 2 * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
					formula.timeFiFoF = Bytes.toInt(item.getBytes(), (count * 2 + 1) * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//					System.out.println("fofId " + fofId + " timeFiFoF " + formula.timeFiFoF);						
					if (fofId2FormulaListMap.isEmpty()) {
						fofRankFormulaList = new ArrayList<FoFRankFormula>();
						fofRankFormulaList.add(formula); 
						fofId2FormulaListMap.put(fofId, fofRankFormulaList);						
					} else {
						if (null == fofId2FormulaListMap.get(fofId)) {
							fofRankFormulaList = new ArrayList<FoFRankFormula>();
							fofRankFormulaList.add(formula); 
							fofId2FormulaListMap.put(fofId, fofRankFormulaList);
						}  else {
							fofId2FormulaListMap.get(fofId).add(formula);
						}
					} 
				}						
			}	
			
//			logger.info("host's fofId number " + fofId2FormulaListMap.size());
			if (!fofId2FormulaListMap.isEmpty()) {
				//FoFRankModel implementation
				List<FoFRank> fofRankList = FoFRankModel.getFoFRankList(fofId2FormulaListMap, blockSet);
				 int counter = 0;		
				 int mutualFriendsNumber = 0;		
				 Iterator<FoFRank> it = fofRankList.iterator();
				 FoFRank ffr = null;
//				 while ((it.hasNext()) && (counter++ < kMaxFoFNumber)) {
				 while (it.hasNext()) {
					 ffr = it.next();
					 String id = String.valueOf(hostId) + '\t' + String.valueOf(ffr.fofId); 
					 Text hostIdfofId= new Text(id);
					 StringBuffer sb = new StringBuffer();
				     mutualFriendsNumber = ffr.mutualFriendIdListSize;
				     sb.append("\t").append(ffr.mutualFriendNumber);
				     sb.append("\t").append(ffr.rank);
				     if (mutualFriendsNumber > 0) {
					     for (int i = 0; i < mutualFriendsNumber; ++i) {
					    	 sb.append("\t").append(ffr.mutualFriendIdList[i]);
					     }						 
						 context.write(hostIdfofId, new Text(sb.toString()));											 
				     }
				 }
			}				
			
			if ((++MapperCount % RECORDNUM) == 0) {
				context.setStatus("Finished processing " + MapperCount
						+ " mapper records");
			}
		}			
	}	

	private final String inPath;
	private final String outPath;
	private final Configuration conf;
	
	private RcdFoFSeqToHDFS(String inPath, String outPath, Configuration conf) {
		this.inPath = inPath;
		this.outPath = outPath;
		this.conf = conf;
	}
	
	@SuppressWarnings("deprecation")
	public void doIt() throws Exception {			

		
		Job job = new Job(this.conf);
		job.setJarByClass(RcdFoFSeqToHDFS.class);
        FileInputFormat.addInputPath(job, new Path(this.inPath));	
        Path p = new Path(this.outPath);
		FileSystem fs = FileSystem.get(new URI(this.outPath), conf);
		if (fs.exists(p) && fs.isDirectory(p)) {
			fs.delete(p);
		}
		FileOutputFormat.setOutputPath(job, new Path(this.outPath));
		job.getConfiguration().set("INPUT_RTABLE", this.inPath);
		job.getConfiguration().set("OUTPUT_RTABLE", this.outPath);
		
		JobUtils.InitJobOptimizations(job, "");
		
		job.setMapperClass(ReadExtendRelationMapper.class);
		job.setNumReduceTasks(0);
		job.setInputFormatClass(SequenceFileInputFormat.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		System.exit(job.waitForCompletion(true) ? 0:1);
	}
	
	@SuppressWarnings("static-access")
	private static RcdFoFSeqToHDFS parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table name in HBase").withLongOpt("in")
				.create("i"));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output table in HBase").withLongOpt("out")
				.create("o"));
		Configuration conf = new Configuration();
		
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String inPath = cmd.getOptionValue('i');
		String outTable = cmd.getOptionValue('o');
		return new RcdFoFSeqToHDFS(inPath, outTable, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RcdFoFSeqToHDFS rff = parseArgs(args);
		rff.doIt();
	}
	
}


