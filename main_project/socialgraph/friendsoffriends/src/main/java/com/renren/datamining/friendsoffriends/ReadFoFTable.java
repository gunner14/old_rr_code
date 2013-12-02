/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:      junwu.xiong@renren-inc.com
 * Function:   Write model final ranking values to fof table in HBase
 * Date:  2012-07-02
*********************************************************************************************************************
*  HBase table format
*  rowkey             columnfamily:quailfier:value
*  hostIdfofId		   f|rank1|rank2(-1)|mutualfriendnumber|mutualfriendIdList 
*  but now the format is as follows:
*  hostIdfofId        fofId|rank1|rank2(-1)| mutualFriendNumber|mutualFriendIdList
*********************************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
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
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ReadFoFTable {
	public  static class ReadFoFTableMap extends
    TableMapper<Text, Text> {
		public static final long CountUnit = 100000;
		public static long mapperCount = 0;
		private HTable fofTable;		
		protected void setup( org.apache.hadoop.mapreduce.Mapper<ImmutableBytesWritable, Result, Text, Text>.Context context ) 
				throws IOException, InterruptedException{
			fofTable = new HTable(context.getConfiguration(), context.getConfiguration().get("FOF_TABLE"));
		}
		
		protected void cleanup( org.apache.hadoop.mapreduce.Mapper<ImmutableBytesWritable, Result, Text, Text>.Context context ) 
				throws IOException, InterruptedException{
			fofTable.close();
		}

		public void map(ImmutableBytesWritable row, Result values, Context context ){			
			final byte[] key = row.get();
			final int hostId = Bytes.toInt(key, 0, Bytes.SIZEOF_INT);
			final int fofId= Bytes.toInt(key, Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//			System.out.print(" hostId " + hostId + " fofId " + fofId);
			float rank = 0.0F;
			int mutualFriendNumber = 0;
			int mutualFriendId = 0;
//			for (KeyValue kv : values.raw()) {	
			KeyValue[] kvs = values.raw();			
			rank = Bytes.toFloat(kvs[0].getValue(), 0);
//			System.out.print(" rank " + rank);
			mutualFriendNumber = Bytes.toInt(kvs[0].getValue(), 2*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//			mutualFriendNumber = Bytes.toInt(kvs[0].getValue(), 4*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//			System.out.print(" mutual friend number " + mutualFriendNumber);
			if (mutualFriendNumber > 0  &&  (mutualFriendNumber + 3 ==  (kvs[0].getValueLength()) / 4 *Bytes.SIZEOF_BYTE)) {
				for (int i = 0; i < mutualFriendNumber; ++i) {
					mutualFriendId = Bytes.toInt(kvs[0].getValue(), (i+3)*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//						mutualFriendId = Bytes.toInt(kvs[0].getValue(), (i+5)*Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
//					System.out.print(" " + i + " "+ mutualFriendId);
				} 
				System.out.println("");			
			} else {
				System.out.println("mutualFriendNumber " + mutualFriendNumber + " value length " + kvs[0].getValueLength());
			}
//			}		
			
			if (++mapperCount % CountUnit == 0) {
				context.setStatus("mapperCount " + mapperCount);
			}
		}						
	}
	
    private final String fofTableName;
    private final Configuration conf;
    private ReadFoFTable(String fofTableName, Configuration conf){
	    this.fofTableName = fofTableName;
	    this.conf = conf;
	}
	  
	public int doIt() throws IOException, InterruptedException,
	      ClassNotFoundException {
	    System.out.println("[Predictor1] doIt");
	    Job job = new Job(this.conf);
	    job.getConfiguration().set("FOF_TABLE", this.fofTableName);	    
	    job.setJarByClass(ReadFoFTable.class);
	    job.setMapperClass(ReadFoFTableMap.class);
	    job.setNumReduceTasks(0);
	    job.getConfiguration().set("mapred.map.tasks.speculative.execution",  "false");
	    job.getConfiguration().set("hbase.regionserver.lease.period", "300000");
	    job.getConfiguration().set("hbase.rpc.timeout", "400000");
	    
	    job.getConfiguration().set("mapred.queue.names","q2");
		job.getConfiguration().set("mapred.job.queue.name","q2");
	    
	    Scan scan = new Scan();
	    TableMapReduceUtil.initTableMapperJob(this.fofTableName, scan, ReadFoFTableMap.class,
	    		Text.class, Text.class, job);
	    job.setOutputFormatClass(NullOutputFormat.class);
	    return job.waitForCompletion(true) ? 0 : 1;
	  }
	  
	  @SuppressWarnings("static-access")
	  private static ReadFoFTable parseArgs(String[] args)
	      throws IOException, ParseException {
	    Options opts = new Options();
	    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
	        .withDescription("Input fof table").withLongOpt("fof").create('f'));

	    Configuration conf = HBaseConfiguration.create();
	    String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();

	    PosixParser parser = new PosixParser();
	    CommandLine cmd = parser.parse(opts, otherArgs);
	    String fofTableName = cmd.getOptionValue('f');
	    return new ReadFoFTable(fofTableName, conf);
	  }
	  
	  public static void main(String[] args) throws Exception {
		  ReadFoFTable pred = parseArgs( args );
		  pred.doIt();
	  }
}

