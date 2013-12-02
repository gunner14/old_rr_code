package com.renren.ad.sns.services.uranus;

import java.util.List;

/**
 * interface of data provider for uranus recommender engine
 * 
 * @author ivoryxiong
 *
 */
public interface IDataProvider {
	public UserMetadata getUserMetaData(long uid);
	
	public GoodsMetadata getGoodsMetaData(final long uid , final long gid);
	
	public List<GoodsMetadata> getGoodsMetaData(final long gid , final List<Long> gids);
	
}
