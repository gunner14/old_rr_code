package com.renren.renrengame.utils.config;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;

import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;
import com.renren.renrengame.utils.buddy.PlayerListReader;

public class ConfigFileReader {
	private final Logger log = Logger.getLogger(PlayerListReader.class.getName());

	private ArrayList<String> getConfigsFromFile()
	{
		long   t1   =   System.currentTimeMillis(); 
		log.debug("ConfigFileReader.getPlayerInfoFromFile ----> start to run");
		ArrayList<String> v = new ArrayList<String>();
				
		URL resourceURL = this.getClass().getClassLoader().getResource(RRGameConstants.CONFIG_FILENAME);
		if (resourceURL == null)
		{
			log.error("ConfigFileReader.getPlayerInfoFromFile ---->Cannot find gamelist." );
			return v;
		}
		String gameListPath = resourceURL.getPath();
		log.debug("ConfigFileReader.getPlayerInfoFromFile ----> gameListPath"+gameListPath);
				
		try 
		{								
		    InputStream inStream = new FileInputStream(gameListPath); //读入原文件  
		    int byteread = 0;			
		    int headpos =0 ;
		    int rearpos = 0;
		    byte[] buffer = new byte[RRGameConstants.BUFFER_SIZE];  
		    int offset =0; 
		    while ( (byteread = inStream.read(buffer, offset, RRGameConstants.BUFFER_SIZE-offset)) != -1) 
		    {  
//				log.debug("ConfigFileReader.getPlayerInfoFromFile ----> byteread"+byteread);

		    	headpos = 0;
		    	rearpos = offset;
		    	while (rearpos < byteread+offset)
		    	{
		    		if (buffer[rearpos] == '\n')
		    		{
//						log.debug("PlayerListReader.getPlayerInfoFromFile ----> push_back"+
//								new String(buffer,headpos, rearpos-headpos));
		    			v.add( new String(buffer,headpos, rearpos-headpos));
		    			headpos = rearpos+1;
		    		}
		    		
		    		rearpos++;
		    	}
		    	if (rearpos == byteread+offset)
		    	{	    	
//					log.debug("PlayerListReader.getPlayerInfoFromFile ----> can't found!! p:"+buffer[rearpos-1]);

		    		int tempPos = 0;
		    		while (headpos < byteread+offset )
		    			buffer[tempPos++] = buffer[headpos++];		  
		    		offset = tempPos;
		    	} 		    	
		    }  
		    
		    inStream.close(); 
	
			log.debug("PlayerListReader.getPlayerInfoFromFile ----> v.size"+v.size());

		} 
		catch (Exception e)
		{
			log.warn("ConfigFileReader.getPlayerInfoFromFile---> Get data from file..");
			e.printStackTrace();
		}
		
		long   t2   =   System.currentTimeMillis(); 
		log.info("ConfigFileReader.getPlayerInfoFromFile---> time cost: "+ (t2-t1));
		
		return v;
	}
	
	
	private ArrayList<ConfigInfo> getConfigInfoFromList( ArrayList<String> configInfoList)
	{
		ArrayList<ConfigInfo> ciList = new ArrayList<ConfigInfo>();
	
		for (int i=0; i<configInfoList.size(); ++i)
		{
			ConfigInfo ci = new ConfigInfo();
			
			String item = configInfoList.get(i);
			int pos = item.indexOf('=');
			if (pos == -1)
				continue;
			ci.name = item.substring(0, pos);
			ci.value = item.substring(pos+1);
			
			ciList.add(ci);
		}
		
		return ciList;
	}
	
	public ArrayList<ConfigInfo>  getConfigInfo()
	{
		return getConfigInfoFromList(getConfigsFromFile());
	}
}
