package com.renren.renrengame.utils.buddy;

import java.util.ArrayList;
import java.util.Random;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import com.renren.renrengame.utils.RRGameConstants;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

public class PlayerXMLGenerator {
	private final Logger log = Logger.getLogger(PlayerXMLGenerator.class.getName());
	public ArrayList<String> getPlayerXML(PlayerListInfo pli, String gameTitle, String serverTitle, String number)
	{
		ArrayList<String> playerList = new ArrayList<String>();
		
		if (gameTitle==null || pli==null || pli.gameTitleList==null || pli.playerList==null)
			return playerList;
		
		if (number==null || number.equals(""))
			number = RRGameConstants.PLAYER_SIZE;
		
		boolean continueRead = true;
		boolean flag = true;
		String itemTitle = "";
		
		if (serverTitle==null || serverTitle.equals("") || serverTitle.equalsIgnoreCase("all"))
			itemTitle = gameTitle;
		else
			itemTitle = serverTitle + "$" + gameTitle;	
		
		if (serverTitle==null || serverTitle.equals(""))
			continueRead = false;
		else if (serverTitle.equalsIgnoreCase("all"))
			continueRead = true;
	 						
//		log.debug("PlayerXMLGenerator.getPlayerXML  -----> itemTitle:"+itemTitle+"    continueRead:"+continueRead);
//		log.debug("PlayerXMLGenerator.getPlayerXML  -----> pli.gameTitleList.size():"+pli.gameTitleList.size());
		int itemNum = Integer.valueOf(number).intValue();
		for (int i = 0; i<pli.gameTitleList.size(); ++i)
		{	

			boolean isRead = false;
			String tempGameTitle = pli.gameTitleList.get(i);
			if (tempGameTitle.equals(itemTitle))
				isRead = true;
			else if (continueRead)
			{	
				int pos = tempGameTitle.indexOf('$', 0);	
				if (pos > -1)
				{
					String tempGameName = tempGameTitle.substring(pos +1);
					if (tempGameName.equalsIgnoreCase(itemTitle))
						isRead = true;
				}				
			}
//			log.debug("PlayerXMLGenerator.getPlayerXML  -----> i:"+i+"   gameTitle:"+tempGameTitle
//					+"    isRead:"+isRead+"    continueRead:"+continueRead);

			if (isRead)
			{
				
				int playerNum = pli.playerList.get(i).size();
//				log.debug("PlayerXMLGenerator.getPlayerXML  -----> i:"+i
//						+"   itemNum:"+itemNum+"    playerNum:"+playerNum);
				if (playerNum > itemNum)
				{
					ArrayList<Integer> rdmPosList = getRandomPos(itemNum, playerNum );

					for (int j=0; j<rdmPosList.size(); ++j)
						playerList.add(pli.playerList.get(i).get( rdmPosList.get(j) ));

				}
				else
				{
					for (int j=0; j<playerNum; ++j)					
						playerList.add(pli.playerList.get(i).get(j));

				}
				
				if (!continueRead)
					flag = false;
			}
 						
			if (!flag)
				break;			 			
		}
 	
		if (playerList.size() > itemNum)
		{
			ArrayList<Integer> rdmPosList = getRandomPos(itemNum, playerList.size() );
			ArrayList<String> allPlayerList = new ArrayList<String>();
			
			for (int j=0; j<rdmPosList.size(); ++j)			
				allPlayerList.add(playerList.get( rdmPosList.get(j) ));		
			
			return allPlayerList;
		}
		
		return playerList;
		//return getItemXML(playerList, gameTitle, serverTitle);
	}
	
	public ArrayList<PlayerInfo> getPlayerInfoList(ArrayList<String> playerIDList)
	{
		ArrayList<PlayerInfo> playerInfoList = new ArrayList<PlayerInfo>();		
		WUserAdapter wa  = WUserAdapter.getInstance();	
		
		for (int i=0; i<playerIDList.size(); ++i)
		{
			PlayerInfo pi = new PlayerInfo();
			pi.id = playerIDList.get(i);
			int uid = Integer.parseInt(pi.id);
			if (uid > 0)
			{
				User u = wa.get(uid);
				pi.name = u.getName();
				pi.headUrl = u.getHeadFullUrl();
				pi.tinuUrl = u.getTinyFullUrl();
			}
			
			playerInfoList.add(pi);
		}
		
		return playerInfoList;
	}
	
	private ArrayList<Integer> getRandomPos(int number, int total)
	{
		ArrayList<Integer> randomPosList = new ArrayList<Integer>();
		if (number >= total)
			return randomPosList;
		
		
		int flag=0;
		int count = 0;
		int rdmNum = 0;
		while (count < number)
		{
			Random rdm = new Random(System.currentTimeMillis());
			rdmNum = Math.abs(rdm.nextInt()) % total;
            for(int i=0;i < count;i++){
                if(randomPosList.get(i).intValue() == rdmNum){
                    flag = 1;
                    break;
                }else{
                    flag = 0;
                }
            }
            if( flag == 0){           	
            	randomPosList.add(new Integer(rdmNum));    
                count++;
                
            }		
		}		
		return randomPosList;
	}
	
	public String getItemXML(ArrayList<PlayerInfo> playerList, String gameTitle, String serverTitle)
	{
		Document document = DocumentHelper.createDocument();
		Element playerlist = document.addElement("playerlist");
		
		if (gameTitle!=null && !gameTitle.equals(""))
			playerlist.addElement("game").addText(gameTitle);
		if (serverTitle!=null && !serverTitle.equals(""))
			playerlist.addElement("server").addText(serverTitle);
		
		for (int i=0; i<playerList.size(); ++i)
		{
			Element playerinfo = playerlist.addElement("playerinfo");
			playerinfo.addElement("playerid").addText(playerList.get(i).id);
			playerinfo.addElement("nickname").addText(playerList.get(i).name);
			playerinfo.addElement("headurl").addText(playerList.get(i).tinuUrl);
		}
		
		return document.asXML();		
	}
}
