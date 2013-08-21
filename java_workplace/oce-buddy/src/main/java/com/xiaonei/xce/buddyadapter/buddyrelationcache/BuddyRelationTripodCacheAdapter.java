package com.xiaonei.xce.buddyadapter.buddyrelationcache;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import xce.buddy.BuddyDesc;

import com.renren.tripod.client.TripodClient;
import com.xiaonei.xce.domain.buddy.BuddyRelation;

/**
 * BuddyRelationTripodCacheAdapter <br/>
 * Relation: Friend 好友关系, Apply 申请关系, Applied 被申请关系, Block 拉黑关系, Blocked 被拉黑关系 <br/>
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class BuddyRelationTripodCacheAdapter {

  public final static String VERSION = "1.9";

  /** Desc是与BuddyRelationProducer一致!! */
  private static int kFriendDesc = 0x10;
  private static int kApplyDesc = 0x01;
  private static int kAppliedDesc = 0x02;
  private static int kBlockDesc = 0x04;
  private static int kBlockedDesc = 0x08;

  /** 单例 */
  private static BuddyRelationTripodCacheAdapter instance = null;

  private final String zkAddress =
      "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/TripodRedis";
  private final String namespaceId = "buddyrelationcache";
  private final String businessId = "buddyrelationcache";

  private TripodClient tripodClient = null;

  private BuddyRelationTripodCacheAdapter() {
//    tripodClient = new TripodClient(zkAddress, namespaceId, businessId);
  }

  public static BuddyRelationTripodCacheAdapter getInstance() {
    if (instance == null) {
      synchronized (BuddyRelationTripodCacheAdapter.class) {
        if (instance == null) {
          instance = new BuddyRelationTripodCacheAdapter();
        }
      }
    }
    return instance;
  }

  /**
   * 取两个id关系的接口
   * 
   * @param fromId
   * @param toId
   * @return
   */
  public BuddyRelation getRelation(int fromId, int toId) {
    RemoteLogger.getInstance().log(
        "BuddyRelationTripodCacheAdapter.getRelation()#version:" + VERSION);
    return BuddyRelationCacheAdapter.getInstance().getRelation(fromId, toId);
    // if (fromId <= 0 || toId <= 0) {
    // BuddyDesc desc = BuddyDesc.NoPath;
    // return new BuddyRelation(fromId, toId,
    // BuddyRelation.parseBuddyDesc(desc));
    // }
    // if (fromId == toId) {
    // BuddyDesc desc = BuddyDesc.Self;
    // return new BuddyRelation(fromId, toId,
    // BuddyRelation.parseBuddyDesc(desc));
    // }
    // boolean exchange = fromId > toId ? true : false;
    // String minId = String.valueOf(fromId);
    // String maxId = String.valueOf(toId);
    // if (exchange) {
    // String tmp = minId;
    // minId = maxId;
    // maxId = tmp;
    // }
    //
    // BuddyDesc desc = BuddyDesc.NoPath;
    // if (tripodClient != null) {
    // List<String> fields = new ArrayList<String>();
    // fields.add(maxId);
    // Map<String, String> hashPair = tripodClient.getHash(minId, fields);
    // String value = hashPair.get(maxId);
    // if (value != null) {
    // desc = parse(value, exchange);
    // }
    // }
    // return new BuddyRelation(fromId, toId,
    // BuddyRelation.parseBuddyDesc(desc));
  }

  /**
   * load
   * 
   * @param userId
   * @return
   */
//  public boolean load(int userId) {
//    if (userId <= 0) {
//      return false;
//    }
//    try {
//      return tripodClient.loadHash(String.valueOf(userId), true, 10000);
//    } catch (Exception e) {
//      e.printStackTrace();
//    }
//    return false;
//  }

  /**
   * 为了保证客户端不修改代码，故存在parse方法
   * 
   * @param value
   * @param exchange
   * @return
   */
  // private static BuddyDesc parse(String value, boolean exchange) {
  // BuddyDesc desc = BuddyDesc.NoPath;
  // int v = 0;
  // try {
  // v = Integer.parseInt(value);
  // if ((v & kFriendDesc) > 0) {
  // desc = BuddyDesc.Friend;
  // }
  // if ((v & kApplyDesc) > 0) {
  // desc = (exchange != true ? BuddyDesc.Apply : BuddyDesc.Applied);
  // }
  // if ((v & kAppliedDesc) > 0) {
  // desc = (exchange == true ? BuddyDesc.Apply : BuddyDesc.Applied);
  // }
  // if ((v & kBlockDesc) > 0) {
  // desc = (exchange != true ? BuddyDesc.Block : BuddyDesc.Blocked);
  // }
  // if ((v & kBlockedDesc) > 0) {
  // desc = (exchange == true ? BuddyDesc.Block : BuddyDesc.Blocked);
  // }
  // } catch (Exception e) {
  // e.printStackTrace();
  // return desc;
  // }
  // return desc;
  // }
}
