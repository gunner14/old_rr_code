/**************************************************************************************************
 * Developer:  Xiongjunwu
 * Email:  junwu.xiong@renren-inc.com
 * Function:  SocialGraph Friends of Friends recommendation service ranking model implementation
 * Date:  2012-04-18
***************************************************************************************************/

package com.renren.datamining.friendsoffriends;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;


public class FoFRankModel{
	
	public static final int kAvgFrdNum = 120;
	public static final int kDispMutFrdNum = 10;
	
	private static final double getDayNum(final int seconds1, final int seconds2) {
		if (seconds1 > 0 && seconds2 > 0) {
			final Date now = new Date();
			final float nowSeconds = now.getTime() / 1000L; 	
			final float t1 = (nowSeconds - seconds1) / 3600 + 1;
			final float t2 = (nowSeconds - seconds2) /3600 + 1;
			final double hours = t1 * t2;
			return hours;
		} else
			return -1;
	}
	
	public static List<FoFRank> getFoFRankList(final Map<Integer, List<FoFRankFormula> > fofId2FormulaListMap, final Set<Integer> blockSet) {
		if (!fofId2FormulaListMap.isEmpty()) {
			List<FoFRank> fofRankList = new ArrayList<FoFRank>();
			for (Entry<Integer, List<FoFRankFormula> > entry : fofId2FormulaListMap.entrySet()) {				
				final int fofId = entry.getKey();	
				if (blockSet.contains(fofId))  //filter block list
				    continue;			
				FoFRank fofRank = new FoFRank();	
				fofRank.fofId = fofId;
				List<FoFRankFormula> fofRankFormulaList = entry.getValue();				
				fofRank.mutualFriendNumber = fofRankFormulaList.size();
				Iterator<FoFRankFormula> it = fofRankFormulaList.iterator();

				while (it.hasNext()) {
					FoFRankFormula ffrf = it.next();
					int fiId = ffrf.fiId;
					int timeHostFi = ffrf.timeHostFi;
					int timeFiFoF = ffrf.timeFiFoF;
					int fiFrdNum = ffrf.fiFrdNum;
					if (fofRank.mutualFriendNumber <= 1)
						timeHostFi = (int) Math.pow(timeHostFi, 1.5);
					double timeI = getDayNum(timeHostFi, timeFiFoF);
					////calculate the weight of fof to be recommended to the user 
					//u according to the formulation of "Facebook's People You may know"
					if ((fiFrdNum > 0) && (timeI > 0))
						fofRank.rank += (float) (Math.pow(timeI, -0.3) / Math.sqrt(fiFrdNum));
					else {
						if (timeI > 0) {
							if (fiFrdNum == 0)
								fiFrdNum = kAvgFrdNum;
							fofRank.rank += (float) (Math.pow(timeI, -0.3) / Math.sqrt(fiFrdNum));
						} else {
							System.out.print("fiId: " + fiId + " fiFrdNum: "+ fiFrdNum + " timeI: " + timeI
									+ "timeHostFi: " + timeHostFi + " timeFiFoF: " + timeFiFoF);
						}
					}
					if (fofRank.mutualFriendIdListSize < kDispMutFrdNum)
						fofRank.addtoMutalFriendIdList(fiId);
				}			
				if (fofRank.rank >= 0)
				    fofRankList.add(fofRank);
			}
			
			Collections.sort(fofRankList);	//sorting ranking values
			return fofRankList;
		}	else {
			return new ArrayList<FoFRank>();
		}
	}
	
}
