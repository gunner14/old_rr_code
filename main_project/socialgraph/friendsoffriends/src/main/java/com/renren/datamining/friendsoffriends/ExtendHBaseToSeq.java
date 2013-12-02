/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function:  Read raw data(including from userId, friendId, adding friends time among them) from 
 * table in HBase
 * Date:  2012-04-16
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.List;

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



public class ExtendHBaseToSeq {
	public static final long RECORDNUM = 1000000; // number of records
	public static final int partitionNum = 300;

	public static class ExMapper extends TableMapper<IntWritable, IdTimeStructure> {
		private long MapperCount = 0;
		@Override
		public void map(ImmutableBytesWritable row, Result values, Context context) throws IOException, InterruptedException {
			if (values.raw().length > 8000)
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
				context.write(new IntWritable(Bytes.toInt(friendId)), 
					          new IdTimeStructure(hostId, time, fofNumber, bytes));
			}			
			context.getCounter("FRTMapper", "USER_COUNT").increment(1);
			if ((++MapperCount % RECORDNUM) == 0) {
				context.setStatus("Finished processing " + MapperCount	+ " mapper records");
			}
		}
	}

	public static class ExReducer extends Reducer<IntWritable, IdTimeStructure, IntWritable, IdTimeStructureList> {
		private long ReducerCount = 0;

		@Override	
		protected void reduce(IntWritable key, Iterable<IdTimeStructure> values, Context context)
				              throws IOException, InterruptedException {
		  
			List<IdTimeStructure> idTimeStructureList = new ArrayList<IdTimeStructure>();
			for (IdTimeStructure item : values)
				idTimeStructureList.add(new IdTimeStructure(item));

			context.write(key, new IdTimeStructureList(idTimeStructureList, idTimeStructureList.size()));

			context.getCounter("FRTReducer", "USER_COUNT").increment(1);
			if ((++ReducerCount % RECORDNUM) == 0)
				context.setStatus("Finished processing " + ReducerCount + " reducer records");
		}
	}
	
	public final String input;
	public final String output;
	public final Configuration conf;

	@SuppressWarnings("deprecation")
	public int doIt() throws IOException, InterruptedException, ClassNotFoundException, URISyntaxException {
		System.out.println("[ExtendFriendsRelation] doIt");
		
		Path p = new Path(this.output);
		FileSystem fs = FileSystem.get(new URI(this.output), conf);
		if (fs.exists(p) && fs.isDirectory(p))
			fs.delete(p);
		
		Job job = new Job(this.conf);
		
		job.getConfiguration().set("INPUT_RTABLE", this.input);
		job.getConfiguration().set("output", this.output);
		
		//JobUtils.InitJobOptimizations(job, "");
		
		job.setJarByClass(ExtendHBaseToSeq.class);
		job.setMapperClass(ExMapper.class);
	    job.setReducerClass(ExReducer.class);
	    job.setNumReduceTasks(partitionNum);
	    
	    Scan scan = new Scan();
	    TableMapReduceUtil.initTableMapperJob(this.input, scan, ExMapper.class, IntWritable.class, IdTimeStructure.class, job);
	    TableMapReduceUtil.setScannerCaching(job, 20000);
		
	    job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(IdTimeStructureList.class);
		FileOutputFormat.setOutputPath(job, new Path(this.output));
		job.setOutputFormatClass(SequenceFileOutputFormat.class);	


		int result = job.waitForCompletion(true) ? 0 : 1;
		return result;
	}

	private ExtendHBaseToSeq(String input, String output,
			Configuration conf) {
		this.input = input;
		this.output = output;
		this.conf = conf;
	}

	@SuppressWarnings("static-access")
	private static ExtendHBaseToSeq parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table").withLongOpt("in").create('i'));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Output diretory").withLongOpt("out")
				.create('o'));
	    opts.addOption(OptionBuilder.hasArg(true).isRequired(false)
	            .withDescription("Config file").withLongOpt("conf")
	            .create('f'));

		Configuration conf = new Configuration();
		conf = HBaseConfiguration.addHbaseResources(conf);

		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();

		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String input = cmd.getOptionValue('i');
		String output = cmd.getOptionValue('o');
        if (cmd.hasOption('f')) {
            String confname = cmd.getOptionValue('f');
            Path p = new Path(confname);
            System.out.println("Config file is: " + p.toString());
            conf.addResource(p);
        }
		return new ExtendHBaseToSeq(input, output, conf);
	}

	public static void main(String[] args) throws Exception {
		ExtendHBaseToSeq efr = parseArgs(args);
		efr.doIt();
	}
}
