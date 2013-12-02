package com.renren.ad.sns.services.uranus.prg;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Ranker {
    
    /**
     * Sort by the natural order 
     * @param cpa
     * @param MaxNum
     * @return
     */
	public static List<Integer> rank(List<Double> cpa, int MaxNum) {
		class unit {
			int index;
			double value;
		}

		ArrayList<unit> goods = new ArrayList<unit>();
		ArrayList<Integer> result = new ArrayList<Integer>();
		int i;

		for (i = 0; i < cpa.size(); i++) {
			unit good = new unit();
			good.index = i;
			good.value = cpa.get(i);
			goods.add(good);
		}

		Collections.sort(goods, new Comparator<unit>() {
			public int compare(unit arg0, unit arg1) {
				if (arg0.value > arg1.value)
					return 1;
				else
					return -1;
			}
		});

		if (MaxNum > goods.size())
			MaxNum = goods.size();

		for (i = 0; i < MaxNum; i++) {
			result.add(goods.get(i).index);
		}

		return result;

	}

	/**
	 * Sort by the reversed natural order 
	 * @param score
	 * @param cpa
	 * @param MaxNum
	 * @return
	 */
	public static List<Integer> rank(List<Double> score, List<Double> cpa, int MaxNum) {
		class unit {
			int index;
			double value;
		}

		ArrayList<unit> goods = new ArrayList<unit>();
		ArrayList<Integer> result = new ArrayList<Integer>();
		int i;

		for (i = 0; i < score.size(); i++) {
			unit good = new unit();
			good.index = i;
			good.value = score.get(i) * cpa.get(i);
			goods.add(good);
		}
		Collections.sort(goods, new Comparator<unit>() {
			public int compare(unit arg0, unit arg1) {
				if (arg0.value <= arg1.value)
					return 1;
				else
					return -1;
			}
		});

		if (MaxNum > goods.size())
			MaxNum = goods.size();

		for (i = 0; i < MaxNum; i++) {
			result.add(goods.get(i).index);
		}

		return result;
	}

}
