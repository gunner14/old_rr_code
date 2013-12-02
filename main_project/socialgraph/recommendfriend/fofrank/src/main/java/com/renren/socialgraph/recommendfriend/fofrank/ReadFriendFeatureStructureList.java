package com.renren.socialgraph.recommendfriend.fofrank;


import java.io.IOException;
import java.net.URI;

import org.apache.commons.io.IOUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.util.ReflectionUtils;

 
public class ReadFriendFeatureStructureList {
	private static org.apache.commons.logging.Log logger = org.apache.commons.logging.LogFactory
			.getLog(RcdFoFSeqToHBase.class);	
	
	public static void main(String[] args) throws IOException {
		String uri = args[0];  
	    Configuration conf = new Configuration();  
	    FileSystem fs = FileSystem.get(URI.create(uri), conf);  
//	    FileSystem fs = FileSystem.get(conf);
	    Path path = new Path(uri);  
	    SequenceFile.Reader reader = null;  
	    try {  
	        reader = new SequenceFile.Reader(fs, path, conf);  
	        IntWritable key = (IntWritable)  
	            ReflectionUtils.newInstance(reader.getKeyClass(), conf);  
	        FriendFeatureStructureList value = (FriendFeatureStructureList)  
	            ReflectionUtils.newInstance(reader.getValueClass(), conf);  
//	        long position = reader.getPosition();  
	        int i = 0;
	        while (reader.next(key, value) && (++i < 10)) {  
				logger.info("[getFriendRankValue]: hostId " + key.get() + " friendNumber "
						+ value.getFriendNumber());
				for (FriendFeatureStructure item : value.getFriendFeatureStructureList()) {
					int friendsNumOfFriend = item.getFriendFriendNumber();
					logger.info("Friend Id " + Bytes.toInt(item.getFriendId()) + 
							" time " + Bytes.toInt(item.getTime()) + 
							" friend number " + friendsNumOfFriend + 
							" static weight " + Bytes.toFloat(item.getStaticWeight()) + 
							" active weight " + Bytes.toFloat(item.getActiveWeight()));
					byte[] mutualFriendsIdBytes = item.getBytes(); 
					for (int count = 0; count < friendsNumOfFriend; ++count) {
						int fofId = Bytes.toInt(mutualFriendsIdBytes, count * Bytes.SIZEOF_INT, Bytes.SIZEOF_INT);
						logger.info("fofId " + fofId);
					}
				}        	
//	            String syncSeen = reader.syncSeen() ? "*" : "";  	            
//	            System.out.printf("[%s%s]\t%s\t%s\n", position, syncSeen, key, value);  
//	            position = reader.getPosition(); // beginning of next record  	            
	        }  
	    } finally {  
	        IOUtils.closeQuietly(reader);  
	    }  
	}
   
}  

