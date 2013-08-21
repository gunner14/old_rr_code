package com.xiaonei.xce.notify;

import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.log.Oce;

import xce._notify.NotifyGatePrx;
import xce._notify.NotifyGatePrxHelper;
import xce._notify.NotifyIndexManagerPrx;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;

public class NotifyGateAdapter extends AbstractModProxyHolder<Integer, NotifyGatePrx>{

    private static NotifyGateAdapter instance;
    private static String endpoints = "@NotifyGate";

    private NotifyGateAdapter() {
      super(ChannelFactory.getChannel(ChannelType.NOTIFY),
          "M"+endpoints, 10);
    }

    public static NotifyGateAdapter getInstance() {
      if (instance == null) {
        instance = new NotifyGateAdapter();
      }
      return instance;
    }

    @Override
    protected int mod(Integer key, int size) {
      return key % size;
    }

    @Override
    public NotifyGatePrx uncheckedCast(ObjectPrx baseProxy) {
      return NotifyGatePrxHelper.uncheckedCast(baseProxy);
    }

    @Override
    protected ObjectPrx initProxy(ObjectPrx baseProxy) {
      return baseProxy;
    }
    
    private NotifyGatePrx getManagerOneway(int uid) {
      return uncheckedCast(getProxy(uid).ice_oneway().ice_timeout(300));
    }

    private NotifyGatePrx getManager(int uid) {
      return uncheckedCast(getProxy(uid).ice_timeout(300));
    }
    
    public List<String> getByType(int uid,  int view, int[] type, long lastNotifyId, int limit)  {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = prx.getByType(uid, view, type, lastNotifyId, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getByType [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public List<String> getReplySeq(int uid, int view, long lastNotifyId, int limit){
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = prx.getReplySeq(uid, view, lastNotifyId, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getReplySeq [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public List<String> getNotifySeq(int uid, int view, long lastNotifyId, int limit)  {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = prx.getNotifySeq(uid, view, lastNotifyId, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getNotifySeq [ " + prx + " ] uid=" + uid;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    
    public List<String> getByType2(int uid, int view, int[] type, int begin, int limit) {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = getManager(uid).getByType2(uid, view, type, begin, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getByType2 [ " + prx + " ] uid=" + uid;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public List<String> getReplySeq2(int uid, int view, int begin, int limit) {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = getManager(uid).getReplySeq2(uid, view, begin, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getReplySeq2 [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public List<String> getNotifySeq2(int uid, int view, int begin, int limit) {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = getManager(uid).getNotifySeq2(uid, view, begin, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getReplySeq2 [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    //  void RemoveLogicSource(int uid, int type, long source);
    //    void RemoveLogicBySourceSeq(int uid, int type, MyUtil::LongSeq sources);
    public void RemoveLogicBySource(int uid, int type, long source) {
      NotifyGatePrx prx = getManagerOneway(uid);
      try{
        prx.RemoveLogicSource(uid, type, source);
      } catch (Exception e){
        String err = this.getClass().getName() + ".RemoveLogicBySource [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public void RemoveLogicBySourceSeq(int uid, int type, long[] sources) {
      NotifyGatePrx prx = getManagerOneway(uid);
      try{
        prx.RemoveLogicBySourceSeq(uid, type, sources);
      } catch (Exception e){
        String err = this.getClass().getName() + ".RemoveLogicBySourceSeq [ " + prx + " ] uid=" + uid ;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
    public List<String> getRMsgByType(int uid, int view, int type, int begin, int limit) {
      NotifyGatePrx prx = getManager(uid);
      List<String> l = new ArrayList<String>();
      try{
        String[] res = prx.getRMsgByType(uid, view, type, begin, limit);
        for(int i = 0 ;i < res.length; ++i){
          l.add(res[i]);
        }
        return l;
      } catch (Exception e){
        String err = this.getClass().getName() + ".getRMsgByType [" + prx + "] uid " + uid;
        Oce.getLogger().error(err + " " + e);
        throw new RuntimeException(err,e);
      }
    }
		public int getUndealCountByBigType(int uid, int bigType){
			NotifyGatePrx prx = getManager(uid);
			int undealCount = 0;
			try{
				undealCount = prx.getUndealCountByBigType(uid, bigType);
				return undealCount;
			}catch (Exception e){
				String err = this.getClass().getName() + ".GetUndealCountByBigtype[" + prx + "] uid:" + uid;
				Oce.getLogger().error(err + " " + e);
				throw new RuntimeException(err,e);
			}
		}

}
