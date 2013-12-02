package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.KeyValue;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.client.Scan;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.io.hfile.Compression.Algorithm;
import org.apache.hadoop.hbase.mapreduce.HFileOutputFormat;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.regionserver.StoreFile.BloomType;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;



public class ExtendHBaseToSeqHFile {
	public static final long RECORDNUM = 1000000; // number of records
	public static final byte[] COL_FAM = Bytes.toBytes("f");
	public static final int numRegions = 15000;
	public static final String queue = "q";
	public static final Algorithm algo = Algorithm.SNAPPY;

	public static class ExMapper extends TableMapper<ImmutableBytesWritable, KeyValue> {
		private long MapperCount = 0;
		@Override
		public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
			if (values.raw().length > 10000)
				return;
			int fofNumber = values.size();
			byte[] bytes = new byte[fofNumber * Bytes.SIZEOF_INT * 2 + Bytes.SIZEOF_INT];
			int pos = 1;
			for (KeyValue kv : values.raw()) {
				System.arraycopy(kv.getQualifier(), 0, bytes, pos * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				System.arraycopy(kv.getValue(), 0, bytes, (pos + 1)	* Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
				pos = pos + 2;
			}	
			
			byte[] friendId = new byte[Bytes.SIZEOF_INT];
			byte[] hostId = new byte[Bytes.SIZEOF_INT];
			int host = Integer.parseInt(Bytes.toString(row.get()));
			System.arraycopy(Bytes.toBytes(host), 0, hostId, 0, Bytes.SIZEOF_INT);
            
			int vsize = values.size();
			for (int i = 0, j = 1; i < vsize; ++i, j += 2) {
				System.arraycopy(bytes, j * Bytes.SIZEOF_INT, friendId, 0, Bytes.SIZEOF_INT);
				System.arraycopy(bytes, (j + 1) * Bytes.SIZEOF_INT, bytes, 0, Bytes.SIZEOF_INT);	// time of friend
				byte[] keybyte = new StringBuffer(String.valueOf(Bytes.toInt(friendId))).reverse().toString().getBytes();
				context.write(new ImmutableBytesWritable(keybyte), 
							  new KeyValue(keybyte, COL_FAM, hostId, bytes));
			}			

			if ((++MapperCount % RECORDNUM) == 0)
				context.setStatus("Finished processing " + MapperCount	+ " mapper records");
		}
	}

	public final String intable;
	public final String outpath;
	public final String desTable;
	public final boolean iscreate;
	
	public final Configuration conf;

	@SuppressWarnings("deprecation")
	public int doIt() throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
		System.out.println("[ExtendFriendsRelation] doIt");
		
		// Create HBase output table
		if (iscreate) {
			HBaseAdmin hAdmin = new HBaseAdmin(new Configuration());
		    if (hAdmin.tableExists(this.desTable)) {
		    	if (hAdmin.isTableEnabled(this.desTable))
		    		hAdmin.disableTable(this.desTable);
		    	hAdmin.deleteTable(this.desTable);
		    }
		    HTableDescriptor htableDesc = new HTableDescriptor(this.desTable);
		    HColumnDescriptor hcolumnDesc = new HColumnDescriptor(COL_FAM);
		    hcolumnDesc.setMaxVersions(1);
		    hcolumnDesc.setBloomFilterType(BloomType.ROW);
		    hcolumnDesc.setCompressionType(algo);
		    hcolumnDesc.setCompactionCompressionType(algo);
		    htableDesc.addFamily(hcolumnDesc);
		    
		    byte[] startKey = Bytes.toBytes(String.valueOf("0000000000")); 
		    byte[] endKey = Bytes.toBytes(String.valueOf("9999999999")); 
		    System.out.println("start " + startKey+ " end " + endKey);
	        hAdmin.createTable(htableDesc, startKey, endKey, numRegions);	   
			hAdmin.close();
		}
		
        Path p = new Path(this.outpath);
		FileSystem fs = FileSystem.get(new URI(this.outpath), conf);
		if (fs.exists(p) && fs.isDirectory(p)) {
			fs.delete(p);
		}

		// Hadoop job configuration
		Job job = new Job(this.conf);		
		job.setJarByClass(ExtendHBaseToSeqHFile.class);
		job.setMapperClass(ExMapper.class);
		
		JobUtils.InitJobOptimizations(job, queue);
		job.getConfiguration().setLong("hbase.hregion.max.filesize", 1024*1024*1024L);
	    
	    Scan scan = new Scan();
	    TableMapReduceUtil.initTableMapperJob(this.intable, scan, ExMapper.class, ImmutableBytesWritable.class, KeyValue.class, job);
	    TableMapReduceUtil.setScannerCaching(job, 20000);
	    
	    FileOutputFormat.setOutputPath(job, new Path(this.outpath));
        HTable htable = new HTable(this.conf, this.desTable); 
        HFileOutputFormat.configureIncrementalLoad(job, htable);
		
		int result = job.waitForCompletion(true) ? 0 : 1;
		return result;
	}

	private ExtendHBaseToSeqHFile(String intable, String outpath, String destable, boolean iscreate, Configuration conf) {
		this.intable = intable;
		this.outpath = outpath;
		this.desTable = destable;
		this.iscreate = iscreate;
		this.conf = conf;
	}

	@SuppressWarnings("static-access")
	private static ExtendHBaseToSeqHFile parseArgs(String[] args)
			throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table").withLongOpt("in").create('i'));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output path").withLongOpt("outpath").create('o'));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Dest table").withLongOpt("dest").create('d'));
		opts.addOption(OptionBuilder.hasArg(false).isRequired(false)
				.withDescription("Need create dest table").withLongOpt("create").create('c'));


		Configuration conf = new Configuration();
		HBaseConfiguration.addHbaseResources(conf);

		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String input = cmd.getOptionValue('i');
		String output = cmd.getOptionValue('o');
		String desTable = cmd.getOptionValue('d');
		boolean iscreate = cmd.hasOption('c') ? true : false;
		
		return new ExtendHBaseToSeqHFile(input, output, desTable, iscreate, conf);
	}

	public static void main(String[] args) throws Exception {
		ExtendHBaseToSeqHFile efr = parseArgs(args);
		efr.doIt();
	}
}

