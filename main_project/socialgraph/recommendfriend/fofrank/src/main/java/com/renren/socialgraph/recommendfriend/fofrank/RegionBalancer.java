
package com.renren.socialgraph.recommendfriend.fofrank;

import java.io.IOException;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.PosixParser;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.hadoop.hbase.HRegionInfo;
import org.apache.hadoop.hbase.ServerName;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.hbase.HBaseConfiguration;

public class RegionBalancer {	

	public String[] tableName;
	public double[] prop;
	HBaseAdmin hBaseAdmin;
	public Configuration config;
	
	public static class ServerInfo {
		ServerName server;
		Map<String, List<HRegionInfo>> regions;			// Current region allocations of the server
		Map<String, List<HRegionInfo>> regionsBuffer;	// Buffers for temporary movement
		Map<String, Integer> regionCount;				// Region count of specified table, just for convenience
		
		public ServerInfo(ServerName s) {
			server = s;
			regions = new HashMap<String, List<HRegionInfo>>();
			regionsBuffer = new HashMap<String, List<HRegionInfo>>();
			regionCount = new HashMap<String, Integer>();
		}
		
		/** Get the region count of specified table */
		public int getRegionCount(String tableName) {
			return regionCount.containsKey(tableName) ? regionCount.get(tableName) : 0;
		}
		
		/** 
		 * Add region info to the server
		 * 
		 * @param regionInfo region to add
		 * @param isOriginal whether the region is added initially or by balancer.  
		 */
		public void addRegion(HRegionInfo regionInfo, boolean isOriginal) {
			Map<String, List<HRegionInfo>> queue = isOriginal ? regions : regionsBuffer;
			String tableName = regionInfo.getTableNameAsString();
			List<HRegionInfo> li = queue.get(tableName);
			if (li == null) {
				li = new Vector<HRegionInfo>();
				queue.put(tableName, li);
			}
			li.add(regionInfo);
			
			Integer x = regionCount.get(tableName);
			if (x == null)
				regionCount.put(tableName, 1);
			else
				regionCount.put(tableName, x + 1);
		}
		
		/**
		 * Remove a region of the specified table
		 * We prefer a buffered region to the original region to prevent additional movement.
		 * 
		 * @param tableName
		 * @return the region that removed from the server
		 */
		public HRegionInfo removeRegion(String tableName) {
			List<HRegionInfo> queue = null;
			Map<String, List<HRegionInfo>> src = null;
			
			// Determine the source queue of the remove.
			if (regionsBuffer.containsKey(tableName) && regionsBuffer.get(tableName).size() > 0) {
				queue = regionsBuffer.get(tableName);
				src = regionsBuffer;
			}
			else if (regions.containsKey(tableName) && regions.get(tableName).size() > 0){
				queue = regions.get(tableName);
				src = regions;
			}
			if (queue == null)
				return null;
			
			// Randomly remove a region and return its info
			HRegionInfo ret = queue.remove((int)Math.floor(Math.random() * queue.size()));
			if (queue.size() == 0)
				src.remove(tableName);

			// Update region count info
			Integer x = regionCount.get(tableName);
			if (x > 1)
				regionCount.put(tableName, x - 1);
			else
				regionCount.remove(tableName);
			
			return ret;
		}
		
		/**
		 * Check whether we have regions of other tables on this server
		 * 
		 * @param tableName
		 * @return nonzero if have regions of other tables
		 */
		public int hasOtherTable(String tableName) {
			for (String t : regionCount.keySet()) {
				if (!t.equals(tableName) && regionCount.get(t) > 0)
					return 1;
			}
			return 0;
		}
		
		/**
		 * Check whether we have the regions of the given table on this server
		 * 
		 * @param tableName
		 * @return true if they exists
		 */
		public boolean hasTable(String tableName) {
			return regionCount.containsKey(tableName) && (regionCount.get(tableName) > 0);
		}
	}
	
	// All available servers of the cluster, dead server excluded.
	public HashMap<String, ServerInfo> allServers = new HashMap<String, ServerInfo>();
	
	private RegionBalancer(String[] table, String[] proportion, Configuration conf) throws Exception {
		hBaseAdmin = new HBaseAdmin(conf);
		tableName = table;
		config = conf;
		
		// Calculate the proportions of servers of each table.
		// If the proportion is not specified,
		// We divide them equally.
		prop = new double[table.length];
		if (proportion != null) {
			double sum = 0;
			for (int i = 0; i < proportion.length; ++i)
				sum += Double.parseDouble(proportion[i]);
			for (int i = 0; i < proportion.length; ++i)
				prop[i] = Double.parseDouble(proportion[i]) / sum;
		}
		else {    
			for (int i = 0; i < table.length; ++i)
				prop[i] = 1.0 / (double)(table.length);			
		}
	}
	
