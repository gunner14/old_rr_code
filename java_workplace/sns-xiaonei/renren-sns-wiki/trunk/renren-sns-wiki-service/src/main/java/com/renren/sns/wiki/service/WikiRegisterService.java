package com.renren.sns.wiki.service;

import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.WikiBiz;
import com.renren.sns.wiki.biz.info.WikiInfoBiz;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.info.Book;
import com.renren.sns.wiki.model.info.Movie;
import com.renren.sns.wiki.model.info.WikiInfoProperty;

/**
 * 和Wiki创建相关的服务
 * 
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
@Service
public class WikiRegisterService extends BaseService {

    @Autowired
    private WikiBiz wikiBiz;

    @Autowired
    private WikiInfoBiz wikiInfoBiz;

    /**
     * 简单的根据Wiki名字和类型来创建一个Wiki, 会根据不同的规则进行排重
     * 
     * @param name
     * @return 返回创建好的Wiki对象
     */
    public Wiki createWiki(String name, WikiType wikiType, String isbn, String imdb) {
        logger.warn(String.format("创建wiki,name=%s, wikiType=%d, isbn=%s, imdb=%s", name, wikiType
                .getWikiType(), isbn, imdb));
        //判断wiki是否存在
        List<Integer> idList = wikiBiz.getWikiIdList(name, wikiType.getWikiType());
        if (idList.size() > 0) {
            if (wikiType == WikiType.BOOK) {
                if (StringUtils.isBlank(isbn)) {
                    throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "isbn不能为空");
                }

                for (int wikiId : idList) {
                    List<WikiInfoProperty> propertyList = wikiInfoBiz
                            .getPropertyListByWikiId(wikiId);
                    Book bean = (Book) wikiInfoBiz.createWikiInfo(wikiId, wikiType, propertyList);
                    if (bean != null && isbn.equals(bean.getIsbn())) {
                        throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "名字为 " + name
                                + ", isbn为" + isbn + " 的图书已经存在");
                    }
                }
            } else if (wikiType == WikiType.MOVIE) {

                if (StringUtils.isBlank(imdb)) {
                    throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "imdb不能为空");
                }

                for (int wikiId : idList) {
                    List<WikiInfoProperty> propertyList = wikiInfoBiz
                            .getPropertyListByWikiId(wikiId);
                    Movie bean = (Movie) wikiInfoBiz.createWikiInfo(wikiId, wikiType, propertyList);
                    if (logger.isDebugEnabled()) {
                        logger.debug("wikiId=" + wikiId + ", bean.getIMDb()=" + bean.getIMDb());
                    }
                    if (bean != null && imdb.equals(bean.getIMDb())) {
                        throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "名字为" + name
                                + ", IMDb为" + imdb + " 的电影已经存在");
                    }
                }

            } else {
                throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "名字为" + name
                        + "的WIKI已经存在");
            }
        }

        try {
            Wiki wiki = buildWiki(name, wikiType);
            int wikiId = wikiBiz.createWiki(wiki);
            wiki.setId(wikiId);

            if (wikiType == WikiType.BOOK && isbn != null) {
                WikiInfoProperty wikiInfoProperty = new WikiInfoProperty();
                wikiInfoProperty.setWikiId(wiki.getId());
                wikiInfoProperty.setName("isbn");
                wikiInfoProperty.setValue(isbn);
                wikiInfoBiz.saveOrUpdateWikiInfoProperty(wikiInfoProperty, wikiType);
            }

            if (wikiType == WikiType.MOVIE && imdb != null) {
                WikiInfoProperty wikiInfoProperty = new WikiInfoProperty();
                wikiInfoProperty.setWikiId(wiki.getId());
                wikiInfoProperty.setName("imdb");
                wikiInfoProperty.setValue(imdb);
                wikiInfoBiz.saveOrUpdateWikiInfoProperty(wikiInfoProperty, wikiType);
            }

            return wiki;
        } catch (Exception e) {
            logger.error("create wiki error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    /**
     * 修改wiki的头像
     * 
     * @param wikiId
     * @param headUrl
     */
    public boolean updateWikiHead(int wikiId, String tinyUrl, String headUrl, String mainUrl,
            String largeUrl) {
        try {
            logger.info(String.format("wikiId=%d, tinyUrl=%s, headUrl=%s, mainUrl=%s, largeUrl=%s",
                    wikiId, tinyUrl, headUrl, mainUrl, largeUrl));
            return wikiBiz.updateWikiHead(wikiId, tinyUrl, headUrl, mainUrl, largeUrl);
        } catch (Exception e) {
            logger.error("updateWikiHead error", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, e);
        }
    }

    private Wiki buildWiki(String name, WikiType wikiType) {
        Wiki wiki = new Wiki();
        wiki.setName(name);
        wiki.setType(wikiType.getWikiType());
        wiki.setTinyUrl(wikiType.getDefaultHeadImage());
        wiki.setHeadUrl(wikiType.getDefaultHeadImage());
        wiki.setMainUrl(wikiType.getDefaultHeadImage());
        wiki.setLargeUrl(wikiType.getDefaultHeadImage());

        return wiki;
    }
}
