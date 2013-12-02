package com.renren.ad.sns.services.uranus;

import java.util.Calendar;

/**
 * Modify the {@link CompositeMetadataProvider}'s getXMetaData methods to
 * the following:<br/>
 * if the data is not in cache, just return a default XMetaData Object,
 * rather than query the database.
 * 
 * @author xiang.zhang last modified: 2012/06/05
 * 
 */
public class SimpleMetadataProvider extends CompositeMetadataProvider {

    @Override
    public UserMetadata getUserMetaData(long uid) {
        if (users.containsKey(uid)) {
            synchronized (usersLock) {
                if (users.containsKey(uid)) {
                    return users.get(uid);
                }
            }
        }
        return DefaultUserMetaData.getDefaultUserMetaDataObject();
    }

    @Override
    public GoodsMetadata getGoodsMetaData(final long uid, final long gid) {
        GoodsMetadata result = null;
        try {
            int ownerId = 0;
            if (staticGoods.containsKey(gid)) {
                int[] sg = null;
                synchronized(staticGoodsLock){
                    sg = staticGoods.get(gid);
                }
                
                if (sg != null && sg.length > 0) {
                    ownerId = sg[0];
                    long key = uid * Integer.MAX_VALUE + ownerId;
                    synchronized(u2usLock){
                        synchronized(u2csLock){
                            if (u2us.containsKey(key) && u2cs.containsKey(key)) {
                                int[] u2u = u2us.get(key);
                                int[] u2c = u2cs.get(key);
                                int[] now = new int[2];
                                now[0] = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
                                now[1] = Calendar.getInstance().get(Calendar.DAY_OF_WEEK);
                                result = new GoodsMetadata(sg, u2u, u2c, now);
                            }
                        }
                    }
                }
            }
        } catch (Exception e) {
            LOG.error(e, e);
        } finally {
            if (result == null) {
                result = DefaultGoodsMetaData.getDefaultGoodsMetadataObject();
            }
        }
        return result;
    }

}

class DefaultUserMetaData {

    private static final long DEFAULT_USER_ID = 0;

    private static final UserMetadata instance = new UserMetadata(DEFAULT_USER_ID);

    public static UserMetadata getDefaultUserMetaDataObject() {
        return instance;
    }
}

class DefaultGoodsMetaData {

    private static final long DEFAULT_GOODS_ID = 0;

    private static final GoodsMetadata instance = new GoodsMetadata(DEFAULT_GOODS_ID);

    public static GoodsMetadata getDefaultGoodsMetadataObject() {
        return instance;
    }
}
