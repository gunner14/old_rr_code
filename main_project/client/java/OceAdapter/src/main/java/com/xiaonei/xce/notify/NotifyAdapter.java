package com.xiaonei.xce.notify;

import java.util.List;
import java.util.Map;

public class NotifyAdapter {
  private static NotifyAdapter instance = null;

  
  private NotifyAdapter() {
    
  }
  
  public static NotifyAdapter getInstance() {
    if (instance == null) {
      instance = new NotifyAdapter();
    }
    return instance;
  }
  public void dispatch(NotifyBody notify){
    NotifyDispatcherAdapter.getInstance().dispatch(notify.getProps());
  }
  public void deliverWebpager(NotifyBody notify){
    NotifyDispatcherAdapter.getInstance().deliverWebpager(notify.getProps());
  }
  public void removeByNotifyId(int uid, long notifyId)  {
    NotifyIndexAdapter.getInstance().removeById(uid, notifyId);
  }
  public void removeByNotifyIds(int uid, long[] notifyIds)  {
    NotifyIndexAdapter.getInstance().removeByIds(uid, notifyIds);
  }
  
  
  /*
   * key 格式： type-source, 是类型与source合在一起的一个串
   */
  public void removeBySource(int uid, String key) {
    String[] l = key.split("-");
    if(l.length == 2){
      NotifyIndexAdapter.getInstance().removeBySource(uid, Integer.valueOf(l[0]), Long.valueOf(l[1]));
    }
  }
  
  public void removeBySource(int uid, int type, long source)  {
    NotifyIndexAdapter.getInstance().removeBySource(uid, type,source);
  }

	public void RemoveRMessageByIdNoType(int userId, long notifyId){
		NotifyIndexNAdapter.getInstance().RemoveRMessageByIdNoType(userId, notifyId);
	}
  /* 取回复和通知，返回结果即有回复又有通知
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * type: 回复与通知的类型， 按指定类型取
   * lastNotifyId: 上次取的列表中NotifyId最小的一个，返回的结果为比lastNotifyId小的列表，取最新的N条时，lastNotifyId取0
   * limit:返回多少条
   */
  public List<String> getByType(int uid, int view, int[] type, long lastNotifyId, int limit)  {
    return NotifyGateAdapter.getInstance().getByType(uid, view, type, lastNotifyId, limit);
  }
  /* 取回复
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * lastNotifyId: 上次取的列表中NotifyId最小的一个，返回的结果为比lastNotifyId小的列表，取最新的N条时，lastNotifyId取0
   * limit:返回多少条
   */
  public List<String> getReplySeq(int uid, int view, long lastNotifyId, int limit){
    return NotifyGateAdapter.getInstance().getReplySeq(uid, view, lastNotifyId, limit);
  }
  /* 取通知
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * lastNotifyId: 上次取的列表中NotifyId最小的一个，返回的结果为比lastNotifyId小的列表，取最新的N条时，lastNotifyId取0
   * limit:返回多少条
   */
  public List<String> getNotifySeq(int uid, int view, long lastNotifyId, int limit)  {
    return NotifyGateAdapter.getInstance().getNotifySeq(uid, view, lastNotifyId, limit);
  }
  /* 取回复和通知，返回结果即有回复又有通知
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * type: 回复与通知的类型， 按指定类型取
   * begin: 从那个位置开始取
   * limit: 取多少条
   */
  public List<String> getByType2(int uid, int view, int[] type,  int begin, int limit)  {
    return NotifyGateAdapter.getInstance().getByType2(uid, view, type, begin, limit);
  }
  /* 取回复
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * begin: 从那个位置开始取
   * limit: 取多少条
   */
  public List<String> getReplySeq2(int uid, int view, int begin, int limit){
    return NotifyGateAdapter.getInstance().getReplySeq2(uid, view, begin, limit);
  }
  /* 取通知
   * view: 各业务的视图类型， 如 0表示 首页的回复提示的视图
   * begin: 从那个位置开始取
   * limit: 取多少条
   */
  public List<String> getNotifySeq2(int uid, int view, int begin, int limit){
    return NotifyGateAdapter.getInstance().getNotifySeq2(uid, view, begin, limit);
  }
  /*注意：
   * 此接口只提供给手机使用，请注意，调用结果是直接从DB查的，调用量大的业务不要使用
   */
  public int getCount(int user, int[] types) {
    return NotifyIndexAdapter.getInstance().getCount(user, types);
  }
  /* get undeal count by the v6 interface IndexN to improve performance
   * the count stored by cache after it has been first loaded from database 
   */
  public int getUndealCountByTypes(int user, int[] types) {
    return NotifyIndexNAdapter.getInstance().GetUndealCountByTypes(user, types);
	}
	/*
	* 此接口和上面接口类似，但是上面接口是返回总数，这个接口会返回MAP，里面包含具体类型
	*/
	public Map getEachUndealCountByTypes(int user, int[] types) {
		return NotifyIndexNAdapter.getInstance().GetEachUndealCountByTypes(user, types);
	}

  /*注意：
   * 此接口只提供给小站使用，请注意，调用结果是直接从DB查的，调用量大的业务不要使用
   */
  public int getUnreadCountByTypes(int user, int[] types){
    return NotifyIndexAdapter.getInstance().getUnreadCountByTypes(user, types);
  }
	/*
	*  此接口给开放平台调用，根据大类型获取消息总数
	*/
	public int getUndealCountByBigType(int user, int bigType){
		return NotifyGateAdapter.getInstance().getUndealCountByBigType(user, bigType);
	}

  public List<Integer> getFromIdSeq(int user, int[] types, int begin, int limit){
    return NotifyIndexAdapter.getInstance().getFromIdSeq(user, types, begin, limit);
  }
  
  public void removeBySourceAndSender(int uid, int type, long source, int sender){
    NotifyIndexAdapter.getInstance().removeBySourceAndSender(uid, type, source, sender);
  }
  public void removeBySourceAndSenderBigtype(int uid, int type, long source, int sender, int bigtype){
    NotifyIndexAdapter.getInstance().removeBySourceAndSender(uid, type, source, sender);
    NotifyIndexNAdapter.getInstance().removeBySourceAndSenderBigtype(uid, type, source, sender, bigtype);
  }
  public void removeRMsgBySource(int uid, int type, long source){
    NotifyIndexAdapter.getInstance().removeBySource(uid, type, source);
    NotifyIndexNAdapter.getInstance().RemoveRMessageBySource(uid, type, source);
  }
  public List<String> getRMsgByType(int uid, int view, int type, int begin, int limit) {
    return NotifyGateAdapter.getInstance().getRMsgByType(uid, view, type, begin, limit);
  }
}
