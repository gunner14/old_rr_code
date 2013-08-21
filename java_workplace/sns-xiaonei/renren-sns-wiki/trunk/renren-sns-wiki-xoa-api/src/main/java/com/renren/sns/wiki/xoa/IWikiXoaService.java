package com.renren.sns.wiki.xoa;

import com.renren.xoa2.annotation.XoaService;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-31
 * 
 */
@XoaService("social_wiki.sns.xoa.renren.com")
public interface IWikiXoaService extends WikiXoaService.Iface {

    /**
     * 清空简评
     * 
     * @param userId
     * @param wikiId
     */
    public void deleteSimpleComment(int wikiId, int userId);

    /**
     * 封禁wiki
     * 
     * @param wikiId
     */
    public boolean forbiddenWiki(int wikiId);

    /**
     * 暂停wiki
     * 
     * @param wikiId
     */
    public boolean pauseWiki(int wikiId);

    /**
     * 开启wiki
     * 
     * @param wikiId
     */
    public boolean reopenWiki(int wikiId);
    
    /**
     * 获取Wiki状态
     * 
     * @param wikiId
     */
    public WikiState getWikiState(int wikiId);
    
    /**
     * 是否wiki profile的测试用户
     */
    public WikiProfile getWikiProfile(int userId);
    

}
