package com.renren.socialgraph.RecommendEmail;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.json.JSONArray;
import org.json.JSONObject;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.socialgraph.BusinessCacheAdapter;
import com.xiaonei.xce.socialgraph.SocialGraphAdapter;
import com.xiaonei.xce.socialgraph.ffucr.Recommend;
import com.xiaonei.xce.socialgraph.friendfinderworker.Common;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Item;
import com.xiaonei.xce.socialgraph.proto.GeneratedItems.Items;

public class RecommendEmail {

    public static void main(String[] args) throws Exception {

	if (args.length < 2) {
	    System.out.println("args is srcFile destFile");
	    System.exit(-1);
	}

	File file = new File(args[0]);
	if (!file.exists()) {
	    System.err.println("file error");
	    System.exit(-1);
	}

	BufferedReader reader = new BufferedReader(new FileReader(file));
	List<Integer> users = new ArrayList<Integer>();
	String line = null;
	while ((line = reader.readLine()) != null) {
	    int id;
	    try {
		id = Integer.parseInt(line);
		users.add(id);
	    } catch (Exception e) {
		continue;
	    }
	}
	reader.close();
	File destFile = new File(args[1]);
	BufferedWriter writer = new BufferedWriter(new FileWriter(destFile));
	for (int i = 0; i < users.size(); ++i) {
	    int uid = users.get(i);
	    try {
		work(uid, writer);		//error@2012.3.19 author:yize.tan  solved@2012.3.19
	    } catch (Exception e) {
		continue;
	    }

	    if (i % 100000 == 0) {
		System.out.println("i:" + i);
	    }
	}
	writer.close();
	System.exit(0);
    }

    private static List<Item> getRcdData(int id) {
	byte[] bin = new byte[0];
	bin = BusinessCacheAdapter.getInstance().get(BusinessCacheAdapter.RCD,    
		id);
	List<Item> itemList = new ArrayList<Item>();
	try {
	    Items items = Items.parseFrom(bin);
	    for (Item item : items.getItemsList()) {
		if (BusinessCacheAdapter.FFW.equals(item.getMessage())) {
		    itemList.add(item);
		}
	    }
	} catch (InvalidProtocolBufferException e) {
	    System.err.println(e.toString() + " id:" + id);
	}

	return itemList;

    }

    private static void work(int id, BufferedWriter writer) throws Exception {
	// List<Recommend> recommends = new ArrayList<Recommend>();
	// List<Common> commons = SocialGraphAdapter
	// .getFriendFinderWorkerAdapter().getCommonFriends(id, -1);
	// Collections.shuffle(commons);
	// List<Common> nl = commons;
	// if (commons.size() > 6) {
	// nl = commons.subList(0, 6);
	// }
	// for (Common c : nl) {
	// Recommend r = new Recommend();
	// r.setId(c.getUserId());
	// r.setCommonFriendCount(c.getShareCount());
	// r.getCommonFriends().addAll(c.getShares());
	// recommends.add(r);
	// }

	// Set<Integer> idSet = new HashSet<Integer>();
	// for (Recommend r : recommends) {
	// idSet.add(r.getId());
	// idSet.addAll(r.getCommonFriends());
	// }

	Set<Integer> idSet = new HashSet<Integer>();
	List<Item> itemList = getRcdData(id);		
	for (Item item : itemList) {
	    idSet.add(item.getId());
	    idSet.addAll(item.getFieldsList());
	}
	
	if (idSet.isEmpty()) {
		//System.out.println("id didn't find in the RcdData"+id);		//test@yize.tan 2012.3.19
	    return;
	}

	Map<Integer, WUserCache> userCacheMap = SnsAdapterFactory
		.getUserCacheAdapter().getUserCacheMap(
			new ArrayList<Integer>(idSet));
	JSONObject jsonObject = new JSONObject();
	jsonObject.put("id", id);

	JSONArray jsonArray = new JSONArray();

	for (Item recommend : itemList) {
	    WUserCache recommendUserCache = userCacheMap.get(recommend.getId());
	    if (recommendUserCache == null) {
		continue;
	    }
	    JSONObject subObject = new JSONObject();
	    subObject.put("rid", recommendUserCache.getId());
	    subObject.put("name", recommendUserCache.getName());
	    subObject.put("commonFriendCount", recommend.getField());
	    subObject.put("tinyurl", recommendUserCache.getTiny_Url());
	    UserBasic userBasic = SnsAdapterFactory.getUserBasicAdapter().get(
		    recommendUserCache.getId());
	    subObject.put("homeProvince", userBasic.getHomeProvince());
	    subObject.put("homeCity", userBasic.getHomeCity());
	    if (userBasic.getUniv() != 0) {
		subObject.put("prefix", parseUnivInfo(userBasic.getUniv()));
		subObject.put("postfix", userBasic.getUnivName());
	    }
	    jsonArray.put(subObject);
	}

	jsonObject.put("recommend", jsonArray);
	writer.write(jsonObject.toString() + "\n");
    }

    private static String parseUnivInfo(int networkId) {
	if (networkId >= 1001 && networkId <= 19999999)
	    return "学校";
	if (networkId >= 20000001 && networkId <= 29999999)
	    return "公司";
	if (networkId >= 30000001 && networkId <= 49999999)
	    return "学校";
	if (networkId >= 60000001 && networkId <= 69999999)
	    return "城市";
	if (networkId >= 80000000 && networkId <= 90000000)
	    return "学校";
	return "";
    }
}