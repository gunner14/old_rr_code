/********************************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:      junwu.xiong@renren-inc.com
 * Function:   Importa original data from hdfs file that was dumped from mysql
 * Date:  2012-07-02
*********************************************************************************************************************/


package com.renren.datamining.friendsoffriends;

import java.io.IOException;
import java.util.Map.Entry;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.NullOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class ImportRelationFile {
  public static class ImportRelationFileMap extends
      Mapper<LongWritable, Text, NullWritable, NullWritable> {
    @Override
    protected void setup(
        Mapper<LongWritable, Text, NullWritable, NullWritable>.Context context)
        throws IOException, InterruptedException {
      this.table = new HTable(context.getConfiguration(), context.getConfiguration().get("OUTPUT_RTABLE"));
    };

    @Override
    protected void cleanup(
        Mapper<LongWritable, Text, NullWritable, NullWritable>.Context context)
        throws IOException, InterruptedException {
      this.table.flushCommits();
      this.table.close();
    };

    private HTable table;
    private int priorUserId = 0;
    private FriendListWithTime lists = new FriendListWithTime();

    @Override
    protected void map(LongWritable key, Text value,
        Mapper<LongWritable, Text, NullWritable, NullWritable>.Context context)
        throws IOException, InterruptedException {

        try {
            String line = value.toString();	        
            FriendWithTime ft = FriendWithTime.parse(line);
            if (ft.getUserId() != priorUserId) {               	
            	if (priorUserId != 0) {
	              Put put = new Put(Bytes.toBytes(priorUserId));
	              for (Entry<Integer, Integer> entry : lists.data.entrySet()) {
	                put.add(Bytes.toBytes("g"), Bytes.toBytes(entry.getKey()),
	                    Bytes.toBytes(entry.getValue()));
	              }
	              if(!put.isEmpty()) table.put(put);
              }
              context.getCounter("ImportRelationFile", "USER_COUNT").increment(1);
              context.getCounter("ImportRelationFile", "FRIEND_COUNT").increment(lists.data.size());
              lists = new FriendListWithTime(); 
          	  lists.data.put(ft.getFriendId(), ft.getTime());
              priorUserId = ft.getUserId();
            } else {     
              lists.data.put(ft.getFriendId(), ft.getTime());
            }      
      } catch (Exception e) {
        e.printStackTrace();
      }
    };
  }

  private final String inPath;
  private final String outTable;
  private final Configuration conf;

  private ImportRelationFile(String inPath, String outTable, Configuration conf) {
    this.inPath = inPath;
    this.outTable = outTable;
    this.conf = conf;
  }

  public void doIt() throws Exception {
    Job job = new Job(this.conf);
    job.setJarByClass(ImportRelationFile.class);
    job.setMapperClass(ImportRelationFileMap.class);
    job.setNumReduceTasks(0);
    FileInputFormat.addInputPath(job, new Path(this.inPath));
    job.setOutputFormatClass(NullOutputFormat.class);
    job.getConfiguration().set("OUTPUT_RTABLE", this.outTable);
    job.submit();
  }

  @SuppressWarnings("static-access")
  private static ImportRelationFile parseArgs(String[] args)
      throws ParseException, IOException {
    Options opts = new Options();
    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
        .withDescription("Input file path in HDFS").withLongOpt("in")
        .create('i'));
    opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
        .withDescription("Output table name in HBASE").withLongOpt("out")
        .create('o'));

    Configuration conf = new Configuration();
    conf = HBaseConfiguration.addHbaseResources(conf);
    String[] otherArgs = new GenericOptionsParser(conf, args)
        .getRemainingArgs();



    PosixParser parser = new PosixParser();
    CommandLine cmd = parser.parse(opts, otherArgs);
    String input = cmd.getOptionValue('i');
    String output = cmd.getOptionValue('o');
    return new ImportRelationFile(input, output, conf);
  }

  public static void main(String[] args) throws Exception {
    ImportRelationFile irf = parseArgs(args);
    irf.doIt();
  }

}


