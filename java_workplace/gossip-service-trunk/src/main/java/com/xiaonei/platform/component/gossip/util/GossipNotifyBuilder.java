package com.xiaonei.platform.component.gossip.util;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.app.at.xoa.api.util.AtNotifyInfoBeanBuilder;
import com.renren.ugc.model.at.AtNotifyInfoBean;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.xce.notify.NotifyBody;

public class GossipNotifyBuilder {

    private NotifyBody mentionNotifyBody = new NotifyBody();

    private Log logger = LogFactory.getLog(this.getClass());

    private Gossip gossip;

    private User host;

    private User guest;

    private int cc;

    private int source;

    private boolean whisper;

    private List<Integer> idList;

    //@留言提醒类型 220
    public final int MENTIONNOTIFYTYPE = 220;

    public GossipNotifyBuilder(Gossip gossip, User host, User guest, int cc, int source,
            boolean whisper) {
        super();
        this.gossip = gossip;
        this.host = host;
        this.guest = guest;
        this.cc = cc;
        this.source = source;
        this.whisper = whisper;
    }

    /**
     * 获得@留言提醒NotifyBody
     * 
     * @return
     */
    public AtNotifyInfoBean getAtNotifyInfoBean() {
        AtNotifyInfoBeanBuilder builder = new AtNotifyInfoBeanBuilder();
        if (logger.isDebugEnabled()) {
            logger.debug(String.format(
                    "host.getId():%d,guest.getId():%d,cc.getId():%d,source:%d,whisper:%s", host
                            .getId(), guest.getId(), cc, source, whisper));
        }
        if (gossip != null) {
            Map<String, String> busiMap = new HashMap<String, String>();
            busiMap.put("from_name", host.getName());
            busiMap.put("whisper", getWhisperResult());
            builder.setSourceId(source).setHostId(host.getId()).setOwnerId(guest.getId()).setSchemaId(MENTIONNOTIFYTYPE)
                    .setType(MENTIONNOTIFYTYPE).setBusiParams(busiMap);
            return builder.build();
        } else {
            return null;
        }

    }

    public List<Integer> getIdList() {
        return idList;
    }

    /**
     * 获得@需要的whisper结果
     * 
     * @return
     */
    public String getWhisperResult() {
        return whisper ? "&whisper=1" : "&whisper=0";
    }
}
