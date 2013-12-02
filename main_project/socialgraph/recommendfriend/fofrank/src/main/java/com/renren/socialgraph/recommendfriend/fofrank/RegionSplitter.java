
package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.IOException;
import java.net.URI;
import java.util.List;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.hadoop.hbase.HRegionInfo;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.fs.FileStatus;

public class RegionSplitter {	

	public String tableName;
	public int maxsize;
	public String root;
	public Configuration conf; 

	public static class MyRegion {
		public String key;
		public String completeKey;
		public int size;
		public boolean isSplitting;
	}
	
	private RegionSplitter(String tname, String hbaseroot, int size, Configuration conf) {
		this.conf = conf;
		this.tableName = tname;
		this.maxsize = size;
		this.root = hbaseroot;
	}

	int dus(String src) throws IOException {
		Path srcPath = new Path(src);
		FileSystem srcFs = srcPath.getFileSystem(conf);
    	FileStatus status[] = srcFs.globStatus(new Path(src));
    	if (status == null || status.length == 0)
    	  return -1;
      	long totalSize = srcFs.getContentSummary(status[0].getPath()).getLength();
      	return (int)(totalSize / 1024 / 1024);
  	}
	
	public void doIt() throws Exception {
		HBaseAdmin hBaseAdmin = new HBaseAdmin(conf);
		if (!hBaseAdmin.tableExists(tableName)) {
			System.out.println("No such table: " + tableName);
			return;
		}
		
		boolean isSplitting = false;
		boolean hasZeroSizeRegion = false;
		do 
		{
			isSplitting = false;
			hasZeroSizeRegion = false;
			
			List<HRegionInfo> list = hBaseAdmin.getTableRegions(Bytes.toBytes(tableName));
			System.out.println("We got " + list.size() + " regions");
			for (HRegionInfo r : list) {
				String rname = r.getRegionNameAsString();
				String[] tmp = rname.split("\\.") ;
				String key = tmp[tmp.length - 1];
				System.out.print("Checking region: " + rname + "\t");
				if (r.isSplitParent()) {
					System.out.println("Split parent skipped");
					isSplitting = true;
					continue;
				}

				if (r.isSplit()) {
					System.out.println("Split skipped");
					isSplitting = true;
					continue;
				}
			
				String rpath = this.root + "/" + tableName + "/" + key;
				Path p = new Path(rpath);
				FileSystem fs = FileSystem.get(new URI(rpath), this.conf);
				if (!fs.exists(p)) {
					System.out.println("Region dir not exists");
					isSplitting = true;
					continue;
				}
				
				Path ps = p.suffix("/.splits");
				//System.out.println("Checking split flag: " + ps.toString());
				if (fs.exists(ps)) {
					System.out.println("Region is splitting");
					isSplitting = true;
					continue;
				}
			
				int rsize = dus(rpath);
				System.out.println(rsize + "MB");
				if (rsize == 0)
					hasZeroSizeRegion = true;
				if (rsize > this.maxsize) {
					System.out.println("Need split: \t" + rname + "\t" + rsize + "MB");
					hBaseAdmin.split(rname);
					isSplitting = true;
				}
			}
			if (isSplitting || hasZeroSizeRegion) {
				System.out.println("Waiting for 10 secs");
				Thread.sleep(10000);
			}
		}
		while (isSplitting);
	}
	
	@SuppressWarnings("static-access")
	private static RegionSplitter parseArgs(String[] args) throws IOException, ParseException {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("Input table name in HBase").withLongOpt("table")
				.create("t"));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("HBase root dir").withLongOpt("root")
				.create("r"));
		opts.addOption(OptionBuilder.hasArg(true).isRequired(true)
				.withDescription("MaxSize").withLongOpt("size")
				.create("s"));
		Configuration conf = new Configuration();
		conf = HBaseConfiguration.addHbaseResources(conf);
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		String t = cmd.getOptionValue('t');
		String r = cmd.getOptionValue('r');
		int s = Integer.parseInt(cmd.getOptionValue('s'));
		return new RegionSplitter(t, r, s, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RegionSplitter rff = parseArgs(args);
		rff.doIt();
	}
	
}

