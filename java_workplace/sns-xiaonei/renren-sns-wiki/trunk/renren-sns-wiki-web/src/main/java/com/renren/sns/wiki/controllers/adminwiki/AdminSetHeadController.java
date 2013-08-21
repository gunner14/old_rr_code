package com.renren.sns.wiki.controllers.adminwiki;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
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

import com.renren.sns.wiki.service.WikiRegisterService;
import com.renren.sns.wiki.service.admin.utils.HttpToolkit;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;

@Path("")
public class AdminSetHeadController {

    private static final Log logger = LogFactory.getLog(AdminSetHeadController.class);

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private WikiRegisterService wikiRegisterService;

    @Post("/setHead/${wikiId:\\d+}")
    @LoginRequired
    public String upload(Invocation inv, @Param("wikiHeadFile") MultipartFile[] files,
            @Param("wikiId") int wikiId, @Param("light") Boolean light,
            @Param("wikiType") int wikiType) {

        if (light == null) {
            light = true;
        }

        User host = hostHolder.getUser();
        try {
            String uploadResult = "";
            if (files.length > 0) {
                String res = uploadImg(files[0].getBytes(), host.getId());
                uploadResult = setHeadByUploadRes(wikiId, res);
            } else {
                uploadResult = "请选择要上传的图片";
            }
            inv.addModel("uploadResult", uploadResult);
            return "r:/adminwiki/modifyWikiHead/" + wikiId + "?light=" + light + "&wikiType="
                    + wikiType;
        } catch (Exception e) {
            logger.error("", e);
            return "@错误:" + e.getMessage();
        }
    }

    @Get("/set-head-by-url")
    @Post("/set-head-by-url")
    @LoginRequired
    public String setHeadByUrl(Invocation inv, @Param("wikiId") int wikiId,
            @Param("imageUrl") String imageUrl, @Param("light") Boolean light,
            @Param("wikiType") int wikiType) {

        try {
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
            inv.addModel("uploadResult", uploadResult);
            return "r:/adminwiki/modifyWikiHead/" + wikiId + "?light=" + light + "&wikiType="
                    + wikiType;

        } catch (Exception e) {
            logger.error("", e);
            return "@错误:" + e.getMessage();
        }
    }

    private String setHeadByUploadRes(int wikiId, String res) throws ParseException {
        JSONObject jsonObject = new JSONObject(res);
        JSONArray jsonArray = jsonObject.getJSONArray("files");

        // 三个尺寸的图片
        String largeUrl = null;
        String tinyUrl = null;
        String headUrl = null;
        if (jsonArray.length() >= 1) {
            JSONArray imageJsonArray = jsonArray.getJSONObject(0).getJSONArray("images");
            for (int i = 0; i < imageJsonArray.length(); i++) {
                JSONObject imageJsonObject = imageJsonArray.getJSONObject(i);
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
