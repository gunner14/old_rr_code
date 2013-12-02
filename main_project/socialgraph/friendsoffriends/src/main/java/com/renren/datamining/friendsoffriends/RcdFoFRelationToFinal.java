/**************************************************************************************************
 * Developer:  Qu Zhi
 * Email:  zhi.qu@renren-inc.com
 * Function:  Read raw data(including from userId, friendId, adding friends time among them) from 
 * table in HBase
 * Date:  2012-04-16
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.net.URISyntaxException;
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
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.regionserver.StoreFile.BloomType;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;



public class RcdFoFRelationToFinal {

    public static final byte[] COL_FM = Bytes.toBytes("f");
	public static final int numRegions = 10;
	public static final String queue = "q";
//	public static final String queue = "";

    public static class CalcMapper extends TableMapper<NullWritable, NullWritable> {    	
        public static HTable desTable;
        public static HTable srcTable;
        public static CreateFoFTable putter = null;
        
        @Override
        protected void setup(Context context) throws IOException, InterruptedException {
        	srcTable = new HTable(context.getConfiguration(), context.getConfiguration().get("INPUT_RTABLE"));
            desTable = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
            desTable.setWriteBufferSize(5 * 1024 * 1024);
            desTable.setAutoFlush(false);            
            putter = new CreateFoFTableOneToAllGroup(COL_FM, 1000, 100, desTable);
        }
        
        @Override
        protected void cleanup (Context context) throws IOException, InterruptedException{
            System.out.println(new Date() + " cleaning up");
            desTable.flushCommits();
        }
        
        public void fillFriendData(IdTimeStructure[] friends) throws InterruptedException, IOException {
        	List<Get> batch = new ArrayList<Get>(friends.length);
        	for(IdTimeStructure x : friends) {
        		Get g = new Get(String.valueOf(Bytes.toInt(x.getFriendId())).getBytes());
        		g.addFamily(COL_FM);
        		batch.add(g);
        	}
        	Result[] results = srcTable.get(batch);
        	
        	for (int i = 0; i < results.length; ++i) {
        		Result values = results[i];
                int fofNumber = values.size();
    			byte[] bytes = new byte[fofNumber * Bytes.SIZEOF_INT * 2];
    			int pos = 0;
    			for (KeyValue kv : values.raw()) {
    				System.arraycopy(kv.getQualifier(), 0, bytes, pos * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
    				System.arraycopy(kv.getValue(), 0, bytes, (pos + 1)	* Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
    				pos += 2;
    			}
    			friends[i].setFofNumber(fofNumber);
    			friends[i].setBytes(bytes);
        	}        	
        }
        
        @Override
        public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
            if (values.raw().length > 8000)
                return; 
            int hostId = Integer.parseInt(Bytes.toString(row.get()));
            int friendNumber = values.size();
            
            IdTimeStructure[] friend = new IdTimeStructure[friendNumber];
            for (int i = 0; i < friendNumber; ++i) {
            	KeyValue kv = values.raw()[i];
            	friend[i] = new IdTimeStructure();
            	friend[i].setFriendId(kv.getQualifier());
            	friend[i].setTime(kv.getValue());
            }
            fillFriendData(friend);
            
			Set<Integer> blockSet = new HashSet<Integer>();
			Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >();	
			blockSet.add(hostId);
			for (IdTimeStructure item : friend) {	
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
				List<FoFRank> fofRankList = FoFRankModel.getFoFRankList(fofId2FormulaListMap, blockSet); 
				if (!fofRankList.isEmpty())
				  putter.AddReocrd(hostId, fofRankList);
			}
        }
    }

//    public static class CalcReducer extends Reducer<IntWritable, IdTimeStructure, NullWritable, NullWritable> {
//      
//      public static HTable desTable;
//      public static CreateFoFTable putter = null;
//      
//      @Override
//      protected void setup(Context context) throws IOException, InterruptedException {
//          desTable = new HTable(context.getConfiguration().get("OUTPUT_RTABLE"));
//          desTable.setWriteBufferSize(5 * 1024 * 1024);
//          desTable.setAutoFlush(false);
//          
//          putter = new CreateFoFTable_OneToAll_Group(Bytes.toBytes("f"), 1000, 100, desTable);
//      }
//      
//      @Override
//      protected void cleanup (Context context) throws IOException, InterruptedException{
//          System.out.println(new Date() + " cleaning up");
//          desTable.flushCommits();
//      }           
//
//      @Override   
//      protected void reduce(IntWritable key, Iterable<IdTimeStructure> values, Context context) throws IOException, InterruptedException {
//        
//        int hostId = key.get();
//        Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap = new HashMap<Integer, List<FoFRankFormula> >();  
//        Set<Integer> blockSet = new HashSet<Integer>();
//        blockSet.add(hostId);
//           
//        for (IdTimeStructure item : values) {
//            int fof_num = item.getFofNumber();
//            for (int count = 0; count < fof_num; ++count) {
//                FoFRankFormula formula = new FoFRankFormula();
//                formula.fiId = Bytes.toInt(item.getFriendId());
//                blockSet.add(formula.fiId);
//                formula.timeHostFi = Bytes.toInt(item.getTime());
//                formula.fiFrdNum = item.getFofNumber();
//                int fofId = Bytes.toInt(item.getBytes(), count * 2 * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//                formula.timeFiFoF = Bytes.toInt(item.getBytes(), (count * 2 + 1) * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//
//                List<FoFRankFormula> p = fofId2FormulaListMap.get(fofId);
//                if (p == null) {
//                  p = new ArrayList<FoFRankFormula>();
//                  fofId2FormulaListMap.put(fofId, p);
//                }
//                p.add(formula);
//            }                       
//        }                       
//    
//        if (!fofId2FormulaListMap.isEmpty()) {
//            List<FoFRank> fofRankList = FoFRankModel.getFoFRankList(fofId2FormulaListMap, blockSet); 
//            if (!fofRankList.isEmpty())
//              putter.AddReocrd(hostId, fofRankList);
//        }
//      }
//  }
    
    public final String input;
    public final String output;
    public final Configuration conf;
    
    public void InitOutputTable() throws IOException {
      HBaseAdmin hAdmin = new HBaseAdmin(new Configuration());
      boolean exists = hAdmin.tableExists(this.output);
      if (exists) {
          hAdmin.disableTable(this.output);
          hAdmin.deleteTable(this.output);
      }
      
      HTableDescriptor htableDesc = new HTableDescriptor(this.output);
      HColumnDescriptor hcolumnDesc = new HColumnDescriptor(COL_FM);
      hcolumnDesc.setMaxVersions(1);
      hcolumnDesc.setBloomFilterType(BloomType.ROW);
      htableDesc.addFamily(hcolumnDesc);      
              
      byte[] startKey = Bytes.toBytes(String.valueOf("0000000000")); 
      byte[] endKey = Bytes.toBytes(String.valueOf("9999999999")); 
      System.out.println("start " + startKey + " end " + endKey);
      hAdmin.createTable(htableDesc, startKey, endKey, numRegions);      
      hAdmin.close(); 
    }

    public int doIt() throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
        System.out.println("[RelationToFinal] doIt");
        Job job = new Job(this.conf);
        
        // Initialize Output Table
        InitOutputTable();
        
        // Used parameters in MapReduce.
        job.getConfiguration().set("INPUT_RTABLE", this.input);
        job.getConfiguration().set("OUTPUT_RTABLE", this.output);
        
        // MapReduce Input and output settings.
        job.setJarByClass(RcdFoFRelationToFinal.class);
        job.setMapperClass(CalcMapper.class);   
        job.setNumReduceTasks(0);
        job.setOutputFormatClass(NullOutputFormat.class);
        Scan scan = new Scan();
        TableMapReduceUtil.initTableMapperJob(this.input, scan, CalcMapper.class, NullWritable.class, NullWritable.class, job);
        TableMapReduceUtil.setScannerCaching(job, 20000);
        
        JobUtils.InitJobOptimizations(job, queue);
        job.getConfiguration().setFloat("hfile.block.cache.size", 0.4F);

        int result = job.waitForCompletion(true) ? 0 : 1;
        return result;
    }


    private RcdFoFRelationToFinal(String input, String output, Configuration conf) {
        this.input = input;
        this.output = output;
        this.conf = conf;
    }

    @SuppressWarnings("static-access")
    private static RcdFoFRelationToFinal parseArgs(String[] args) throws IOException, ParseException {
        Options opts = new Options();
        opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
                .withDescription("Input table").withLongOpt("in").create('i'));
        opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
                .withDescription("Output table").withLongOpt("out").create('o'));

        Configuration conf = new Configuration();
        conf = HBaseConfiguration.addHbaseResources(conf);
        
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        PosixParser parser = new PosixParser();
        CommandLine cmd = parser.parse(opts, otherArgs);
        String input = cmd.getOptionValue('i');
        String output = cmd.getOptionValue('o');
        return new RcdFoFRelationToFinal(input, output, conf);
    }

    public static void main(String[] args) throws Exception {
        RcdFoFRelationToFinal efr = parseArgs(args);
        efr.doIt();
    }
}
