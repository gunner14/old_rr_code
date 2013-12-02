package com.renren.renrengame.utils.buddy;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;

import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;

public class PlayerListReader {
	private final Logger log = Logger.getLogger(PlayerListReader.class.getName());

	private ArrayList<String> getPlayerInfoFromFile()
	{
		long   t1   =   System.currentTimeMillis(); 
		log.debug("PlayerListReader.getPlayerInfoFromFile ----> start to run");
		ArrayList<String> v = new ArrayList<String>();
				
		URL resourceURL = this.getClass().getClassLoader().getResource(RRGameConstants.PLAYERLIST_FILENAME);
		if (resourceURL == null)
		{
			log.error("PlayerListReader.getPlayerInfoFromFile ---->Cannot find gamelist." );
			return v;
		}
		String gameListPath = resourceURL.getPath();
		log.debug("PlayerListReader.getPlayerInfoFromFile ----> gameListPath"+gameListPath);
				
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
		    	headpos = 0;
		    	rearpos = offset;
		    	while (rearpos < byteread+offset)
		    	{
		    		if (buffer[rearpos] == '\n')
		    		{
		    			v.add( new String(buffer,headpos, rearpos-headpos));
		    			headpos = rearpos+1;
		    		}
		    		
		    		rearpos++;
		    	}
		    	if (rearpos == byteread+offset)
		    	{		    	
//		    		if (byteread < RRGameConstants.BUFFER_SIZE-offset)
//		    		{
//		    			v.add( new String(buffer, headpos, rearpos-headpos));
//		    		}
//		    		else
//		    		{
			    		int tempPos = 0;
			    		while (headpos < byteread+offset )
			    			buffer[tempPos++] = buffer[headpos++];		  
			    		offset = tempPos;
//		    		}
		    	} 		
		    	
		    	
		    }  
		    
		    inStream.close(); 		    
		    			    		    		
		} 
		catch (Exception e)
		{
			log.warn("PlayerListReader.getPlayerInfoFromFile---> Get data from file..");
			e.printStackTrace();
		}
		
		long   t2   =   System.currentTimeMillis(); 
		log.info("PlayerListReader.getPlayerInfoFromFile---> time cost: "+ (t2-t1));
		
		return v;
	}
	
	private PlayerListInfo getPlayerInfoFromList( ArrayList<String> playerInfoList)
	{
		long   t1   =   System.currentTimeMillis(); 
		
		PlayerListInfo pli = new PlayerListInfo();
		pli.gameTitleList = new ArrayList<String>();
		pli.playerList = new ArrayList<ArrayList<String>>();
		
		log.debug("PlayerListReader.getPlayerInfoFromList---> playerInfoList.size:"+playerInfoList.size());		
		
		for (int i=0; i<playerInfoList.size(); ++i)
		{
			String playerInfo = playerInfoList.get(i);
 			int pos = playerInfo.indexOf(' ');
			if (pos == -1)
				pos = playerInfo.indexOf('\t');
			if (pos == -1)
				return pli;
			
			String gameTitle = playerInfo.substring(0, pos).trim();
			String playerId = playerInfo.substring(pos+1).trim();
			
 			int j = 0;
			for (; j<pli.gameTitleList.size(); ++j)
			{
				if (gameTitle.equals(pli.gameTitleList.get(j)))
				{
					pli.playerList.get(j).add(playerId);
					break;
				}
			}
			if (j == pli.gameTitleList.size())
			{
 				pli.gameTitleList.add(gameTitle);
				
				ArrayList<String> playerSublist = new ArrayList<String>();
				playerSublist.add(playerId);
				pli.playerList.add(playerSublist);
			}
			
		}
		log.debug("PlayerListReader.getPlayerInfoFromList---> pli.gameTitleList.size:"+pli.gameTitleList.size());
		log.debug("PlayerListReader.getPlayerInfoFromList---> pli.playerList.size:"+pli.playerList.size());
//		for (int i=0; i<pli.gameTitleList.size(); ++i)
//		{
//			log.debug("PlayerListReader.getPlayerInfoFromList---> pli.gameTitleList_"+i+":"
//					+pli.gameTitleList.get(i)+"  pli.playerList.size:"+pli.playerList.get(i).size());
//		}
		
		long   t2   =   System.currentTimeMillis(); 
		log.info("PlayerListReader.getPlayerInfoFromList---> time cost: "+ (t2-t1));
		return pli;
		
	}
	
	public PlayerListInfo getPlayerIDList()
	{
		return getPlayerInfoFromList(getPlayerInfoFromFile());
	}
}
