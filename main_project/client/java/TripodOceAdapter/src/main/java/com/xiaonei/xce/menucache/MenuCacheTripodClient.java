package com.xiaonei.xce.menucache;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import xce.menu.CacheType;
import xce.menu.MenuCacheRecord;
import xce.menutripod.MenuTripodData.MenuData;

import com.google.protobuf.InvalidProtocolBufferException;
import com.renren.tripod.client.TripodClient;

public class MenuCacheTripodClient {
  final private String ZK_ADDRESS =
    "UserZooKeeper1.n.xiaonei.com:2181,UserZooKeeper2.n.xiaonei.com:2181,UserZooKeeper3.n.xiaonei.com"
    + ":2181,UserZooKeeper4.n.xiaonei.com:2181,UserZooKeeper5.n.xiaonei.com:2181/Tripod";
  final private String ZK_NAMESPACE = "ne1";


  private TripodClient recentMenuClient_ = new TripodClient(ZK_ADDRESS, ZK_NAMESPACE, "RecentMenu");
  private TripodClient favoriteMenuClient_ = new TripodClient(ZK_ADDRESS, ZK_NAMESPACE, "FavoriteMenu");
  private MenuCacheTripodClient() {
  }
  private static MenuCacheTripodClient instance_ = new MenuCacheTripodClient();
  public static MenuCacheTripodClient getInstance() {
    return instance_;
  }
  public Map<CacheType, int[]> get(int userId, CacheType[] typeArray) {
    Map<CacheType, int[]> resultMap = new HashMap<CacheType, int[]>();
    for (CacheType cacheType : typeArray) {
      TripodClient client = getTripodClientWithCacheType(cacheType);
      MenuCacheRecord record = getFromTripodCache(client, userId);
      resultMap.put(cacheType, record.infoArray);
    }
    return resultMap;
  }
  
  public Map<CacheType, int[]> getAll(int userId) {
    CacheType[] cacheTypes = {CacheType.FAVORITE, CacheType.RECENTLY};
    return get(userId, cacheTypes);
  }
  
  private TripodClient getTripodClientWithCacheType(CacheType cacheType) {
    if (cacheType == CacheType.RECENTLY) {
      return recentMenuClient_;
    } else {
      return favoriteMenuClient_;
    }
  }
  
  private MenuCacheRecord getFromTripodCache(final TripodClient client, int userId) {
    MenuCacheRecord record = new MenuCacheRecord();
    record.infoArray = new int[0];
    List<String> keys = new ArrayList<String>();
    List<String> missedKeys = new ArrayList<String>();
    keys.add(String.valueOf(userId));
    Map<String, byte[]> dataMap = client.get(keys, missedKeys);
    client.load(missedKeys, 0, false);
    if (dataMap.containsKey(String.valueOf(userId))) {
      try {
        MenuData data = MenuData.parseFrom(dataMap.get(String.valueOf(userId)));
        record.infoArray = new int[data.getAppIdsCount()];
        for (int i = 0; i < data.getAppIdsCount(); ++i) {
          record.infoArray[i] = data.getAppIds(i);
        }
      } catch (InvalidProtocolBufferException e) {
        e.printStackTrace();
      }
    }
    return record;
  }
}
