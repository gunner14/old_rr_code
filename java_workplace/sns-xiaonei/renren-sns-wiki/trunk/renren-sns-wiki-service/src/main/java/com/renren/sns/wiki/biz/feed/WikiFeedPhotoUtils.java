package com.renren.sns.wiki.biz.feed;

import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.net.URL;

import javax.imageio.ImageIO;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.model.feed.WikiFeedPhoto;

/**
 * 处理图片相关的工具类
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-17
 * 
 */
public class WikiFeedPhotoUtils {

    private static final Log logger = LogFactory.getLog(WikiFeedPhotoUtils.class);

    /**
     * 完善photo的宽高
     * @param wikiFeedPhoto
     */
    public static void completePhotoWidthHeight(WikiFeedPhoto wikiFeedPhoto) {
        String largeUrl = wikiFeedPhoto.getLargeUrl();
        if(!largeUrl.startsWith("http")){
            return;
        }
        int width = wikiFeedPhoto.getLargeWidth();
        int height = wikiFeedPhoto.getLargeHeight();
        if (width == 0 || height == 0) {
            try {
                URL url = new URL(largeUrl);
                InputStream is = url.openStream();

                BufferedImage image = ImageIO.read(is);
                width = image.getWidth();
                height = image.getHeight();
                is.close();
                wikiFeedPhoto.setLargeWidth(width);
                wikiFeedPhoto.setLargeHeight(height);
            } catch (Exception e) {
                logger.error("completeImageWidthHeight error");
            }
        }
    }
    
    /**
     * 根据参数名称返回参数值
     * 
     * @param originUrl
     * @param key
     * @return
     */
    public static String getUrlValue(String urlParams, String key) {
        if (urlParams == null) {
            return null;
        }
        if (urlParams.contains("?")) {
            int beginIndex = urlParams.indexOf("?");
            urlParams = urlParams.substring(beginIndex + 1);
        }
        urlParams = urlParams.replaceAll("&amp;", "&");

        String[] ss = urlParams.split("&");
        for (String keyValue : ss) {
            String[] entry = keyValue.split("=", 2);
            if (entry.length == 2) {
                String paramsKey = entry[0];
                String paramsValue = entry[1];
                if (paramsKey.equals(key)) {
                    return paramsValue;
                }
            }
        }
        return null;
    }

}
