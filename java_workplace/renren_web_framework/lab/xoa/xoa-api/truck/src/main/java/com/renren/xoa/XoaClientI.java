package com.renren.xoa;

import com.renren.xoa.XoaClient.XoaResponseCallback;

/**
 * XoaClient的接口
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public interface XoaClientI {

    /**
     * 提交一个XOA请求，并获取相应的调用信息
     * 
     * @param xoaMethod 请求本身
     * @param callback 回调逻辑
     * @return 调用信息
     */
    public InvocationInfo submit(Method xoaMethod, XoaResponseCallback callback);
}
