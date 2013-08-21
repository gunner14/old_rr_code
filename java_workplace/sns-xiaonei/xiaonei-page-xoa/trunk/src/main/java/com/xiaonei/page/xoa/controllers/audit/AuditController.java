package com.xiaonei.page.xoa.controllers.audit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.audit.AuditAcceptEnum;
import com.xiaonei.page.service.AdminService;
import com.xiaonei.page.service.extract.PageBlogBizService;
import com.xiaonei.page.service.extract.PageDoingBizService;
import com.xiaonei.page.service.extract.PagePhotoDescBizService;
import com.xiaonei.page.service.extract.PageShareBizService;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

/**
 * 为安全提供的接口
 * 
 * @author yi.li@renren-inc.com since 2011-11-28
 * 
 */
@Path("")
public class AuditController {

    private Log logger = LogFactory.getLog(AuditController.class);

    @Autowired
    private AdminService adminService;

    @Autowired
    private PageDoingBizService doingBizService;

    @Autowired
    private PageBlogBizService blogBizService;

    @Autowired
    private PagePhotoDescBizService photoDescService;

    @Autowired
    private PageShareBizService shareBizService;

    @Post("delAdmin")
    public Object delAdminForPage(@Param("pid") int pageId, @Param("uid") int userId) {
        try {
            adminService.delAdmin(pageId, userId);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /**
     * 状态先审后发审核通过后执行回调
     * 
     * @param auditId 先审后发记录id
     * @param pageId pageId
     * @return
     */
    @Post("doing/accept")
    public boolean postDoingAfterAudit(@Param("content-id") long contentId,
            @Param("assist-id") long assistId, @Param("page-id") int pageId) {
        try {
            if (logger.isDebugEnabled()) {
                logger.debug(String.format(
                        "call back for doing audit first content_id=%d assist_id=%d page_id=%d",
                        contentId, assistId, pageId));
            }
            doingBizService.handleAuditFirstCallBack(assistId, AuditAcceptEnum.ACCEPT);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }

    /**
     * 日志先审后发审核通过后执行回调
     * 
     * @param auditId 先审后发记录id
     * @param pageId page id
     * @return
     */
    @Post("blog/accept")
    public boolean postBlogAfterAudit(@Param("content-id") long contentId,
            @Param("assist-id") long assistId, @Param("page-id") int pageId) {
        try {
            if (logger.isDebugEnabled()) {
                logger.debug(String.format(
                        "call back for blog audit first content_id=%d assist_id=%d page_id=%d",
                        contentId, assistId, pageId));
            }
            blogBizService.handleAuditFirstCallBack(assistId, AuditAcceptEnum.ACCEPT);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }

    /**
     * 照片描述先审后发审核通过后执行回调
     * 
     * @param auditId 先审后发记录id
     * @param pageId page id
     * @return
     */
    @Post("photo/desc/accept")
    public boolean postPhotoDescAfterAudit(@Param("content-id") long contentId,
            @Param("assist-id") long assistId, @Param("page-id") int pageId) {
        try {
            if (logger.isDebugEnabled()) {
                logger.debug(String
                        .format("call back for photo description audit first content_id=%d assist_id=%d page_id=%d",
                                contentId, assistId, pageId));
            }
            photoDescService.handleAuditFirstCallBack(assistId, AuditAcceptEnum.ACCEPT);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }

    /**
     * 日志先审后发审核通过后执行回调
     * 
     * @param auditId 先审后发记录id
     * @param pageId page id
     * @return
     */
    @Post("share/accept")
    public boolean postShareAfterAudit(@Param("content-id") long contentId,
            @Param("assist-id") long assistId, @Param("page-id") int pageId) {
        try {
            if (logger.isDebugEnabled()) {
                logger.debug(String.format(
                        "call back for doing audit first content_id=%d assist_id=%d page_id=%d",
                        contentId, assistId, pageId));
            }
            shareBizService.handleAuditFirstCallBack(assistId, AuditAcceptEnum.ACCEPT);
        } catch (Exception e) {
            logger.error("", e);
            return false;
        }
        return true;
    }
}
