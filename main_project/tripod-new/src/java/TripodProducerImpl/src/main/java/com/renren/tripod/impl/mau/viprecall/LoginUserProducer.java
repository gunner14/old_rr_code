/**
 * 
 */
package com.renren.tripod.impl.mau.viprecall;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.tripod.TripodProducer;
import com.xiaonei.mau.recall.vip.dao.LoginEventDAO;
import com.xiaonei.mau.recall.vip.model.LoginEventLogs;

/**
 * 记录非活跃登陆用户的cache，方便在线上使用
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Mar 12, 2012 8:40:02 PM
 * 
 */
public class LoginUserProducer implements TripodProducer {

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#create(java.lang.String[], boolean)
     */
    @Override
    public Map<String, byte[]> create(final String[] keys, final boolean isMaster) {
        // 写入cache
        final Map<String, byte[]> result = new HashMap<String, byte[]>();
        for (final String key : keys) {
            result.put(key, "0".getBytes());
        }

        return result;
    }

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#createHotKeys(java.lang.String, int)
     */
    @Override
    public String[] createHotKeys(final String beginKey, final int limit) {
        return null;
    }

    /* (non-Javadoc)
     * @see com.renren.tripod.TripodProducer#createKeys(java.lang.String, int)
     */
    @Override
    public String[] createKeys(final String beginKey, final int limit) {
        int beginUserid;
        try {
            beginUserid = Integer.valueOf(beginKey);
        } catch (final NumberFormatException e) {
            e.printStackTrace();
            return new String[] {};
        }
        final LoginEventLogs lel = LoginEventDAO.getInstance().getUserIds(beginUserid, limit);
        final List<Integer> intlist = lel.getList();
        final List<String> list = new ArrayList<String>();
        for (final Integer integer : intlist) {
            list.add("" + integer);
        }
        return list.toArray(new String[] {});
    }

}
