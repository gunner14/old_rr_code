package com.xiaonei.platform.component.gossip.util;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.app.at.model.AtFormatType;
import com.renren.app.at.xoa.api.util.AtInfoBeanBuilder;
import com.renren.ugc.commons.tools.xoa.AtXoaClient;
import com.renren.ugc.model.at.AtInfoBean;
import com.renren.ugc.model.at.AtNotifyInfoBean;
import com.renren.xoa.jackson.ObjectMapperFactory;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.scorecache.ScoreCache;
import com.xiaonei.xce.scorecache.ScoreCacheAdapter;

/**
 * @author song.wen 留言工具
 */
public class GossipUtils {

    private static Log logger = LogFactory.getLog(GossipUtils.class);

    private static AtXoaClient client = new AtXoaClient();

    public static boolean isVip(int hostId) {
        ScoreCache cache = ScoreCacheAdapter.getInstance().getScoreData(hostId);
        if (cache != null) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 格式化@
     * 
     * @param user
     * @param body
     * @param formartType
     * @return
     */
    public static String formartAt(int user, String body, AtFormatType formartType) {
        AtInfoBean atInfoBean = generateAtInfoBean(body, user);
        atInfoBean.setFormatType(AtFormatType.NO_HREF_NAME_ID);
        return client.formartAt(atInfoBean);
    }

    /**
     * 格式化@
     * 
     * @param user
     * @param body
     * @param formartType
     * @return
     */
    public static String formartAt(String body, AtFormatType formartType) {
        AtInfoBean atInfoBean = generateAtInfoBean(body, 0);
        atInfoBean.setFormatType(formartType);
        //不校验
//        atInfoBean.setNeedCheck(false);
        return client.formartAt(atInfoBean);
    }

    /**
     * 处理留言@信息
     * 
     * @param user 当前发@用户
     * @param gossip
     */
    public static void formatGossipNoHerfWithNameAndId(int user, Gossip gossip) {
        gossip.getBody().setBody(
                formartAt(user, gossip.getBody().getBody(), AtFormatType.NO_HREF_NAME_ID));
    }

    /**
     * 发@任务
     * 
     * @param gossip
     * @param host
     * @param guest
     * @param cc
     * @param source
     * @param whisper
     */
    public static void sendGossipAtNotify(Gossip gossip, User host, User guest, int cc,
            int source, boolean whisper) {

        AtInfoBean atInfoBean = generateAtInfoBean(gossip.getBody().getBody(), host.getId());;
        AtNotifyInfoBean atNotifyInfoBean = new GossipNotifyBuilder(gossip, host, guest, cc,
                source, whisper).getAtNotifyInfoBean();
        if (logger.isInfoEnabled()) {
            try {
                logger.info(String.format("submitAtTask,params[atInfoBean:%s,atNotifyInfoBean:%s]",
                        ObjectMapperFactory.getInstance().getObjectMapper().writeValueAsString(
                                atInfoBean), ObjectMapperFactory.getInstance().getObjectMapper()
                                .writeValueAsString(atNotifyInfoBean)));
            } catch (Exception e) {
                logger.error("序列化失败:", e);
                e.printStackTrace();
            }
        }
        client.submitAtTask(atInfoBean, atNotifyInfoBean);

    }

    /**
     * 生成AtInfoBean
     * 
     * @param content
     * @param hostId
     * @return
     */
    public static AtInfoBean generateAtInfoBean(String content, int hostId) {
        AtInfoBeanBuilder builder = new AtInfoBeanBuilder().setContent(content).setHostId(hostId);
        return builder.build();
    }
}
