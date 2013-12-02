package com.renren.xce.socialgraph.updator;

import java.util.Comparator;

import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;

/**
 * Comparator between Item.Builder
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class ItemComparator implements Comparator<Item>{

	public int compare(Item o1, Item o2) {
		if (o2.getField() < o1.getField()) {
			return 1;
		} else {
			return 0;
		}
	}
}
