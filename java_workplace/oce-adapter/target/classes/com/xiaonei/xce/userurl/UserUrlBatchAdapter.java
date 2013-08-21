package com.xiaonei.xce.userurl;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public final class UserUrlBatchAdapter {

    private static volatile UserUrlBatchAdapter _instance = null;

    private UserUrlTripodAdapter<DefaultUserUrlInfo> _adapter;

    private UserUrlBatchAdapter() {
        _adapter = new UserUrlTripodAdapter<DefaultUserUrlInfo>(new DefaultUserUrlFactory());
    }

    public static UserUrlBatchAdapter getInstance() {
        if (_instance == null) {
            synchronized (UserUrlBatchAdapter.class) {
                if (_instance == null) {
                    _instance = new UserUrlBatchAdapter();
                }
            }
        }
        return _instance;
    }

    /**
     * 批量取用户头像地址
     * 
     * @param ids 用户ID列表
     * @return 用户ID到UserUrlInfo的map
     */
    public Map<Integer, DefaultUserUrlInfo> multiGetUserUrl(List<Integer> ids) {
        return _adapter.multiGetUserUrl(ids);
    }

//    public static void main(String[] args) {
//        List<Integer> keys = new LinkedList<Integer>();
//        keys.add(35178);
//        keys.add(35179);
//        System.out.println(UserUrlBatchAdapter.getInstance().multiGetUserUrl(keys));
//        System.exit(0);
//    }

}
