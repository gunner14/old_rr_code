package com.xiaonei.xce.notify;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Iterator;

import com.xiaonei.xce.log.Oce;

import xce._notify.NotifyContent;
import xce._notify.NotifyIndexManagerNPrx;
import xce._notify.NotifyIndexManagerNPrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;


public class  NotifyIndexNAdapter extends AbstractModProxyHolder<Integer, NotifyIndexManagerNPrx>{

  private static NotifyIndexNAdapter instance;
  private static String endpoints = "@NotifyIndexN";

  private NotifyIndexNAdapter() {
    super(ChannelFactory.getChannel(ChannelType.NOTIFY), "M" + endpoints, 10);
  }

  public static NotifyIndexNAdapter getInstance() {
    if (instance == null) {
      instance = new NotifyIndexNAdapter();
    }
    return instance;
  }
  @Override
  protected int mod(Integer key, int size) {
    return key % size;
  }

  @Override
  public NotifyIndexManagerNPrx uncheckedCast(ObjectPrx baseProxy) {
    return NotifyIndexManagerNPrxHelper.uncheckedCast(baseProxy);
  }

  @Override
  protected ObjectPrx initProxy(ObjectPrx baseProxy) {
    return baseProxy;
  }
  
  private NotifyIndexManagerNPrx getManagerOneway(int uid) {
    return uncheckedCast(getProxy(uid).ice_oneway().ice_timeout(300));
  }

  private NotifyIndexManagerNPrx getManager(int uid) {
    return uncheckedCast(getProxy(uid).ice_timeout(300));
  }

/*
 * void RemoveRMessageById(int uid, long notifyId, int type, int bigtype);
 * void RemoveRMessageByIds(int uid, MyUtil::LongSeq notifyIds, 
 *   int type, int bigtype);
 * void RemoveRMessageBySource(int uid, int type, long source, int bigtype);
 *
 * */
  public void RemoveRMessageById(int uid, long notifyId, int type)  {
    NotifyIndexManagerNPrx prx = getManagerOneway(uid);
    try{
      prx.RemoveRMessageById(uid, notifyId, type, 1);
    }catch(Exception e){
      String err = this.getClass().getName() + ".RemoveRMessageById [ " + 
          prx + " ] uid=" + uid ;
      Oce.getLogger().error(err + " " + e);
      throw new RuntimeException(err,e);
    }
  }
  public void RemoveRMessageByIds(int uid, long[] notifyIds, int type)  {
//    getManagerOneway(uid).RemoveRMessageByIds(uid, notifyIds, type, 1);
    NotifyIndexManagerNPrx prx = getManagerOneway(uid);
    try{
      prx.RemoveRMessageByIds(uid, notifyIds, type, 1);
    }catch(Exception e){
      String err = this.getClass().getName() + ".RemoveRMessageById [ " 
          + prx + " ] uid=" + uid;
      Oce.getLogger().error(err + " " + e);
      throw new RuntimeException(err,e);
    }
	}
	public void RemoveRMessageByIdNoType(int userId, long notifyId)  {
		//    getManagerOneway(uid).RemoveRMessageByIds(uid, notifyIds, type, 1);
		NotifyIndexManagerNPrx prx = getManagerOneway(userId);
		try{
			prx.RemoveRMessageByIdNoType(userId, notifyId);
		}catch(Exception e){
			String err = this.getClass().getName() + ".RemoveRMessageByIdNoType[ "
				+ prx + " ] uid=" + userId;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}

  public void RemoveRMessageBySource(int uid, int type, long source){
//    getManagerOneway(uid).RemoveRMessageBySource(uid, type, source, 1);
    NotifyIndexManagerNPrx prx = getManagerOneway(uid);
    try{
      prx.RemoveRMessageBySource(uid, type, source, 1);
    }catch(Exception e){
      String err = this.getClass().getName() + ".RemoveRMessageBySource [ " 
          + prx + " ] uid=" + uid ;
      Oce.getLogger().error(err + " " + e);
      throw new RuntimeException(err,e);
    }
  }
  public void RemoveRMessageBySourceSeq(int uid, int type, long[] sources){
//    getManagerOneway(uid).RemoveRMessageBySourceSeq(uid, type, sources, 1);
    NotifyIndexManagerNPrx prx = getManagerOneway(uid);
    try{
      prx.RemoveRMessageBySourceSeq(uid, type, sources, 1);
    }catch(Exception e){
      String err = this.getClass().getName() + ".RemoveRMessageBySourceSeq [ "
          + prx + " ] uid=" + uid ;
      Oce.getLogger().error(err + " " + e);
      throw new RuntimeException(err,e);
    }
  }
/*
  public int getUnreadCountByTypes(int uid, int[] types) {
    return getManager(uid).getUnreadCountByTypes(uid, types);
  }*/
  public int GetUndealCountByTypes(int uid, int[] types) {
    int count = 0;
    try {
      Map typeCountMap = getManager(uid).GetUndealCountByTypes(uid, types);
      Iterator it = typeCountMap.entrySet().iterator();
      while (it.hasNext()) {
        Map.Entry entry = (Map.Entry)it.next();
        Object val = entry.getValue();
        count += (Integer)val;
      }
      return count;
    } catch (Exception e) {
      String err = this.getClass().getName() + ".GetUndealCountByTypes uid[ "
          + uid + "] types size[" + types.length + "]";
      Oce.getLogger().error(err + " " + e);
      throw new RuntimeException(err, e);
		}
	}
	public Map GetEachUndealCountByTypes(int uid, int[] types) {
		try {
			Map typeCountMap = getManager(uid).GetUndealCountByTypes(uid, types);
			return typeCountMap;
		} catch (Exception e) {
			String err = this.getClass().getName() + ".GetEachUndealCountByTypes uid[ "
				+ uid + "] types size[" + types.length + "]";
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err, e);
		}
	}

  void removeBySourceAndSenderBigtype(int uid, int type, long source, int sender, int bigtype){
    NotifyIndexManagerNPrx prx = getManagerOneway(uid);
    try {
      prx.RemoveRMessageBySourceAndSender(uid, type, source, sender, bigtype);
    } catch (Exception e) {
      String err = this.getClass().getName() + ".removeBySourceAndSenderBigtype [" + prx + "] uid[" + uid + "]";
      Oce.getLogger().error(err + "Exception[" + e +"]");
      throw new RuntimeException(err,e);
    }
  }  
  //public static void main(String[] args)  {
  //  int[] types = {16};
  //  NotifyIndexNAdapter.instance.RemoveRMessageById(232626782, 123456789, 14);
    //NotifyIndexNAdapter.instance.getUnreadCountByTypes(220678672,types);
  //}


//  void removeById(int uid, long notifyId){
//    getManagerOneway(uid).removeById(uid, notifyId);
//  }
//  void removeByIds(int uid, long[] notifyIds){
//    getManagerOneway(uid).removeByIds(uid, notifyIds);
//  }
//  void removeBySource(int uid, int type, long source){
//    getManagerOneway(uid).removeBySource(uid, type,source);
//  }
//  
//  int getCount(int user, int[] types){
//    return getManager(user).getCount(user,types);
//  }
//  
//  List<Integer> getFromIdSeq(int user, int[] types, int begin, int limit){
//    int [] ids =  getManager(user).getFromIdSeq(user,types,begin, limit);
//    List<Integer> res = new ArrayList<Integer>();
//    for(int i = 0; i < ids.length; ++i){
//      res.add(ids[i]);
//    }
//    return res;
//  }
   
}
