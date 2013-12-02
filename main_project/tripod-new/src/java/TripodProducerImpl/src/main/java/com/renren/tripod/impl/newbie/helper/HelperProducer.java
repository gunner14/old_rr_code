package com.renren.tripod.impl.newbie.helper;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.newbie.dao.NewbieHelperDAO;
import com.renren.newbie.module.NewBieHelperStep;
import com.renren.tripod.TripodProducer;
import com.xiaonei.mau.recall.vip.dao.LoginEventDAO;
import com.xiaonei.mau.recall.vip.model.LoginEventLogs;

public class HelperProducer implements TripodProducer {

	
	private final NewbieHelperDAO newbieHelperDAO = NewbieHelperDAO.getInstance();
	
	public Map create(String[] keys, boolean isMaster) {
		final Map<String, byte[]> rtmap = new HashMap<String, byte[]>();
        for (final String key : keys) {
        	int id = Integer.valueOf(key);
        	NewBieHelperStep stephelper = newbieHelperDAO.getNewbieStep(id);
        	ByteArrayOutputStream bots = new ByteArrayOutputStream();
        	byte[] step = null;
        	try {
				ObjectOutputStream ots = new ObjectOutputStream(bots);
				ots.writeObject(stephelper);
				step = bots.toByteArray();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            rtmap.put(key, step);
        }
       
        return rtmap;
	}

	public String[] createKeys(String beginKey, int limit) {
        int beginUserid;
        try {
            beginUserid = Integer.valueOf(beginKey);
        } catch (final NumberFormatException e) {
            e.printStackTrace();
            return new String[] {};
        }
        final List<Integer> intlist = NewbieHelperDAO.getInstance().getUserIds(beginUserid, limit);
        
        final List<String> list = new ArrayList<String>();
        for (final Integer integer : intlist) {
            list.add("" + integer);
        }
        return list.toArray(new String[] {});
	}

	public String[] createHotKeys(String beginKey, int limit) {
		// TODO Auto-generated method stub
		return null;
	}
	
}
