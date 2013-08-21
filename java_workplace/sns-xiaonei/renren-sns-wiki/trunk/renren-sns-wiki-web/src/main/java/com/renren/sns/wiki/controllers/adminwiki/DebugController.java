package com.renren.sns.wiki.controllers.adminwiki;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONObject;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.multipart.ByteArrayPartSource;
import org.apache.commons.httpclient.methods.multipart.FilePart;
import org.apache.commons.httpclient.methods.multipart.MultipartRequestEntity;
import org.apache.commons.httpclient.methods.multipart.Part;
import org.apache.commons.httpclient.methods.multipart.PartSource;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.JSONArray;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.sns.wiki.adapter.WikiProfileAdapter;
import com.renren.sns.wiki.controllers.ajax.BaseAjaxController;
import com.renren.sns.wiki.delegator.xce.FeedLogicDelegator;
import com.renren.sns.wiki.enums.WikiType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.model.info.Movie;
import com.renren.sns.wiki.model.info.WikiInfo;
import com.renren.sns.wiki.service.WikiFeedService;
import com.renren.sns.wiki.service.WikiRegisterService;
import com.renren.sns.wiki.service.WikiService;
import com.renren.sns.wiki.service.admin.utils.HttpToolkit;
import com.renren.sns.wiki.service.info.WikiInfoService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.core.model.User;

/**
 * 用于后台开发调试
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-23
 * 
 */
@Path("debug")
public class DebugController extends BaseAjaxController {

    private static final Log logger = LogFactory.getLog(DebugController.class);

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiService wikiService;

    @Autowired
    private WikiFeedService wikiFeedService;

    @Autowired
    private WikiInfoService wikiInfoService;

    @Autowired
    private FeedLogicDelegator feedLogicDelegator;

    @Autowired
    private WikiRegisterService wikiRegisterService;

    /**
     * 展示某个新鲜事返回的具体信息
     * 
     * @param feedIds
     * @return
     */
    @Get("feedMap")
    public String getWikiMap(@Param("feedId") long[] feedIds) {
        try {
            Feed[] feeds = wikiFeedService.getFeeds(0, feedIds);
            StringBuilder sb = new StringBuilder("@");
            if (feeds.length > 0) {
                Feed feed = feeds[0];
                sb.append(feed.getFeedType()).append("<br>\n");
                Map<String, List<String>> map = feed.getData();
                for (Entry<String, List<String>> entry : map.entrySet()) {
                    sb.append(entry.getKey()).append(":").append(entry.getValue()).append("<br>\n");
                }
            }
            return sb.toString();
        } catch (WikiException we) {
            return "@" + we.getDisplayMsg();
        } catch (Exception e) {
            logger.error("", e);
            return "@" + e.getMessage();
        }
    }

    /**
     * 展示某个新鲜事返回的具体信息
     * 
     * @param feedIds
     * @return
     */
    @Get("wikiFeed")
    public String getWikiFeed(@Param("feedId") long[] feedIds) {
        try {
            User host = hostHolder.getUser();
            Feed[] feeds = wikiFeedService.getFeeds(0, feedIds);
            Map<String, Object> map = wikiFeedService.getFeedMap(0, host, feeds);
            return "@" + JSONObject.fromObject(map);
        } catch (WikiException we) {
            return "@" + we.getDisplayMsg();
        } catch (Exception e) {
            logger.error("", e);
            return "@" + e.getMessage();
        }
    }

    @Get("feedMapById")
    public String getWikiFeedByUgc(@Param("ugcId") long ugcId, @Param("owner") int owner,
            @Param("stype") int stype) {
        long feedId = feedLogicDelegator.getFeedIdBySource(ugcId, stype, owner);

        return getWikiMap(new long[] { feedId });
    }

    /**
     * 清空缓存起来的photo的Id
     * 
     * @param ugcId
     * @param owner
     * @param stype
     * @return
     */
    @Get("feedMapById")
    public String clearWikiPhotoCache(@Param("ugcId") long ugcId, @Param("owner") int owner,
            @Param("stype") int stype) {
        long feedId = feedLogicDelegator.getFeedIdBySource(ugcId, stype, owner);

        return getWikiMap(new long[] { feedId });
    }

