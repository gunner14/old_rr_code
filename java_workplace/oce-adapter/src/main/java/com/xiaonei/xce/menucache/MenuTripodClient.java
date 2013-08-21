package com.xiaonei.xce.menucache;

import java.util.HashMap;
import java.util.Map;

import xce.menu.CacheType;
import xce.menu.MenuCacheRecord;
import xce.menutripod.MenuTripodData.MenuData;
import xce.tripod.TripodClient;
import xce.tripod.TripodException;

import com.google.protobuf.InvalidProtocolBufferException;
import com.xiaonei.xce.log.Oce;

public class MenuTripodClient {

    private static MenuTripodClient instance = null;

    private static final String NAMESPACE_ID = "xce_menu";

    private static final String RECENT_MENU_BIZ_ID = "recent_menu";

    private static final String FAVOURITE_MEUNU_BIZ_ID = "favourite_menu";

    private TripodClient recentMenuTripodClient = null;

    private TripodClient favouriteMenuTripodClient = null;

    private MenuTripodClient() {
        recentMenuTripodClient = new TripodClient(NAMESPACE_ID, RECENT_MENU_BIZ_ID);
        favouriteMenuTripodClient = new TripodClient(NAMESPACE_ID, FAVOURITE_MEUNU_BIZ_ID);
    }

    public static MenuTripodClient getInstance() {
        if (instance == null) {
            synchronized (MenuTripodClient.class) {
                if (instance == null) {
                    instance = new MenuTripodClient();
                }
            }
        }
        return instance;
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
        CacheType[] cacheTypes = { CacheType.FAVORITE, CacheType.RECENTLY };
        return get(userId, cacheTypes);
    }

    private TripodClient getTripodClientWithCacheType(CacheType cacheType) {
        if (cacheType == CacheType.RECENTLY) {
            return recentMenuTripodClient;
        } else {
            return favouriteMenuTripodClient;
        }
    }

    private MenuCacheRecord getFromTripodCache(final TripodClient client, int userId) {
        MenuCacheRecord record = new MenuCacheRecord();
        record.infoArray = new int[0];
        byte[] byteData = null;
        try {
            byteData = client.get(String.valueOf(userId), 0);
        } catch (TripodException e1) {
            Oce.getLogger().error(
                    this.getClass().getName() + ".getFromTripodCache userId=" + userId
                            + " TripodException msg:" + e1.getMessage());
        }
        try {
            MenuData data = MenuData.parseFrom(byteData);
            record.infoArray = new int[data.getAppIdsCount()];
            for (int i = 0; i < data.getAppIdsCount(); ++i) {
                record.infoArray[i] = data.getAppIds(i);
            }
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        return record;
    }

}
