package com.renren.sns.wiki.service.security;

import java.util.Date;

import org.springframework.stereotype.Service;

import com.xiaonei.admin.biz.model.audit.AuditReplyView;
import com.xiaonei.admin.biz.model.audit.AuditReplyView.AuditReplyType;
import com.xiaonei.admin.common.logic.audit.AuditLogic;

/**
 * 用于安全审核
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-28
 * 
 */
@Service
public class WikiAuditService {

    /**
     * 提交简评的敏感词调用这个方法
     */
    public void auditSimpleComment(int wikiId, int userId, String userName,
            String body) {

        AuditReplyView view = new AuditReplyView();

        view.setReplyId(wikiId);
        view.setContentId(wikiId);
        view.setCreatorId(userId);
        view.setCreatorName(userName);
        view.setOwnerId(userId);
        view.setOwnerName(userName);
        view.setTitle("");
        view.setBody(body);
        view.setCreateTime(new Date());
        view.setUrl("http://wiki.renren.com/" + wikiId);
        view.setType(AuditReplyType.SOCIAL_WIKI_COMMENT);

        AuditLogic.getInstance().saveAuditReply(view);

    }
}
