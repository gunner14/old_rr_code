package com.renren.sns.minisite.feed.xoa.components.notify;

import java.util.Arrays;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import com.renren.sns.minisite.feed.xoa.model.BroadcastModel;

@Component("lsNotifyService")
public class LowSpeedNotifyService implements NotifyService {


    @Autowired
    @Qualifier("syncNotifyService")
    private NotifyService syncNotifyService;

    @Override
    public void notify(final long ugcId, final List<Integer> userIds,final BroadcastModel model) {
        for (final Integer userId : userIds) {
            final Runnable command = new Runnable() {

                @Override
                public void run() {
                    LowSpeedNotifyService.this.syncNotifyService.notify(ugcId,
                            Arrays.asList(userId),model);
                    model.incUnActiveFinished();
                }
            };
            Resource.LOW_EXECUTOR.execute(command);
        }
    }

}
