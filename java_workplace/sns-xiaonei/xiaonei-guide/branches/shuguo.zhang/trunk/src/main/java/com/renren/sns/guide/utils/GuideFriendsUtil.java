package com.renren.sns.guide.utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.guide.model.GuideAbstractModel;
import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.guide.model.GuideIRelationFriend;
import com.renren.sns.guide.model.GuideIRelationModel;
import com.xiaonei.platform.component.friends.home.FriendsHome;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午10:56:02 此类是向外提供用的 所以不要轻易改类名或包路径
 */
public class GuideFriendsUtil {

    public static final Log log = LogFactory.getLog("friend");

    private static ExecutorService es = Executors.newCachedThreadPool();

    /**
     * 寻找可能认识的人 有超时机制
     * 
     * @param user
     * @param args
     * @return
     * @throws IllegalArgumentException
     */
    public static List<GuideIRelation> findPeople(final User user, GuideIRelationModel... args)
            throws IllegalArgumentException {
        if (args.length <= 0) {
            throw new IllegalArgumentException();
        }
        List<GuideIRelationModel> models = Arrays.asList(args);
        List<GuideIRelation> relation = new ArrayList<GuideIRelation>();
        List<Future<GuideIRelationModel>> futures = new ArrayList<Future<GuideIRelationModel>>();
        for (GuideIRelationModel model : models) {
            futures.add(es.submit(model));
        }
        long timeout = getTimeOut();
        long sum = timeout;
        Iterator<Future<GuideIRelationModel>> iter = futures.iterator();
        int cnt = 0;
        while (iter.hasNext()) {
        	cnt ++ ;
            GuideIRelationModel model = null;
            try {
                long begin = System.currentTimeMillis();
                if (sum <= 0) {
                    model = iter.next().get(0, TimeUnit.MILLISECONDS);
                } else {
                    model = iter.next().get(sum, TimeUnit.MILLISECONDS);
                }
                long end = System.currentTimeMillis();
                sum = sum - (end - begin);
                relation.addAll(model.getResult());
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
            } catch (TimeoutException e) {
                sum = 0;
                //System.err.println(e + "\t friend search timeout class:GuideFriendsUtil A"+" c:"+cnt);
                String mstr = null;
                try {
                	GuideAbstractModel m = (GuideAbstractModel) models.get(cnt-1);
                	long now = System.currentTimeMillis();
                	mstr = m.getClass().getSimpleName()+" c:"+(now-m.getBegin());
				} catch (Exception e2) {
					System.err.println("GuideFriendsUtil:"+e2.toString());
				}
                System.err.println(e + "\t friend search timeout class:GuideFriendsUtil A"+" i:"+cnt+" m:"+mstr);
            }
        }
        //过滤掉好友
        List<Integer> friendIdList = FriendsHome.getInstance().getFriendsIds(user.getId());
        for (GuideIRelation ir : relation) {
            List<GuideIRelationFriend> list = ir.getList();
            for (int index = list.size() - 1; index >= 0; index--) {
                if (friendIdList.contains(list.get(index).getId())
                        || user.getId() == list.get(index).getId()) {
                    list.remove(index);
                }
            }
        }
        return relation;
    }
    
    public static List<GuideIRelation> findPeople(final User user, List<GuideIRelationModel> models)
            throws IllegalArgumentException {
        List<GuideIRelation> relation = new ArrayList<GuideIRelation>();
        List<Future<GuideIRelationModel>> futures = new ArrayList<Future<GuideIRelationModel>>();
        for (GuideIRelationModel model : models) {
            futures.add(es.submit(model));
        }
        long timeout = getTimeOut();
        long sum = timeout;
        Iterator<Future<GuideIRelationModel>> iter = futures.iterator();
        int cnt = 0;
        while (iter.hasNext()) {
        	cnt ++ ;
            GuideIRelationModel model = null;
            try {
                long begin = System.currentTimeMillis();
                if (sum <= 0) {
                    model = iter.next().get(0, TimeUnit.MILLISECONDS);
                } else {
                    model = iter.next().get(sum, TimeUnit.MILLISECONDS);
                }
                long end = System.currentTimeMillis();
                sum = sum - (end - begin);
                relation.addAll(model.getResult());
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
            } catch (TimeoutException e) {
                sum = 0;
                String mstr = null;
                try {
                	GuideAbstractModel m = (GuideAbstractModel) models.get(cnt-1);
                	long now = System.currentTimeMillis();
                	mstr = m.getClass().getSimpleName()+" c:"+(now-m.getBegin());
				} catch (Exception e2) {
					System.err.println("GuideFriendsUtil:"+e2.toString());
				}
                System.err.println(e + "\t friend search timeout class:GuideFriendsUtil B"+" i:"+cnt+" m:"+mstr);
            }
        }
        //过滤掉好友
        List<Integer> friendIdList = FriendsHome.getInstance().getFriendsIds(user.getId());
        for (GuideIRelation ir : relation) {
            List<GuideIRelationFriend> list = ir.getList();
            for (int index = list.size() - 1; index >= 0; index--) {
                if (friendIdList.contains(list.get(index).getId())
                        || user.getId() == list.get(index).getId()) {
                    list.remove(index);
                }
            }
        }
        return relation;
    }

    private static final String SEARCH_TIME_OUT = "search_timeout";

    private static int getTimeOut() {
        try {
            MemCacheManager mgr = MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_profile);
            Integer timeout = (Integer) mgr.get(SEARCH_TIME_OUT);
            if (timeout == null) {
                timeout = new Integer(700);
                mgr.set(SEARCH_TIME_OUT, timeout, (int) MemCacheKeys.day);
                return timeout.intValue();
            } else {
                return timeout.intValue();
            }
        } catch (Exception e) {
            e.printStackTrace();
            return 700;
        }
    }
}
