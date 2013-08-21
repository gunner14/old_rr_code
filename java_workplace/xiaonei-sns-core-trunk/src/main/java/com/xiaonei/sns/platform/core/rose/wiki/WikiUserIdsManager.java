package com.xiaonei.sns.platform.core.rose.wiki;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.wiki.dao.WikiUserIdDAO;
import org.springframework.beans.factory.InitializingBean;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

/**
 * Wiki开放策略门面
 * User: shunlong.jin/David
 * Date: 12-5-29
 * Time: 下午3:23
 */
public class WikiUserIdsManager implements InitializingBean {
    /**
     * 当type=1时，存放的是白名单；
     * 当type=2时，存放的是开放尾号（100以内）；
     * 当type=3时，存放的是from_user_id；
     * 当type=4时，存放的是to_user_id;
     * <p/>
     * 3和4类型是为了支持，对某个区间的user_id开放的情况。
     */
    public static enum WikiPolicyType {
        WHITE_IDS(1), TRAIL(2), FROM(3), TO(4);
        public int code;

        WikiPolicyType(int code) {
            this.code = code;
        }
    }

    //白名单
    private static Map<Integer, Integer> whiteUserIdsMap = new HashMap<Integer, Integer>();
    //开放尾号
    private static Map<Integer, Integer> userIdsTailMap = new HashMap<Integer, Integer>();
    //开放区间
    private static int fromUserId = -1;
    private static int toUserId = -1;

    private static int LOAD_RATE_MINUTE = 5;//以分钟为单位
    private static LoadWikiUserIdsThread loadWikiUserIdsThread = new LoadWikiUserIdsThread();
    private static boolean isThreadStart = false;

    private static WikiUserIdDAO wikiUserIdDAO = WikiUserIdDAO.getInstance();


    /**
     * 判断是否在开放之列
     *
     * @param user
     * @return
     */
    public boolean isWikiUser(final User user) {
        try {
            if(user == null){
                return false;
            }
            //added by shunlong.jin 因为白名单的用户太多了，只能放到userstate中
            //如果已经设置了State的标示
            if(user.getUserState().isSocialWikiOn()){
                return true;
            }
            int userId = user.getId();
            //先判断白名单
            if (whiteUserIdsMap.containsKey(userId)) {
                return true;
            }
            //再判断尾号
            int tail = userId % 100;
            if (userIdsTailMap.containsKey(tail)) {
                return true;
            }
            //再判断区间
            if (fromUserId != -1 && toUserId != -1 && userId >= fromUserId && userId < toUserId) {
                return true;
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return false;
    }
    /**
     * 判断是否在开放之列,这个接口只是为了兼容原来的旧接口
     * @deprecated
     * @param userId
     * @return
     */
    public boolean isWikiUser(final int userId) {
        try {
            User user = SnsAdapterFactory.getUserAdapter().get(userId);
            return isWikiUser(user);
        } catch (Throwable e) {
           e.printStackTrace();
        }
        return false;
    }

    /**
     * 重新加载开放策略
     */
    private static void loadWikiPolicy() {
        final List<Integer> _whiteUserIds = wikiUserIdDAO.getUserIds(WikiPolicyType.WHITE_IDS.code);
        final List<Integer> _userIdsTail = wikiUserIdDAO.getUserIds(WikiPolicyType.TRAIL.code);
        final List<Integer> _fromUserId = wikiUserIdDAO.getUserIds(WikiPolicyType.FROM.code);
        final List<Integer> _toUserId = wikiUserIdDAO.getUserIds(WikiPolicyType.TO.code);

        final Map<Integer, Integer> _whiteUserIdsMap = new HashMap<Integer, Integer>();
        final Map<Integer, Integer> _userIdsTailMap = new HashMap<Integer, Integer>();

        fromUserId = (_fromUserId.size() != 0) ? _fromUserId.get(0) : -1;
        toUserId = (_toUserId.size() != 0) ? _toUserId.get(0) : -1;

        for (Integer whiteUserId : _whiteUserIds) {
            _whiteUserIdsMap.put(whiteUserId, -1);
        }
        for (Integer userIdsTail : _userIdsTail) {
            _userIdsTailMap.put(userIdsTail, -1);
        }

        whiteUserIdsMap = _whiteUserIdsMap;
        userIdsTailMap = _userIdsTailMap;

    }

    /**
     * 刷新线程
     */
    public static class LoadWikiUserIdsThread extends Thread {
        @Override
        public void run() {
            while (true) {
                try {
                    loadWikiPolicy();
                } catch (Throwable e) {
                    e.printStackTrace();
                }
                try {
                    sleep(TimeUnit.MINUTES.toMillis(LOAD_RATE_MINUTE));
                } catch (Throwable e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        if (!isThreadStart) {
            isThreadStart = true;
            loadWikiUserIdsThread.start();
        }
    }
}
