package com.renren.commons.tools.madapter.wrapper;

import com.renren.commons.tools.madapter.InvocationFailedListener;
import com.renren.commons.tools.madapter.MAdaperAgentMethod;
import com.renren.commons.tools.madapter.MAdapterFactory;
import com.xiaonei.aegis.login.client.CheckLoginClient;

public class CheckLoginClientWrapper {

    private static CheckLoginClientWrapper instance = new CheckLoginClientWrapper();

    //此方法不能是private的，cglib的原理是创建一个动态类来继承它，使用方法的覆盖
    @MAdaperAgentMethod
    protected boolean isNewIp(final int userid, final String ip) {
        //这里还可以自定义化，比如获取User对象使用缓存
        return CheckLoginClient.getInstance().isNewIp(userid, ip);
    }

    //对外使用的接口
    public static boolean isNewIpByWrap(final int userid, final String ip) {
        //final long time1 = System.currentTimeMillis();
        //final long m1 = CommonUtil.getUsedMemory();
        final CheckLoginClientWrapper clcw = MAdapterFactory.getSyncMAdapter(instance, Boolean.TRUE, 1, 3, null, new InvocationFailedListener() {

            @Override
            public void invocationFailed(final Object targetObject, final String methodName, final int failsThreshold) {
                System.out.println("InvocationFailedListener");
            }
        });
        //final long time2 = System.currentTimeMillis();
        //final long m2 = CommonUtil.getUsedMemory();
        //System.out.println("time1:" + (time2 - time1));
        //System.out.println("memory1:" + (m2 - m1));

        //final long time3 = System.currentTimeMillis();
        //final long m3 = CommonUtil.getUsedMemory();
        final boolean result = clcw.isNewIp(userid, ip);
        //final long time4 = System.currentTimeMillis();
        //final long m4 = CommonUtil.getUsedMemory();
        //System.out.println("time2:" + (time4 - time3));
        //System.out.println("memory2:" + (m4 - m3));
        return result;
    }

    public static void main(final String[] args) {
        for (int i = 0; i < 5; i++) {
            final boolean isNewIp = isNewIpByWrap(221736464, "123.125.40.250");
            System.out.println(isNewIp);
        }
    }
}
