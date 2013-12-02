package com.renren.ad.sns.services.uranus;

import java.util.List;

/**
 * interface of Candidate Picker
 * @author ivoryxiong
 *
 */
public interface ICandidatePicker {
	/**
	 * pick catidates by user id and goods' id
	 * @param uid
	 * @param gid
	 * @return 
	 */
	List<Candidate> pick(long uid , long gid);
	
	
	/**
	 * 降级版本的pick，返回默认候选集
	 * 
	 * @param uid
	 * @param gid
	 * @return
	 */
	List<Candidate> pickDefault(long uid, long gid);
	
}
