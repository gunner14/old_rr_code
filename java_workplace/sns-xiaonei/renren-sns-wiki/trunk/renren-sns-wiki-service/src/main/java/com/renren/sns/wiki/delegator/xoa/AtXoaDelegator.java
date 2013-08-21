package com.renren.sns.wiki.delegator.xoa;

import org.springframework.stereotype.Service;

import com.renren.app.at.model.AtFormatType;
import com.renren.app.at.xoa.api.util.AtInfoBeanBuilder;
import com.renren.ugc.commons.tools.xoa.AtXoaClient;
import com.renren.ugc.model.at.AtInfoBean;

/**
 * 解析@表情的xoa
 * 
 * @author weiwei.wang@renren-inc.com since 2012-6-27
 * 
 */
@Service
public class AtXoaDelegator {

    /**
     * 通过xoa的方式解析@的内容,推荐使用的方式,可以无需关心服务端更新情况
     * 
     * @param content
     * @return
     */
    public String parseAt(String content) {
        AtXoaClient client = new AtXoaClient();
        // 准备数据
        AtInfoBeanBuilder builder = new AtInfoBeanBuilder().setContent(content).setFormatType(
                AtFormatType.HREF_NAME);
        AtInfoBean atInfoBean = builder.build();
        // 调用转义
        String contentWithAt = client.formartAt(atInfoBean);

        return contentWithAt;
    }

}
