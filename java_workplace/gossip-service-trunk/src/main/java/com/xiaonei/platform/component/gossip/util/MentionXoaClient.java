package com.xiaonei.platform.component.gossip.util;

import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.app.at.xoa.api.AtService;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFuture;

/**
 * @ XOA服务client
 * 
 * @author 温松[wensong1987@gmail.com]
 * @since 2011-5-19 下午02:40:51
 */
public class MentionXoaClient {

    private static MentionXoaClient client = new MentionXoaClient();

    private Log logger = LogFactory.getLog(this.getClass());

    private AtService atService;

    private MentionXoaClient() {
        atService = ServiceFactories.getFactory().getService(AtService.class);
    }

    public static MentionXoaClient getInstance() {
        return client;
    }

    /**
     * @计数
     * @param host
     * @param list
     */
    public void countMention(int host, List<Integer> list) {
        try {
            ServiceFuture<String> future = atService.addAt(host, list);
            //      future.setParam("gid", gid+""); //备用
            future.submit();
        } catch (Exception e) {
            logger.error(String.format("发@计数出错:%s", e.getMessage()));
        }

    }

}
