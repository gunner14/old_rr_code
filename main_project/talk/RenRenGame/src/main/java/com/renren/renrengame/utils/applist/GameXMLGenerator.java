package com.renren.renrengame.utils.applist;

import java.util.ArrayList;

import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import com.renren.renrengame.utils.RRGameConstants;

public class GameXMLGenerator {

	private void gameItemXML(Element subtype ,ArrayList<GameItemInfo> gameItemsList, String gameType)
	{			
			
		for (int i=0; i<gameItemsList.size(); i++)
		{
			if (gameItemsList.get(i).subtype.equals(gameType))
			{										
				Element appid = subtype.addElement("app");
				if (gameType.equalsIgnoreCase(""))
					appid.addAttribute("id", gameItemsList.get(i).ID);
				else
					appid.addElement("id").setText(gameItemsList.get(i).ID);
				appid.addElement("name").setText(gameItemsList.get(i).name);
				appid.addElement("logo").setText(gameItemsList.get(i).logo);
				appid.addElement("logo75").setText(gameItemsList.get(i).logo75);
				appid.addElement("url").setText(gameItemsList.get(i).url);
			}
			
		}

	}
	
	
	private void GameXML(Element type ,ArrayList<GameItemInfo> gameItemsList, ArrayList<String> gameTypeList)
	{
//		Document document = DocumentHelper.createDocument();
//		Element type = document.addElement("type").addAttribute("name", RRGameConstants.SOCIALGAME_TITLE);			
		for (int i=0; i<gameTypeList.size(); i++)
		{
			Element subtype = type.addElement("type").addAttribute("name", gameTypeList.get(i));
			gameItemXML(subtype,gameItemsList, gameTypeList.get(i));
		}			 		
//		return document.asXML();
	}
	
	public String recentGameXML(int uid, ArrayList<GameItemInfo> recentGamesList) {
		Document document = DocumentHelper.createDocument();
		Element gamelist = document.addElement("applist").addAttribute("host", String.valueOf(uid));
		gameItemXML(gamelist, recentGamesList, "");
		return document.asXML();
	}
	
	public String gameXML(ArrayList<GameItemInfo> recommendedGamesList,ArrayList<GameItemInfo> socialGameList, 
			ArrayList<String> socialGameTypeList,
			ArrayList<GameItemInfo> webGameList, ArrayList<String> webGameTypeList)
	{
		Document document = DocumentHelper.createDocument();
		Element gamelist = document.addElement("gamelist");	
		
		if (recommendedGamesList.size() > 0)
		{
			Element socialGameType = gamelist.addElement("type").addAttribute("name", RRGameConstants.RECOMMENDEDGAMES_TITLE);
			gameItemXML(socialGameType ,recommendedGamesList, RRGameConstants.RECOMMENDEDGAMES_TITLE);
		}
		
		if (socialGameList.size()>0 && socialGameTypeList.size()>0)
		{
			Element socialGameType = gamelist.addElement("type").addAttribute("name", RRGameConstants.SOCIALGAME_TITLE);
			GameXML(socialGameType, socialGameList, socialGameTypeList);
		}
		
		if (webGameList.size()>0 && webGameTypeList.size()>0)
		{
			Element webGameType = gamelist.addElement("type").addAttribute("name", RRGameConstants.WEBGAME_TITLE);
			GameXML(webGameType, webGameList, webGameTypeList);
		}
		
		return document.asXML();
	}
}



/******
*  xml 格式
*  
*  <type name="社区游戏">
*    <subtype name="小游戏">
*      <app>
*        <id>108245</id>
*        <name>小小战争</name>
*        <logo>http://app.xnimg.cn/application/20100820/18/25/LxYRP39g018153.gif</logo>
*        <logo75>http://app.xnimg.cn/application/logo/20110617/16/15/LI6jU57o018153.gif</logo75>
*        <url>http://apps.renren.com/littlewar</url>
*      </app>
*      ......
*    </subtype>
*    ......
*  <type>  
*
*/