package com.renren.renrengame.utils.hot;
import java.util.ArrayList;
import com.google.gson.Gson;

public class GameJsonGenerator {

	public String getRecommendedGamesJson(ArrayList<PlatformGameItemInfo> gameItemInfoList)
	{
		String json = "";
		if (gameItemInfoList.size() != 0)
			json = new Gson().toJson(gameItemInfoList);
		return json;
	}
}
