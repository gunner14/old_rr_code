package com.renren.sns.wiki.delegator.security;

import org.springframework.stereotype.Service;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckResult;

@Service
public class AntiSpamDelegator {

    /**
     * 安全的接口进行封装
     * 
     * @param fromId
     * @param toId
     * @param checkType
     * @param content
     * @param userIp
     * @return
     */
    public CheckResult antispamFilter(int fromId, int toId, int checkType, String content,
            String userIp) {
        return AntiSpamAdapter.getInstance().antispamFilter(fromId, toId, checkType, content,
                userIp);
    }

}
