package com.xiaonei.page.xoa.service.xfeed.comment;

import java.util.Map;

import com.renren.app.status.xoa.api.StatusService;
import com.renren.xoa.lite.ServiceFactories;
import com.xiaonei.page.util.FeedDefUtil;


public class DoingCommentResovler implements IResolveSource {

    @Override
    public int supportedFeedType() {
        return (int)FeedDefUtil.PAGE_DOING_COMMENT;
    }

    @Override
    public Map<String, String> resolveSource(long sourceId, int ownerId) throws Exception {
        StatusService service = ServiceFactories.getFactory().getService(StatusService.class);
        service.getDoingById(ownerId, sourceId);
        return null;
    }

}
