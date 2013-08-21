package com.renren.sns.wiki.controllers.adminwiki;

import java.io.IOException;
import java.text.ParseException;
import java.util.Date;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.multipart.ByteArrayPartSource;
import org.apache.commons.httpclient.methods.multipart.FilePart;
import org.apache.commons.httpclient.methods.multipart.MultipartRequestEntity;
import org.apache.commons.httpclient.methods.multipart.Part;
import org.apache.commons.httpclient.methods.multipart.PartSource;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.multipart.MultipartFile;

import com.renren.sns.wiki.service.WikiCoverService;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;

@Path("")
public class AdminSetCoverController {

    private static final Log logger = LogFactory.getLog(AdminSetCoverController.class);

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiCoverService wikiCoverService;

    @Get("/setCover/${wikiId:\\d+}/setDefault")
    @Post("/setCover/${wikiId:\\d+}/setDefault")
    @LoginRequired
    public String setDefault(Invocation inv, @Param("wikiId") int wikiId,
            @Param("light") Boolean light, @Param("wikiType") int wikiType) {

        if (light == null) {
            light = true;
        }

        try {
            boolean result = wikiCoverService.deleteWikiCover(wikiId);
            if (!result) {
                logger.info("cover设置默认失败");
            }

            return "r:/adminwiki/modifyWikiCover/" + wikiId + "?light=" + light + "&wikiType="
                    + wikiType;
        } catch (Exception e) {
            logger.error("", e);
            return "@错误:" + e.getMessage();
        }
    }

    @Post("/setCover/${wikiId:\\d+}")
    @LoginRequired
    public String upload(Invocation inv, @Param("wikiCoverFile") MultipartFile[] files,
            @Param("wikiId") int wikiId, @Param("light") Boolean light,
            @Param("wikiType") int wikiType) {

        if (light == null) {
            light = true;
        }

        String t = CookieManager.getInstance().getCookie(inv.getRequest(), "t");

        User host = hostHolder.getUser();
        try {
            String res = uploadImg(files, host.getId(), t);

            if (logger.isDebugEnabled()) {
                logger.debug("上传图片返回值:" + res);
            }

            JSONObject jsonObject = new JSONObject(res);
            JSONArray jsonArray = jsonObject.getJSONArray("files");

            // 三个尺寸的图片
            String largeUrl = null;
            String mainUrl = null;

            int largeWidth = 0;
            int largeHeight = 0;

            if (jsonArray.length() >= 1) {
                JSONArray imageJsonArray = jsonArray.getJSONObject(0).getJSONArray("images");
                for (int i = 0; i < imageJsonArray.length(); i++) {
                    JSONObject imageJsonObject = imageJsonArray.getJSONObject(i);
                    String type = imageJsonObject.getString("type");
                    String url = "http://fmn.rrimg.com/" + imageJsonObject.getString("url");

                    if ("large".equals(type)) {
                        largeUrl = url;
                        largeWidth = imageJsonObject.getInt("width");
                        largeHeight = imageJsonObject.getInt("height");
                    } else if ("main".equals(type)) {
                        mainUrl = url;
                    }
                }
            }

            if (largeUrl != null && largeUrl.startsWith("http")) {
                boolean result = wikiCoverService.updateWikiCover(wikiId, largeUrl, largeUrl,
                        mainUrl, mainUrl, mainUrl, largeWidth, largeHeight);
                if (!result) {
                    logger.info("cover设置失败");
                }
            }

            return "r:/adminwiki/modifyWikiCover/" + wikiId + "?light=" + light + "&wikiType="
                    + wikiType;
        } catch (Exception e) {
            logger.error("", e);
            return "@错误:" + e.getMessage();
        }
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
    private String uploadImg(MultipartFile[] files, int hostId, String t) throws HttpException,
            IOException, ParseException {
        String url = "http://upload.renren.com/upload.fcgi?pagetype=socialwikihead&hostid="
                + hostId + "&uploadid=" + new Date().getTime()
                + "&tick=ugc.renren.com_socialwiki_RAdfaeRGTuyjRUj";

        PostMethod postMethod = new PostMethod(url);

        postMethod.setRequestHeader("Cookie", "t=" + t);

        Part[] parts = new Part[files.length];
        for (int i = 0; i < files.length; i++) {
            MultipartFile multipartFile = files[i];
            PartSource ps = new ByteArrayPartSource(multipartFile.getOriginalFilename(),
                    multipartFile.getBytes());
            FilePart fp = new FilePart(multipartFile.getName(), ps);
            parts[i] = fp;
        }

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
