package com.renren.sns.wiki.service.dic;

import java.util.Collections;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.dic.WikiDictionaryBiz;
import com.renren.sns.wiki.model.dict.WikiWord;

@Service
public class WikiDictionaryService {

    private static final Log logger = LogFactory.getLog(WikiDictionaryService.class);

    @Autowired
    private WikiDictionaryBiz wikiDictionaryBiz;

    /**
     * 根据id升序
     * 
     * @param curPage
     * @param endId
     * @param page
     * @param pageSize
     * @return
     */
    public List<WikiWord> getWikiWordList(int curPage, int endId, int page, int pageSize) {
        return getWikiWordList(curPage, endId, page, pageSize, null);
    }

    /**
     * @param name
     * @return
     */
    public List<WikiWord> getWikiWordListByName(String name) {
        return wikiDictionaryBiz.getWikiWordListByName(name);
    }

    public List<WikiWord> getWikiWordListById(int wikiId) {
        return wikiDictionaryBiz.getWikiWordListByWikiId(wikiId);
    }

    public List<WikiWord> getWikiWordList(int curPage, int endId, int page, int pageSize,
            String order) {
        //默认升序
        if (order == null) {
            order = "asc";
        }

        int flag = 1;
        if (page <= curPage) {
            flag *= -1;
        }
        if (order.equals("desc")) {
            flag *= -1;
        }

        //查询条件是否包含等号
        String eq = "";
        if (page <= curPage) {
            eq = "=";
        }
        int begin = 0; //起始值
        int limit = pageSize;
        if (page > curPage) {
            begin = (page - curPage - 1) * pageSize;
        } else {
            begin = (curPage - page) * pageSize;
        }

        StringBuilder sb = new StringBuilder();
        if (endId > 0) {
            if (flag > 0) {
                //where 大于 id>endId order by id asc limit 
                sb.append(" where wiki_id >").append(eq).append(" ").append(endId);
                sb.append(" order by wiki_id asc limit ").append(begin).append(",").append(limit);
            } else {
                sb.append(" where wiki_id <").append(eq).append(" ").append(endId);
                sb.append(" order by wiki_id desc limit ").append(begin).append(",").append(limit);
            }
        } else {
            sb.append(" order by wiki_id " + order + " limit ").append(begin).append(",").append(
                    limit);
        }
        if (logger.isDebugEnabled()) {
            logger.debug("condition:" + sb.toString());
        }
        List<WikiWord> wikiWordList = wikiDictionaryBiz.getWikiWordListByCondition(sb.toString());

        if (logger.isDebugEnabled()) {
            if (wikiWordList != null) {
                logger.debug("wikiWordList.size()=" + wikiWordList.size());
            } else {
                logger.debug("wikiWordList is null");
            }
        }

        //page<=curPage时需要倒序
        if (endId > 0 && page <= curPage && wikiWordList != null && wikiWordList.size() > 0) {
            Collections.reverse(wikiWordList);
        }
        return wikiWordList;
    }

    /**
     * 到词库添加词语
     * 
     * @param wikiId
     * @param name
     */
    public int addWikiWord(int wikiId, String name) {
        return wikiDictionaryBiz.addWikiWord(wikiId, name);
    }

    /**
     * 删除
     * @param wikiWordId
     * @return
     */
    public int deleteWikiWord(int wikiWordId) {
        return wikiDictionaryBiz.deleteWikiWord(wikiWordId);
    }

}
