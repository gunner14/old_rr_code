/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function:  Read raw data(including from userId, friendId, id of friends of friends, 
 * adding friends time among them) from extend_relation table in HBase
 * Date:  2012-04-16
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class RcdFoFHBaseToHBase {

	static class ReadExtendRelationMapper extends TableMapper<ImmutableBytesWritable, ImmutableBytesWritable> {
		public static HTable srcTable;
		public static HTable desTable;	
		
		protected void setup(
				org.apache.hadoop.mapreduce.Mapper<ImmutableBytesWritable, Result, ImmutableBytesWritable, ImmutableBytesWritable>.Context context) 
				throws IOException, InterruptedException{
			srcTable = new HTable(context.getConfiguration(), context.getConfiguration().get("INPUT_RTABLE"));
			desTable = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
		}
		
		protected void cleanup (
				org.apache.hadoop.mapreduce.Mapper<ImmutableBytesWritable, Result, ImmutableBytesWritable, ImmutableBytesWritable>.Context context) 
				throws IOException, InterruptedException{
//			srcTable.close();
			System.out.println(new Date() + " cleaning up");
			desTable.flushCommits();
			desTable.close();
		}	
		
		/***************************************************************************************
		 * extend_relation table record format:
		 * rowkey                t(columnfamily)    ff(columnfamily) 
		 * hostId-friendId(key)  v(qualifier):time  ffId1(qualifier):time1(value)...ffIdN(qualifier):timeN(value)
		 ****************************************************************************************/
//		private final static String columnFamily1 = "t";
		private final static String columnFamily = "ff";	
//		private int priorHostId = -1; //initial id indicating prior processed Id is set to -1
//		private byte[] priorQualifier = null;
//		private byte[] currentQualifer = null;
		private List<FoFRankFormula> fofRankFormulaList = new ArrayList<FoFRankFormula>();
    	public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
			int hostId = Bytes.toInt(row.get());
			Set<Integer> blockSet = new HashSet<Integer>();
			Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >(); //to store all fofs id with time	
//			System.out.println("hostId: " + hostId);		
			//Prepare blocking set for fof recommendation, including hostId and friend Id 
			blockSet.add(hostId);
//			for (KeyValue kv : values.raw()) {		
//				if (columnFamily.equals(Bytes.toString(kv.getFamily()))) {	
//					int fiId = Bytes.toInt(kv.getQualifier(), 0, Bytes.SIZEOF_INT);
//					blockSet.add(fiId);
////					System.out.println("block fiId " + fiId);
//				}
//			}			
			
//			//Prepare formula items needed by the fof ranking model			
			FoFRankFormula formula = new FoFRankFormula();
			for (KeyValue kv : values.raw()) {		
				if (columnFamily.equals(Bytes.toString(kv.getFamily()))) {		
					formula.fiId = Bytes.toInt(kv.getQualifier(), 0, Bytes.SIZEOF_INT);
//					System.out.println("fiId: " + formula.fiId);
					blockSet.add(formula.fiId);
					formula.timeHostFi = Bytes.toInt(kv.getQualifier(), 4, Bytes.SIZEOF_INT);
//					System.out.println("fiId " + formula.fiId + " timeHostFi " + formula.timeHostFi);	
					int fofId = 0;
					formula.fiFrdNum = Bytes.toInt(kv.getValue());
//					System.out.println("fiFrdNum: " + formula.fiFrdNum);
					FriendListWithTime flwt = new FriendListWithTime();
					byte[] value = kv.getValue();
					ByteArrayInputStream in = new ByteArrayInputStream(value);
					DataInputStream dataIn = new DataInputStream(in);
					flwt.readFields(dataIn);
					for (Entry<Integer, Integer> entry : flwt.data.entrySet()) {
						fofId = entry.getKey();
//						if (!blockSet.contains(fofId)) {
						formula.timeFiFoF = entry.getValue();
//						System.out.println("fofId " + fofId + " timeFiFoF " + formula.timeFiFoF);						
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
//						} else {
//							continue;
//						}
					}
					dataIn.close();						
   				}

			}
			
			if (!fofId2FormulaListMap.isEmpty()) {
				//FoFRankModel implementation
//				FoFRankModel fofRankModel = new FoFRankModel();
				//Store ranking result to fof Table in HBase 
//				List<FoFRank> fofRankList = fofRankModel.getFoFRankList(fofId2FormulaListMap, blockSet);
//	            new BuildRecommendFoFTable(hostId, fofRankModel.getFoFRankList(fofId2FormulaListMap, blockSet));
//	            new CreateFoFTable(hostId, fofRankList);
//	            System.out.println("after hostId: " + hostId);
	//			fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >();
	//            priorHostId = Integer.valueOf(hostIdWithFriendId[0]);
	//            System.out.println("after hostId " + hostId);
			}

		}
		
		
//		public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
//			String[] hostIdWithFriendId = Bytes.toString(row.get()).split("-");
//			int hostId = Integer.valueOf(hostIdWithFriendId[0]);				
//			System.out.println("priorHostId: " + priorHostId);			
//			//Prepare formula items needed by the fof ranking model			
//			FoFRankFormula formula = new FoFRankFormula();
//			formula.fiId = Integer.valueOf(hostIdWithFriendId[1]);
////			System.out.println("friendId: " + formula.fiId);
//			int fofId = 0;
//			formula.fiFrdNum = values.size(); //number of friends of host's friend fiId
////			System.out.println("fiFrdNum: " + formula.fiFrdNum);
//			for (KeyValue kv : values.list()) {
//				if (columnFamily1.equals(Bytes.toString(kv.getFamily()))) {
//					formula.timeHostFi = Bytes.toInt(kv.getValue());
////					System.out.println("timeHostFi: " + formula.timeHostFi);
//				}
//				if (columnFamily2.equals(Bytes.toString(kv.getFamily()))) {
//					fofId = Bytes.toInt(kv.getQualifier());
//					formula.timeFiFoF = Bytes.toInt(kv.getValue());
////					System.out.println("timeFiFoF: " + formula.timeFiFoF);
//					if (fofId2FormulaListMap.isEmpty()) {
//						fofRankFormulaList = new ArrayList<FoFRankFormula>();
//						fofRankFormulaList.add(formula); 
//						fofId2FormulaListMap.put(fofId, fofRankFormulaList);
//					} else {
//						if (null == fofId2FormulaListMap.get(fofId)) {
//							fofRankFormulaList = new ArrayList<FoFRankFormula>();
//							fofId2FormulaListMap.put(fofId, fofRankFormulaList);
//						}  else {
//							fofId2FormulaListMap.get(fofId).add(formula);
//						}
//					}
//				}
//			}
//			
//			//FoFRankModel implementation
//			FoFRankModel fofRankModel = new FoFRankModel();
//			//Store ranking result to fof Table in HBase 
//			fofRankModel.getFoFRankList(fofId2FormulaListMap);
////                new BuildRecommendFoFTable(hostId, fofRankModel.getFoFRankList(fofId2FormulaListMap));
//			fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >();
//            priorHostId = Integer.valueOf(hostIdWithFriendId[0]);
//			
//		}
		
	}
	
	private final String inTable;
	private final String outTable;
	private final Configuration conf;
	
	private RcdFoFHBaseToHBase(String inTable, String outTable, Configuration conf) {
		this.inTable = inTable;
		this.outTable = outTable;
		this.conf = conf;
	}
	
	public void doIt() throws Exception {
		Job job = new Job(this.conf);
		job.getConfiguration().set("INPUT_RTABLE", this.inTable);
		job.getConfiguration().set("OUTPUT_RTABLE", this.outTable);		
		job.setJarByClass(RcdFoFHBaseToHBase.class);
		job.setMapperClass(ReadExtendRelationMapper.class);
		job.setNumReduceTasks(0);
		Scan scan = new Scan();
		TableMapReduceUtil.initTableMapperJob(this.inTable, scan, ReadExtendRelationMapper.class, 
				ImmutableBytesWritable.class, ImmutableBytesWritable.class, job);
		job.setOutputFormatClass(NullOutputFormat.class);
		System.exit(job.waitForCompletion(true) ? 0:1);
	}
	
	@SuppressWarnings("static-access")
	private static RcdFoFHBaseToHBase parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table name in HBase").withLongOpt("in")
				.create("i"));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output table in HBase").withLongOpt("out")
				.create("o"));
		Configuration conf = new Configuration();
		conf = HBaseConfiguration.addHbaseResources(conf);
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String inTable = cmd.getOptionValue('i');
		String outTable = cmd.getOptionValue('o');
		return new RcdFoFHBaseToHBase(inTable, outTable, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RcdFoFHBaseToHBase rff = parseArgs(args);
		rff.doIt();
	}
	
}
