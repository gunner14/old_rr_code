package com.xiaonei.page.xoa.controllers.bizservice.doing;

import java.sql.SQLException;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.platform.component.doing.home.DoingFeedUtil;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("ddr")
public class DelDoingReplyController {

    @Autowired
    private DoingService doingService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    /**
     * 
     * @param pageId
     * @param doingId
     * @param doingReplyId
     * @param opUserId
     * 
     * @return 成功，返回空串；失败，{@link XoaBizErrorBean}
     */
    @Get
    public Object index(@Param("pageId") int pageId, @Param("doingId") int doingId,
            @Param("doingReplyId") long doingReplyId, @Param("opUserId") int opUserId) {
        try {
            //取page
            Page page = PageXoaUtil.getInstance().getPage(pageId);
            if (page == null) {
                return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                        PageXoaErrors.PAGE_NOT_EXIST.getDesc());
            }

            //取user
            User host = PageXoaUtil.getInstance().getUser(opUserId);
            if (host == null) {
                return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                        PageXoaErrors.USER_NOT_EXIST.getDesc());
            }
            
            User puser = SnsAdapterFactory.getUserAdapter().get(pageId);
            //判断权限失败
            if (!PageXoaUtil.getInstance().authUserAndPage(opUserId, pageId)) {
                return new XoaBizErrorBean(PageXoaErrors.AUTH_FAILED.getType(),
                        PageXoaErrors.AUTH_FAILED.getDesc());
            }else{//以page所有者的身份执行删除
                host = puser;
            }

            // 删除回复表的对应记录
            if (doingService.delReplyDoingWithLog(doingReplyId, doingId, puser.getId(),
                    host.getId(), FeedDefinition.PAGE_STATUS_UPDATE, null) > 0) {
                try {
                    //TODO 删除状态要发新鲜事吗？
                    this.publishFeed(page, doingReplyId, doingId, puser);
                } catch (SQLException e) {
                    logger.error(e.getMessage(), e);
                }
                return "@";
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }

        return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
    }

    private void publishFeed(Page page, long replyId, int doingId, User puser) throws SQLException {
        Doing doing = DoingHome.getInstance().getDoingById(doingId, page.getId());
        // 发feed，同步回复列表，只给page发吧
        logger.info(String.format("publish del status reply feed:page=%s,replyId=%s,doingId=%s",
                page.getId(), replyId, doingId));
        DoingFeedUtil.deleteDoingFeedReply(puser, doing, replyId,
                (int) FeedDefUtil.getSTATUS_UPDATE_REPLY_Def(page));
    }
}
