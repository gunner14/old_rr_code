package com.renren.sns.minisite.feed.xoa.controllers;

import java.util.Arrays;
import java.util.List;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.minisite.feed.xoa.services.fetch.FeedsFetchService;
import com.renren.sns.minisite.feed.xoa.services.publish.FeedsPublishService;
import com.renren.sns.minisite.feeds.xoa.client.MinisiteNewFeedsService;
import com.renren.xoa.XoaClient;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;
import com.renren.xoa.lite.ServiceFutureListener;
import com.renren.xoa.registry.impl.SimpleXoaRegistry;

@Path("msfeeds")
public class MiniSiteFeedsController {

    private final static SimpleXoaRegistry reg = new SimpleXoaRegistry();
    static {
        reg.register("newfeeds.zhan.xoa.renren.com", "10.3.19.115", 8188);
    }

    @Autowired
    private FeedsFetchService feedsFetchService;

    @Autowired
    private FeedsPublishService feedsPublishService;

    private final Log logger = LogFactory.getLog(this.getClass());

    @Get("getnewsfeed")
    public long[] getNewsFeed(@Param("userid") final int userid, @Param("page") final int page) {
        final int cpage = page < 0 ? 0 : page;

        try {
            long t1 = System.currentTimeMillis();
            final List<Long> feeds = this.feedsFetchService.getFeeds(userid, cpage);
            if (this.logger.isInfoEnabled()) {
                this.logger.info("MiniSiteFeedsController.getNewsFeed\t" + userid + "\t" + page
                        + "\t" + feeds.size() + "\t" + (System.currentTimeMillis() - t1));
            }
            return ArrayUtils.toPrimitive(feeds.toArray(new Long[0]));
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return new long[0];

        /*
        //从新版新鲜事系统读，可适当开放用户
        try {
        	List<Long> feeds = null;
        	if(userid == 242412372|| userid == 24260569){
        		System.out.println("======"+userid+" come in =======");
        		final ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        		final MinisiteNewFeedsService service = fact
        				.getService(MinisiteNewFeedsService.class);//通过ServiceFactory来获取具体了Service实例
        		final ServiceFuture<long[]> future = service.fetchFeeds(userid, cpage);
                final long[] ugcids = ServiceFutureHelper.executeWithThroughputControl(future, "fetch-new", 30, 500);
                if(!ArrayUtils.isEmpty(ugcids))
                	feeds = Arrays.asList(ArrayUtils.toObject(ugcids));
                System.out.println("======"+userid+" |"+ feeds.size());
        	}
        	else{
        		feeds = this.feedsFetchService.getFeeds(userid, cpage);
        	}
            //final List<Long> feeds = this.feedsFetchService.getFeeds(userid, cpage);
            return ArrayUtils.toPrimitive(feeds.toArray(new Long[0]));
        } catch (final Exception e) {
            e.printStackTrace();
        }
        return new long[0];
        */

    }

    @Post("publish")
    public void publish(@Param("siteid") final int siteid, @Param("ugcid") final long ugcid) {

        this.feedsPublishService.publish(siteid, ugcid);

        //双写，新版新鲜事系统
        try {
            final XoaClient client = new XoaClient(MiniSiteFeedsController.reg);
            final ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
            final MinisiteNewFeedsService service = fact.getService(MinisiteNewFeedsService.class,
                    client);//通过ServiceFactory来获取具体了Service实例
            final ServiceFuture<Void> future = service.publishFeed(siteid, ugcid);
            /*future.addListener(new ServiceFutureListener() {
            	@Override
            	public void operationComplete(final ServiceFuture<?> future)
            			throws Exception {
            		future.getContent();
            	}
            });*/
            future.submit();
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }
    }
}
