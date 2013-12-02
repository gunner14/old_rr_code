
package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.net.URISyntaxException;
import java.util.Date;

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
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.io.hfile.Compression.Algorithm;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.regionserver.StoreFile.BloomType;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;



public class ExtendHBaseToSeqHBase {

	public static final long RECORDNUM = 1000000; // number of records
	public static final byte[] COL_FAM = Bytes.toBytes("f");
	public static HTable desTable;
	
//	public static final int numRegions = 10000;
//	public static final String queue = "q";
	public static final int numRegions = 10;
	public static final String queue = "";

	public static class ExMapper extends TableMapper<NullWritable, NullWritable> {
		private long MapperCount = 0;
		
		protected void setup(Context context) throws IOException, InterruptedException {
			desTable = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
			desTable.setWriteBufferSize(5 * 1024 * 1024);
			desTable.setAutoFlush(false);
		}
		
		protected void cleanup (Context context) throws IOException, InterruptedException{
			System.out.println(new Date() + " cleaning up");
			desTable.flushCommits();
		}	
		
		@Override
		public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
			if (values.raw().length > 10000)
				return;
			int fofNumber = values.size();
			byte[] bytes = new byte[fofNumber * Bytes.SIZEOF_INT * 2];
			int pos = 0;
			for (KeyValue kv : values.raw()) {
				System.arraycopy(kv.getQualifier(), 0, bytes, pos * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				System.arraycopy(kv.getValue(), 0, bytes, (pos + 1)	* Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				pos = pos + 2;
			}	
			
			byte[] friendId = new byte[Bytes.SIZEOF_INT];
			byte[] time = new byte[Bytes.SIZEOF_INT];
			byte[] hostId = new byte[Bytes.SIZEOF_INT];
			int host = Integer.parseInt(Bytes.toString(row.get()));
			System.arraycopy(Bytes.toBytes(host), 0, hostId, 0, Bytes.SIZEOF_INT);
            
			int vsize = values.size();
			for (int i = 0, j = 0; i < vsize; ++i, j += 2) {
				System.arraycopy(bytes, j * Bytes.SIZEOF_INT, friendId, 0, Bytes.SIZEOF_INT);
				System.arraycopy(bytes, (j + 1) * Bytes.SIZEOF_INT, time, 0, Bytes.SIZEOF_INT);
				Put p = new Put(new StringBuffer(String.valueOf((Bytes.toInt(friendId)))).reverse().toString().getBytes());
				p.add(COL_FAM, hostId, Bytes.toInt(time), bytes);
			    p.setWriteToWAL(false);
			    desTable.put(p);
			}
			
			context.getCounter("FRTMapper", "USER_COUNT").increment(1);
			if ((++MapperCount % RECORDNUM) == 0) {
				context.setStatus("Finished processing " + MapperCount	+ " mapper records");
			}
		}
	}
	
	public final String input;
	public final String output;
	public final Configuration conf;

	public int doIt() throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
		System.out.println("[ExtendFriendsRelation] doIt");
		
		// Create HBase output table
		HBaseAdmin hAdmin = new HBaseAdmin(new Configuration());
	    if (hAdmin.tableExists(this.output)) {
	    	hAdmin.disableTable(this.output);
	    	hAdmin.deleteTable(this.output);
	    }
	    HTableDescriptor htableDesc = new HTableDescriptor(this.output);
	    HColumnDescriptor hcolumnDesc = new HColumnDescriptor(COL_FAM);
	    hcolumnDesc.setMaxVersions(1);
	    hcolumnDesc.setBloomFilterType(BloomType.ROW);
	    hcolumnDesc.setCompressionType(Algorithm.SNAPPY);
	    hcolumnDesc.setCompactionCompressionType(Algorithm.SNAPPY);
	    htableDesc.addFamily(hcolumnDesc);
	    
	    byte[] startKey = Bytes.toBytes(String.valueOf("0000000000")); 
	    byte[] endKey = Bytes.toBytes(String.valueOf("9999999999")); 
	    System.out.println("start " + startKey+ " end " + endKey);
        hAdmin.createTable(htableDesc, startKey, endKey, numRegions);	   
		hAdmin.close();	

		// Hadoop job configuration
		Job job = new Job(this.conf);		
		job.setJarByClass(ExtendHBaseToSeqHBase.class);
		job.setMapperClass(ExMapper.class);
		job.setNumReduceTasks(0);
	    
	    Scan scan = new Scan();
	    TableMapReduceUtil.initTableMapperJob(this.input, scan, ExMapper.class, NullWritable.class, NullWritable.class, job);
	    TableMapReduceUtil.setScannerCaching(job, 20000);
	    job.setOutputFormatClass(NullOutputFormat.class);
		
		job.getConfiguration().set("INPUT_RTABLE", this.input);
		job.getConfiguration().set("OUTPUT_RTABLE", this.output);
		
		JobUtils.InitJobOptimizations(job, queue);
		job.getConfiguration().setLong("hbase.hregion.max.filesize", 1024*1024*1024L);
		job.getConfiguration().setFloat("hfile.block.cache.size", 0.1F);
	    
		int result = job.waitForCompletion(true) ? 0 : 1;
		return result;
	}

	private ExtendHBaseToSeqHBase(String input, String output, Configuration conf) {
		this.input = input;
		this.output = output;
		this.conf = conf;
	}

	@SuppressWarnings("static-access")
	private static ExtendHBaseToSeqHBase parseArgs(String[] args)
			throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table").withLongOpt("in").create('i'));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output table").withLongOpt("out").create('o'));

		Configuration conf = new Configuration();
		HBaseConfiguration.addHbaseResources(conf);

		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();

		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String input = cmd.getOptionValue('i');
		String output = cmd.getOptionValue('o');
		return new ExtendHBaseToSeqHBase(input, output, conf);
	}

	public static void main(String[] args) throws Exception {
		ExtendHBaseToSeqHBase efr = parseArgs(args);
		efr.doIt();
	}
}

