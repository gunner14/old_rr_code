package com.renren.xce.socialgraph.block;

import java.util.HashMap;
import java.util.List;
import java.util.Set;

import xce.socialgraph.recommend.DbUtil;

import com.renren.xce.socialgraph.common.BlockFetcher;

/**
 * fetch block list from table socialgraph_table
 * @author zhangnan
 * @email zhangnan@renren-inc.com
 */
public class SgBlockFetcher implements BlockFetcher {
	public List<Long> getBlockList(int hostId) {
		List<Long> blockList = DbUtil.getInstance().getSgBlockList(hostId);
		return blockList;
	}

	public HashMap<Integer, Set<Integer>> getBlockSet(List<Integer> hostIdList) {
		// TODO Auto-generated method stub
		return null;
	}
}
