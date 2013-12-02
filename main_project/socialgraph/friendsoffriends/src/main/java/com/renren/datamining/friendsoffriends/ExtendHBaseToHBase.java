/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function:  Read raw data(including from userId, friendId, adding friends time among them) from 
 * table in HBase
 * Date:  2012-04-16
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.mapreduce.TableReducer;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.util.Writables;
import org.apache.hadoop.io.compress.BZip2Codec;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ExtendHBaseToHBase {
  public static final byte[] COL_T = Bytes.toBytes("t");
  public static final byte[] COL_FF = Bytes.toBytes("ff");
  public static final byte[] QUAL_V = Bytes.toBytes("v");
  public static final long RECORDNUM = 1000000; //number of records  
  public static final int partitionNum = 87;

  public static class ExMapper extends
      TableMapper<ImmutableBytesWritable, ImmutableBytesWritable> {
	  private long MapperCount = 0;	

    @Override
    public void map(ImmutableBytesWritable row, Result values, Context context)
        throws IOException {
      int hostId = Bytes.toInt(row.get());
      if (values.raw().length > 10000) {
        System.out.println(hostId + " has " + values.raw().length
            + " friends. ignore all of them.");
        return;
      }

      FriendListWithTime flwt = new FriendListWithTime();
      for (KeyValue friendsKV : values.raw()) {
          flwt.data.put(Bytes.toInt(friendsKV.getQualifier()),
              Bytes.toInt(friendsKV.getValue()));
      }
      for (Entry<Integer, Integer> entry : flwt.data.entrySet()) {
    	  int friend = entry.getKey();
    	  int time = entry.getValue();
    	  byte[] qualifierPart1 = row.get();
          byte[] qualifierPart2 = Bytes.toBytes(time);
          byte[] hostFriendList = Writables.getBytes(flwt);
          ByteArrayOutputStream baos = new ByteArrayOutputStream();
          baos.write(qualifierPart1, 0, Bytes.SIZEOF_INT);
          baos.write(qualifierPart2, 0, Bytes.SIZEOF_INT);
          baos.write(hostFriendList, 0, hostFriendList.length);            
          try {
			context.write(new ImmutableBytesWritable(Bytes.toBytes(friend)), new ImmutableBytesWritable(baos.toByteArray()));
          } catch (InterruptedException e) {
			e.printStackTrace();
          } 
    
      }
      context.getCounter("FRTMapper", "USER_COUNT").increment(1);
      
      if ((++MapperCount % RECORDNUM) == 0) {
    	  context.setStatus("Finished processing " + MapperCount + " mapper records");
      }     
    }
  }
  
  public static class ExCombiner extends Reducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable> {
	  private long CombinerCount = 0;
	   
	    protected void reduce(
	    		ImmutableBytesWritable key, 
	    		java.lang.Iterable<ImmutableBytesWritable> values, 
	    		org.apache.hadoop.mapreduce.Reducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable>.Context context)
	    		throws IOException, InterruptedException{		 
	    	Iterator<ImmutableBytesWritable> it = values.iterator();
	    	while(it.hasNext()){
		        context.write(key, it.next());
	    	} 		        
	        context.getCounter("ExCombiner", "USER_COUNT").increment(1);        
	        if ((++CombinerCount % RECORDNUM) == 0) {
	      	  context.setStatus("Finished processing " + CombinerCount + " reducer records");
	        }     
	    }
  }
  
  public static class ExPartitioner extends org.apache.hadoop.mapreduce.Partitioner<ImmutableBytesWritable, ImmutableBytesWritable>{
	@Override
	public int getPartition(ImmutableBytesWritable key, ImmutableBytesWritable value,
			int numPartitions) {
        int hostId = Bytes.toInt(key.get());
		return hostId % 1;
	}	  
  }

  public static class ExReducer extends TableReducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable> {
	  private HTable desTable;    
	  private long ReducerCount = 0;
	  @Override
	    protected void setup(
	        org.apache.hadoop.mapreduce.Reducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable, org.apache.hadoop.io.Writable>.Context context)
	        throws IOException, InterruptedException {
	      desTable = new HTable(context.getConfiguration(), context
	          .getConfiguration().get("OUTPUT_RTABLE"));
	      desTable.setWriteBufferSize(2048 * 1000 * 1000);
	      desTable.setAutoFlush(true);
	    };

	    @Override
	    protected void cleanup(
	        org.apache.hadoop.mapreduce.Reducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable, org.apache.hadoop.io.Writable>.Context context)
	        throws IOException, InterruptedException {
	      System.out.println(new Date() + " cleaning up");
	      desTable.flushCommits();
	      desTable.close();
	    };
    
	    protected void reduce(
	    		ImmutableBytesWritable key, 
	    		java.lang.Iterable<ImmutableBytesWritable> values, 
	    		org.apache.hadoop.mapreduce.Reducer<ImmutableBytesWritable, ImmutableBytesWritable, ImmutableBytesWritable, org.apache.hadoop.io.Writable>.Context context)
	    		throws IOException, InterruptedException{	    	
	        List<Put> puts = new ArrayList<Put>();        
	        for (ImmutableBytesWritable item : values) {	        
	        	int friend = Bytes.toInt(item.get(), 0, Bytes.SIZEOF_INT);
	        	int time = Bytes.toInt(item.get(), 4, Bytes.SIZEOF_INT);
	        	byte[] hostFriendList = new byte[item.get().length-8];   
	        	System.arraycopy(item.get(), 8, hostFriendList, 0, hostFriendList.length);
	        	Put put = new Put(key.get());	        
	        	byte[] qualifier = Bytes.add(Bytes.toBytes(friend), Bytes.toBytes(time)); //		        System.out.println("friend "+ friend + " hostId " + hostId+" time "+time);
		        put.add(COL_FF, qualifier, hostFriendList); 
              put.setWriteToWAL(false);

		        puts.add(put);	        	
	        }	        
//	        desTable.put(puts);
	        context.getCounter("FRTReducer", "USER_COUNT").increment(1); 
	        
	        if ((++ReducerCount % RECORDNUM) == 0) {
	      	  context.setStatus("Finished processing " + ReducerCount + " reducer records");
	        }     
	    }
  }

  private final String input;
  private final String output;
  private final Configuration conf;
//
  public int doIt() throws IOException, InterruptedException,
      ClassNotFoundException {
    System.out.println("[ExtendFriendsRelation] doIt");
    Job job = new Job(this.conf);
    job.getConfiguration().set("INPUT_RTABLE", this.input);
//    job.getConfiguration().set("OUTPUT_RTABLE", this.output);
    FileOutputFormat.setOutputPath(job, new Path(this.output));
    job.setJarByClass(ExtendHBaseToHBase.class);
    job.setMapperClass(ExMapper.class);
    
    job.getConfiguration().set("mapred.map.tasks.speculative.execution",
        "false");
    
    job.getConfiguration().setBoolean("mapred.output.compression", true);
    job.getConfiguration().setClass("mapred.output.compression.codec", BZip2Codec.class, CompressionCodec.class); 
//    job.getConfiguration().setClass("mapred.output.compression.codec", LzoCodec.class, CompressionCodec.class);    

    
    job.setCombinerClass(ExCombiner.class);
    
    job.setPartitionerClass(ExPartitioner.class);
    
    job.setReducerClass(ExReducer.class);
    job.getConfiguration().set("mapred.reduce.tasks.speculative.execution",
    		  "false");
    job.setNumReduceTasks(partitionNum);
//    job.setNumReduceTasks(0);
//    job.setOutputFormatClass(NullOutputFormat.class);
    Scan scan = new Scan();
    scan.setCaching(2000);
    TableMapReduceUtil.initTableMapperJob(this.input, scan, ExMapper.class,
    ImmutableBytesWritable.class, ImmutableBytesWritable.class, job);
//    TableMapReduceUtil.initTableReducerJob(this.output, ExReducer.class, job);
//    job.setOutputFormatClass(NullOutputFormat.class);
    return job.waitForCompletion(true) ? 0 : 1;
  }

   private ExtendHBaseToHBase(String input, String output, Configuration conf) {
      this.input = input;
      this.output = output;
      this.conf = conf;
   }
  @SuppressWarnings("static-access")
  private static ExtendHBaseToHBase parseArgs(String[] args)
      throws IOException, ParseException {
    Options opts = new Options();
    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
        .withDescription("Input table").withLongOpt("in").create('i'));
    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
        .withDescription("Output diretory").withLongOpt("out").create('o'));

    Configuration conf = HBaseConfiguration.create();
    
    String[] otherArgs = new GenericOptionsParser(conf, args)
        .getRemainingArgs();

    PosixParser parser = new PosixParser();
    CommandLine cmd = parser.parse(opts, otherArgs);
    String input = cmd.getOptionValue('i');
    String output = cmd.getOptionValue('o');
    return new ExtendHBaseToHBase(input, output, conf);
}

  public static void main(String[] args) throws Exception {
    ExtendHBaseToHBase efr = parseArgs(args);
    efr.doIt();
  }
}

