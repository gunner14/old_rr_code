package com.renren.addminifeed;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.cassandra.cli.CliParser.exitStmt_return;

import com.xiaonei.platform.component.feed.socail.AddFriendMiniPublisher;
import com.xiaonei.platform.component.feed.socail.model.AddFriendMiniModel;
import com.xiaonei.platform.component.feed.socail.model.FriendInfo;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddybyaddtimecache.DefaultBuddyByAddTime;
import com.xiaonei.xce.log.ActiveTrack;
import xce.socialgraph.util.Clogging;

public class AddFriendMiniFeed {

    /**
     * @param args
     * @throws
     */
    public static void main(String[] args) {
    
    if(2 != args.length) {
    	Clogging.Error("addfriendminifeed error args");
    	System.exit(0);
    }
    Clogging.InitLogger("addfriendminifeed");
    Clogging.Info(Arrays.toString(args));
	  
	final int ONE_DAY_LONG = 24 * 60 * 60 * 1000;

	long nowLong = System.currentTimeMillis();
	Date nowDate = new Date(nowLong);
	Date yesDate = new Date(nowLong - ONE_DAY_LONG);

	Calendar cal = Calendar.getInstance();

	cal.setTimeInMillis(nowLong - ONE_DAY_LONG);
	cal.set(yesDate.getYear() + 1900, yesDate.getMonth(),
		yesDate.getDate(), 0, 0, 0);

	long beginTime = cal.getTimeInMillis();
	cal.set(yesDate.getYear() + 1900, yesDate.getMonth(),
		yesDate.getDate(), 23, 59, 59);
	long endTime = cal.getTimeInMillis();

	
	File file = new File(args[0]);
	File whiteFile = new File(args[1]);
	
	if (!file.exists()) {
	    Clogging.Error("open total id list file failed");
	    System.exit(-1);
	}
	if (!whiteFile.exists()) {
		Clogging.Error("open white list file failed");
		System.exit(-1);
	}

	BufferedReader reader = null;
	List<Integer> idList = new ArrayList<Integer>();
	Map<Integer, Integer> whiteMap = new HashMap<Integer, Integer>();
	try {
	    reader = new BufferedReader(new FileReader(file));
	    String line = null;
	    while ((line = reader.readLine()) != null) {
	    	try {
	    		int id = Integer.parseInt(line.trim());
	    		idList.add(id);
	    	} catch (NumberFormatException e) {
	    		e.printStackTrace();
	    		continue;
	    	}
	    }
	    // white list
	    reader = new BufferedReader(new FileReader(whiteFile));
	    while ((line = reader.readLine()) != null) {
	    	try {
	    		int id = Integer.parseInt(line.trim());
	    		whiteMap.put(id, id);
	    	} catch (NumberFormatException e) {
	    		e.printStackTrace();
	    		continue;
	    	}
	    }
	      
	    if (!idList.isEmpty()) {
		for (Integer id : idList) {
		    int host = id;
		    if (true == whiteMap.containsKey(host)) {
		    	continue;
		    }
		    List<DefaultBuddyByAddTime> friendsByAddTime = null;
		    try {
			friendsByAddTime = BuddyByAddTimeCacheAdapter
				.getInstance().getFriendListWithinTime(host,
					beginTime, endTime);
			Clogging.Debug("host:" + host
				+ "     friendsByAddTime.size():"
				+ friendsByAddTime.size());
			if (friendsByAddTime != null
				&& !friendsByAddTime.isEmpty()) {
			    List<Integer> friends = new ArrayList<Integer>();
			    for (DefaultBuddyByAddTime bba : friendsByAddTime) {
				friends.add(bba.getUserId());
			    }
			    List<Integer> userIds = new ArrayList(friends);
			    userIds.add(host);
			    Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory
				    .getUserCacheAdapter().getUserCacheMap(userIds);

			    WUserCache hostUserCache = userCacheMap.get(host);
			    if (hostUserCache == null) {
			    	Clogging.Info("get use cache null  id:"
					+ host);
			    	continue;
			    }

			    // 10个一发
			    List<Integer> temp = new ArrayList<Integer>();
			    for (int i = 0; i < friends.size(); ++i) {
				temp.add(friends.get(i));
				if (i == friends.size() - 1) {
				    doWork(hostUserCache, temp, userCacheMap);
				    temp.clear();
				}
				if (temp.size() == 10) {
				    doWork(hostUserCache, temp, userCacheMap);
				    temp.clear();
				}
			    }
			}
		    } catch (Ice.TimeoutException e) {
			e.printStackTrace();
			continue;
		    }
		}
	    }

	} catch (FileNotFoundException e) {
	    e.printStackTrace();
	} catch (IOException e) {
	    e.printStackTrace();
	} finally {
	    if (reader != null) {
		try {
		    reader.close();
		} catch (IOException e) {
		    // TODO Auto-generated catch block
		    e.printStackTrace();
		}
	    }
	}

	System.exit(0);
    }

    static void doWork(WUserCache hostUserCache, List<Integer> friends,
	    Map<Integer, WUserCache> userCacheMap) {
    Clogging.Info("host:" + hostUserCache.getId() + "\tfriends:"
    		+ friends);
	try {
	    ActiveTrack.action(hostUserCache.getId(),
		    new String[] { "ADDRBOOK" },
		    "ADD_FRIEND_PROFILE_MINI_FEED", friends.toString());
	} catch (Exception e) {
	    e.printStackTrace();
	}

	AddFriendMiniModel model = new AddFriendMiniModel();
	model.setFromID(hostUserCache.getId());
	model.setFromName(hostUserCache.getName());
	for (Integer friend : friends) {
	    WUserCache friendUserCache = userCacheMap.get(friend);
	    if (friendUserCache == null) {
		continue;
	    }
	    FriendInfo friendInfo = new FriendInfo();
	    friendInfo.setFriendID(friendUserCache.getId());
	    friendInfo.setFriendName(friendUserCache.getName());
	    friendInfo.setFrindHead(friendUserCache.getTiny_Url());
	    model.addFriendInfo(friendInfo);
	}
	try {
	    AddFriendMiniPublisher.publish(model);
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }
}
