package com.xiaonei.page.xoa.controllers;

import java.util.Date;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.PageServiceHome;
import com.xiaonei.page.util.Keys;
import com.xiaonei.page.util.MemcachedUtil;
import com.xiaonei.page.xoa.task.AsynTask;
import com.xiaonei.page.xoa.task.AsynTaskExcutor;
import com.xiaonei.page.xoa.task.CallBack;
import com.xiaonei.page.xoa.task.LogExceptionCallBack;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.component.xfeed.remove.XFeedRemover;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("dd")
public class DelDoingController {
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");
	
	@Autowired
	private PageServiceHome pageServiceHome;
	
	@Autowired
	private PageService pageService;
	
    @Autowired
    private DoingService doingService;
    
    @Autowired
    private AuthService authService;
    
    /**
     * TODO 这个方法太慢了！！！
     * @param pageId
     * @param doingId
     * @param executorId
     * @return
     */
	@Get
	@Post
	public String index(@Param("pageId")int pageId,@Param("doingId")long doingId,@Param("executorId")int executorId){
        Page page = null;
        try {
            page = pageService.get(pageId);
        } catch (PageException e) {
            logger.error(String.format("get page fail: %d ", pageId), e);
            return "@GET_PAGE_FAILED";
        }

        User host = WUserAdapter.getInstance().get(executorId);
        // 没有权限
        try {
            if (!authService.isPageAdministrator(host, pageId)) {
                logger.info(String.format("%d not admin of page: %d", host.getId(), pageId));
                return "@NOT_PAGE_ADMIN";
            }
        } catch (PageException e) {
            logger.error("DB error ", e);
            return "@DB_ERROR";
        }

        try {
            if (host == null) {
                logger.error("The host is null.");
                return "@USER_NOT_EXIST";
            }
            logger.info("begin do del doing task");
            doingService.deleteDoingWithLog(page, doingId, host, null);
            logger.info("after do del doing task");
            
			MemcachedUtil.delete(Keys.DOING_COUNT_INTER, page.getId());
			pageServiceHome.deleteDoingCache(page.getId());//删缓存
            XFeedRemover.getInstance().removeFeedBySource(doingId, FeedDefinition.PAGE_STATUS_UPDATE);
            
//            //同步数据  异步执行任务
//            AsynTaskExcutor.getInstance().execute(new DelDoingAysnTask(page,doingId), 
//            		new LogExceptionCallBack());
//            logger.info("not asyn?");
            
        } catch (Exception e) {
            logger.error(e);
            return "@DEL_DOING_FAILED";
        }
        return "@true";
	}
	
	class DelDoingAysnTask implements AsynTask{
		private Page page;
		private long doingId;
		
		public DelDoingAysnTask(Page page, long doingId) {
			this.page = page;
			this.doingId = doingId;
		}
		@Override
		public void process(CallBack callBack) {
			
			try{
				MemcachedUtil.delete(Keys.DOING_COUNT_INTER, page.getId());
				pageServiceHome.deleteDoingCache(page.getId());//删缓存
	            XFeedRemover.getInstance().removeFeedBySource(doingId, FeedDefinition.PAGE_STATUS_UPDATE);
			}catch (Exception e) {
				String msg="asyn task failed for sync data!In DelDoingController at "+new Date();
				callBack.exceptionCall(e, msg);
			}
			

		}
		
	}
}
