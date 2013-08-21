package com.renren.sns.wiki.biz.dic;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.BaseBiz;
import com.renren.sns.wiki.dao.dict.WikiDictionaryDAO;
import com.renren.sns.wiki.model.dict.WikiWord;

@Service
public class WikiDictionaryBiz extends BaseBiz {

    @Autowired
    private WikiDictionaryDAO wikiDictionaryDAO;

    void aa() {
        wikiDictionaryDAO.getWikiWords(0, 100);
    }

    /**
     * 根据选择条件查询数据
     * 
     * @param condition
     * @return
     */
    public List<WikiWord> getWikiWordListByCondition(String condition) {
        return wikiDictionaryDAO.getWikiWordListByCondition(condition);
    }

    /**
     * @param name
     * @return
     */
    public List<WikiWord> getWikiWordListByName(String name) {
        return wikiDictionaryDAO.getWikiWordListByName(name);
    }

    /**
     * @param name
     * @return
     */
    public List<WikiWord> getWikiWordListByWikiId(int wikiId) {
        return wikiDictionaryDAO.getWikiWordListByWikiId(wikiId);
    }

    public int addWikiWord(int wikiId, String name) {
        return wikiDictionaryDAO.addWikiWord(wikiId, name);
    }

    public int deleteWikiWord(int wikiWordId) {
        return wikiDictionaryDAO.deleteWikiWord(wikiWordId);
    }

}
