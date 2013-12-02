/**
 * 
 */
package com.renren.tripod.impl.mau.viprecall;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.tripod.TripodProducer;
import com.xiaonei.mau.recall.vip.dao.LoginUserDAO;

/**
 * 活跃用户的cache写 <br>
 * +---------+ <br>
 * | host_id | <br>
 * +---------+ <br>
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Mar 12, 2012 2:46:17 PM
 * 
 */
public class ActiveFriProducer implements TripodProducer {

    private final LoginUserDAO loginUserDAO = LoginUserDAO.getInstance();

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#create(java.lang.String[], boolean)
     */
    @Override
    public Map<String, byte[]> create(final String[] keys, final boolean isMaster) {
        final Map<String, byte[]> rtmap = new HashMap<String, byte[]>();
        for (final String key : keys) {
            rtmap.put(key, "0".getBytes());
        }
        return rtmap;
    }

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#createHotKeys(java.lang.String, int)
     */
    @Override
    public String[] createHotKeys(final String beginKey, final int limit) {
        // no hot key
        return null;
    }

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#createKeys(java.lang.String, int)
     */
    @Override
    public String[] createKeys(final String beginKey, final int limit) {
        int begin;
        try {
            begin = Integer.valueOf(beginKey);
        } catch (final NumberFormatException e) {
            e.printStackTrace();
            return null;
        }
        final List<Integer> list = this.loginUserDAO.getLoginUser(2012, 2, begin, limit);
        final List<String> listS = new ArrayList<String>(list.size());
        for (final Integer integer : list) {
            listS.add(String.valueOf(integer));
        }
        return listS.toArray(new String[] {});
    }
}