	/** 
	 * Balance the specified table among given number of servers
	 * 
	 * @param tableName the table that should be balanced
	 * @param serverCnt number of server the table used
	 * @throws IOException
	 */
	public void balanceTable(final String tableName, int serverCnt) throws IOException {
		// Get current region locations of the table.
		HTable table = new HTable(config, Bytes.toBytes(tableName));
		Map<HRegionInfo, ServerName> regionsMap = table.getRegionLocations();
		for (HRegionInfo hi : regionsMap.keySet()) {
			ServerName s = regionsMap.get(hi);
			allServers.get(s.getServerName()).addRegion(hi, true);
		}
		
		// Select all the server as candidates.
		Vector<ServerInfo> candidate = new Vector<ServerInfo>();
		for (String s : allServers.keySet()) {
			ServerInfo info = allServers.get(s);
			candidate.add(info);
		}
		
		// Sort to select top serverCount servers as host servers
		//
		// First, select the server that doesn't has other tables,
		// if they are not enough, select the server that has more 
		// regions to minimize move operations.
		Collections.sort(candidate, new Comparator<ServerInfo>() {
			public int compare(ServerInfo o1, ServerInfo o2) {
				int hasOther1 = o1.hasOtherTable(tableName);
				int hasOther2 = o2.hasOtherTable(tableName);
				if (hasOther1 != hasOther2)
					return hasOther1 - hasOther2;
				else
					return o2.getRegionCount(tableName) - o1.getRegionCount(tableName);
			}
		});
		for (int i = candidate.size() - 1; i >= serverCnt; i--)
			candidate.remove(i);
		
		System.out.println("We have " + candidate.size() + " candidate servers");
		for (ServerInfo s : candidate) {
			System.out.print(s.server.getServerName() + " ");
		}
		System.out.println();
		
		// Move regions from non-candidate servers to host servers
		for (String s : allServers.keySet()) {
			ServerInfo server = allServers.get(s);
			if (candidate.contains(server))
				continue;
			int count = server.getRegionCount(tableName);
			for (int i = 0; i < count; i++) {
				HRegionInfo hi = server.removeRegion(tableName);
				candidate.get((int) (Math.random() * candidate.size())).addRegion(hi, false);
			}
		}
		
		// Balance the regions among host servers
		while (true) {
			int min = Integer.MAX_VALUE, max = Integer.MIN_VALUE;
			ServerInfo minServer = null, maxServer = null;
			for (ServerInfo s : candidate) {
				int cnt = s.getRegionCount(tableName);
				if (cnt > max) {
					max = cnt;
					maxServer = s;
				}
				if (cnt < min) {
					min = cnt;
					minServer = s;
				}
			}
			if (max - min <= 1)
				break;
			minServer.addRegion(maxServer.removeRegion(tableName), false);
		}
		
	    // Perform the balance operations on HBase
		System.out.println("TABLE:\t" + tableName);
		for (String s : allServers.keySet()) {
			ServerInfo server = allServers.get(s);
			System.out.println(server.server.getServerName() + "\t" + server.getRegionCount(tableName));
			List<HRegionInfo> li = server.regionsBuffer.get(tableName);
			if (li == null)
				continue;
			for (HRegionInfo r : li) {
				hBaseAdmin.move(r.getEncodedNameAsBytes(), Bytes.toBytes(server.server.getServerName()));
				System.out.println(r.getEncodedName() + "\t" + server.server.getServerName());
			}
		}
		
	}
	
	public void balance() throws Exception {
	    // Get available servers of the cluster, dead server excluded.
		Collection<ServerName> allServer = hBaseAdmin.getClusterStatus().getServers();
		Collection<ServerName> deadServers = hBaseAdmin.getClusterStatus().getDeadServerNames();
		for (ServerName s : allServer) {
			if (!deadServers.contains(s))
				allServers.put(s.getServerName(), new ServerInfo(s));
		}
		
		// For each table, calculate the number of server it should be
		// distributed on with given proportion.
		for (int i = 0; i < tableName.length; ++i) {
			int serverCount = (int) Math.max(allServers.size() * prop[i], 1);
			
			// If it's the last table, we use all non-used servers.
			if (i == tableName.length - 1) {
				int maxCount = 0;
				for (String s : allServers.keySet()) {
					ServerInfo t = allServers.get(s);
					if (t.hasOtherTable(tableName[i]) == 0)
						maxCount++;
				}
				serverCount = Math.max(maxCount, serverCount);
			}
			
			// Check whether the table exists and begin to balance.
			if (!hBaseAdmin.tableExists(tableName[i])) {
				System.out.println("No such table: " + tableName[i]);
				return;
			}
			System.out.println("Balancing table with " + serverCount + " servers");
			balanceTable(tableName[i], serverCount);
		}
	}
	
	@SuppressWarnings("static-access")
	private static RegionBalancer parseArgs(String[] args) throws Exception {
		Options opts = new Options();
		opts.addOption(OptionBuilder.hasArgs().isRequired(true)
				.withDescription("Table names").withLongOpt("table")
				.create("t"));
		opts.addOption(OptionBuilder.hasArgs().isRequired(false)
				.withDescription("Proportion of region servers with each table").withLongOpt("proportion")
				.create("p"));
		
		Configuration conf = HBaseConfiguration.create();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		
		PosixParser parser = new PosixParser();
		CommandLine cmd = parser.parse(opts, otherArgs);
		
		String [] tableName = cmd.getOptionValues("t");
		String [] proportion = null;
		if (cmd.hasOption("p")) {
			proportion = cmd.getOptionValues("p");
			if (proportion.length != tableName.length) {
				System.out.println("The length of proportion and tablename must match");
				return null;
			}
		}
		return new RegionBalancer(tableName, proportion, conf);
	}
	
	public static void main(String[] args) throws Exception{
		RegionBalancer rff = parseArgs(args);
		rff.balance();
	}
}

