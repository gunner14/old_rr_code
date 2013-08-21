package com.renren.sns.wiki.xoa;

import java.util.List;

import com.renren.sns.wiki.xoa.WikiParserService;
import com.renren.xoa2.annotation.XoaService;

/**
 * @author yi.li@renren-inc.com since 2012-4-6
 * 
 */
@XoaService("social_wiki.sns.xoa.renren.com")
public interface IWikiParserService extends WikiParserService.Iface {

    /**
     * UGC创建时调用该服务对UGC进行切词，返回wiki ids和切词结果
     * 
     * @param context
     */
    public ParseResult parseText(ParseContext context);

    /**
     * UGC更新时调用该服务
     * 
     * @param context
     * @param privacy 隐私数据
     */
    public void asyncUpdateParseResult(ParseContext context, String privacy);

    /**
     * UGC终端页展示时如果读取miss时，则启动该服务异步加载
     * 
     * @param context
     */
    public void asyncReloadResult(ParseContext context);

    /**
     * 批量加载数据。UGC终端页展示时如果读取miss时，则启动该服务异步加载
     * 
     * @param contexts
     */
    public void asyncBatchReloadResult(List<ParseContext> contexts);
}