    @Get("getWikiIdByNameAndType")
    @Post("getWikiIdByNameAndType")
    public String getWikiIdByNameAndType(@Param("name") String name, @Param("type") int type) {

        logger.debug("名字=" + Arrays.toString(name.getBytes()) + ", type=" + type);
        List<Wiki> wikiList = wikiService.getWikiListByName(name);
        if (wikiList == null || wikiList.size() == 0) {
            return "@";
        }
        for (Wiki wiki : wikiList) {
            if (wiki.getType() == type) {
                return "@" + wiki.getId();
            }
        }
        return "@";
    }

    @Get("is-profile-user")
    public String isProfileUser(Invocation inv, @Param("id") int userId) {
        boolean result = WikiProfileAdapter.getInstance().isProfileUser(userId);
        return "@" + result;
    }

    @Get("wiki-profile-list")
    public String wikiProfileList(Invocation inv) {
        Set<Integer> set = WikiProfileAdapter.getInstance().getCacheUserIdSet();
        StringBuilder sb = new StringBuilder("@");
        for (int userId : set) {
            sb.append(userId).append(" ");
        }
        return sb.toString();
    }

    @Get("complete-movie-info")
    public String completeMovieInfo(Invocation inv, @Param("wikiId") int wikiId) {
        try {
            Wiki wiki = wikiService.getWikiById(wikiId);
            if (wiki == null) {
                return "@wiki不存在";
            }
            //获取资料
            WikiInfo wikiInfo = wikiInfoService.getWikiInfoByWiki(wiki);
            if (wikiInfo instanceof Movie) {
                Movie movie = (Movie) wikiInfo;
                String imdb = movie.getIMDb();
                if (StringUtils.isBlank(imdb)) {
                    return "@imdb为空";
                }
                if ("http://s.xnimg.cn/apps/wiki/cssimg/head_movie.png".equals(wiki.getHeadUrl())
                        || StringUtils.isBlank(movie.getReleaseDate())) {} else {
                    return "@无须完善";
                }

                //
                String searchUrl = "http://movie.douban.com/j/subject_suggest?q=" + imdb;

                String searcheRes = new HttpToolkit().get(searchUrl);

                JSONArray jsonArray = null;
                try {
                    jsonArray = new JSONArray(searcheRes);
                } catch (ParseException e) {
                    e.printStackTrace();
                    return "@豆瓣搜索返回jsonArray异常";
                }

                String imageUrl = null;
                String doubanUrl = null;

                if (jsonArray.length() > 0) {
                    org.json.JSONObject jsonObject = jsonArray.getJSONObject(0);
                    imageUrl = jsonObject.getString("img");
                    imageUrl = imageUrl.replaceAll("spic", "lpic");
                    // 如果头像不存在,设置头像
                    doubanUrl = jsonObject.getString("url");
                }
                //            System.out.println("img:" + imageUrl);
                //            System.out.println("url:" + doubanUrl);

                //
                StringBuilder sbResult = new StringBuilder("@修改头像:");
                if ("http://s.xnimg.cn/apps/wiki/cssimg/head_movie.png".equals(wiki.getHeadUrl())) {
                    //更改头像
                    InputStream in = new HttpToolkit().openInputStream(imageUrl, null, null);
                    ByteArrayOutputStream baos = new ByteArrayOutputStream();
                    byte[] bb = new byte[1024 * 8];
                    int len = 0;
                    while ((len = in.read(bb)) > 0) {
                        baos.write(bb, 0, len);
                    }
                    byte[] data = baos.toByteArray();
                    in.close();
                    baos.close();

                    User host = hostHolder.getUser();
                    String res = uploadImg(data, host.getId());
                    String uploadResult = setHeadByUploadRes(wikiId, res);
                    sbResult.append(uploadResult);
                } else {
                    sbResult.append("原来修改过头像");
                }
                sbResult.append("\n");
                sbResult.append("  完善资料:");
                if (StringUtils.isBlank(movie.getReleaseDate())) {
                    //完善资料

                    if (doubanUrl != null && doubanUrl.startsWith("http")) {
                        String url = doubanUrl;

                        String info = "";
                        String res = new HttpToolkit().get(url);

                        Document doc = Jsoup.parse(res);
                        Element infoEle = doc.getElementById("info");
                        String infoHtml = infoEle.html();

                        String[] infoHtmls = infoHtml.split("<br />");
                        StringBuilder sb = new StringBuilder();
                        for (String singleInfoHtml : infoHtmls) {
                            sb.append(Jsoup.parse(singleInfoHtml).text()).append("\n");
                        }
                        info = sb.toString();

                        //更改
                        String[] infos = info.split("[\r\n]{1,2}");
                        if (infos != null && infos.length > 0) {
                            for (String oneInfo : infos) {
                                String[] oneInfos = oneInfo.split(":\\s*", 2);
                                if (oneInfos.length == 2) {
                                    String name = oneInfos[0];
                                    String value = oneInfos[1];
                                    WikiType wikiType = WikiType.getEnum(wiki.getType());
                                    wikiInfoService.saveOrUpdateWikiInfoProperty(wikiId, name,
                                            value, wikiType);
                                }
                            }
                        }

                    }
                    sbResult.append("完毕");
                } else {
                    sbResult.append("不需要");
                }
                return sbResult.toString();
            } else {
                return "@不是电影";
            }

        } catch (Exception e) {
            logger.error("", e);
            return "@error," + e.getMessage();
        }

    }

