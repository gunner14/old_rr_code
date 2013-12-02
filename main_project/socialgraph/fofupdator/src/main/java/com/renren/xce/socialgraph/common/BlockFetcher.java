package com.renren.xce.socialgraph.common;

import java.util.HashMap;
import java.util.List;
import java.util.Set;

/**
 * interface of getting block list
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public interface BlockFetcher {
	public List<Long> getBlockList(int hostId); 
	
	public HashMap<Integer, Set<Integer>> getBlockSet(List<Integer> hostIdList);
}