    private String setHeadByUploadRes(int wikiId, String res) throws ParseException {
        org.json.JSONObject jsonObject = new org.json.JSONObject(res);
        org.json.JSONArray jsonArray = jsonObject.getJSONArray("files");

        // 三个尺寸的图片
        String largeUrl = null;
        String tinyUrl = null;
        String headUrl = null;
        if (jsonArray.length() >= 1) {
            JSONArray imageJsonArray = jsonArray.getJSONObject(0).getJSONArray("images");
            for (int i = 0; i < imageJsonArray.length(); i++) {
                org.json.JSONObject imageJsonObject = imageJsonArray.getJSONObject(i);
                String type = imageJsonObject.getString("type");
                String url = "http://fmn.rrimg.com/" + imageJsonObject.getString("url");

                if ("large".equals(type)) {
                    largeUrl = url;
                } else if ("head".equals(type)) {
                    headUrl = url;
                } else if ("tiny".equals(type)) {
                    tinyUrl = url;
                }
            }
        }

        boolean result = wikiRegisterService.updateWikiHead(wikiId, tinyUrl, headUrl, largeUrl,
                largeUrl);
        String uploadResult = "";
        if (result) {
            uploadResult = "修改头像成功";
        } else {
            uploadResult = "修改头像失败";
        }
        return uploadResult;
    }

    /**
     * 上传图片到图片服务器
     * 
     * @param files
     * @return
     * @throws HttpException
     * @throws IOException
     * @throws ParseException
     */
    private String uploadImg(byte[] data, int hostId) throws HttpException, IOException,
            ParseException {
        String url = "http://upload.renren.com/upload.fcgi?pagetype=wikiinput&hostid=" + hostId
                + "&uploadid=" + new Date().getTime() + "&tick=ugc.renren.com_wikiinput232Jdjl";

        PostMethod postMethod = new PostMethod(url);

        Part[] parts = new Part[1];
        String name = System.currentTimeMillis() + ".jpg";
        PartSource ps = new ByteArrayPartSource(name, data);
        FilePart fp = new FilePart(name, ps);
        parts[0] = fp;

        MultipartRequestEntity mrp = new MultipartRequestEntity(parts, postMethod.getParams());
        postMethod.setRequestEntity(mrp);

        // 执行postMethod
        HttpClient httpClient = new HttpClient();
        httpClient.executeMethod(postMethod);
        String response = postMethod.getResponseBodyAsString();
        //      JSONObject jsonObject = new JSONObject(response);
        return response;
    }

}
