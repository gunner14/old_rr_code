package com.renren.sns.wiki.stat.service.fingerprint;

import java.awt.Graphics2D;
import java.awt.color.ColorSpace;
import java.awt.image.BufferedImage;
import java.awt.image.BufferedImageOp;
import java.awt.image.ColorConvertOp;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 用类Hash的方式计算图像指纹
 * 
 * @author yi.li@renren-inc.com since Jul 24, 2012
 * 
 */
public class ImgHashFingerPrintStrategy implements IdentityStrategy {

    private Log logger = LogFactory.getLog(ImgHashFingerPrintStrategy.class);

    private ImageFingerPrintDict dict;

    private Map<Long, ImgDebugInfo> picMap = new HashMap<Long, ImgDebugInfo>();

    private static Map<Integer, String> significantMap = new HashMap<Integer, String>();

    static {
        significantMap.put(110, "coverImages.url");
        significantMap.put(701, "coverImages.url");
        significantMap.put(709, "coverImages.url");
    }

    public ImgHashFingerPrintStrategy() {
        String diffstr = System.getProperty("difflevel");

        int diffLevel = 10;
        if (!StringUtils.isEmpty(diffstr)) {
            diffLevel = Integer.parseInt(diffstr);
        }
        dict = new ImageFingerPrintDict(diffLevel);
    }

    @Override
    public boolean isDuplicate(int wikiId, int feedType, Map<String, Object> feed) {

        String picUrl = FingerPrintUtil.getSignificantString(significantMap, feed);
        long feedId = FingerPrintUtil.getFeedIdFromWikiFeed(feed);

        try {
            long fingerPrint = caculateImageFingerPrint(picUrl);
            if (logger.isDebugEnabled()) {
                if (!picMap.containsKey(fingerPrint)) {
                    ImgDebugInfo imgDebug = new ImgDebugInfo(feedId, picUrl);
                    picMap.put(fingerPrint, imgDebug);
                }
            }

            Long existsPrint;
            if (feedType == 110) {
                existsPrint = dict.checkDuplicateAndSet(wikiId + "" + feedType, fingerPrint);
            } else {
                existsPrint = dict.checkDuplicateAndSet(wikiId + "", fingerPrint);
            }

            if (null != existsPrint) {
                if (logger.isDebugEnabled()) {
                    ImgDebugInfo debugImg = picMap.get(existsPrint);
                    logger.debug(String
                            .format("find duplicated img exsits: %s e-feedId: %d current: %s wikiId: %d feedId: %d",
                                    debugImg.getUrl(), debugImg.getFeedId(), picUrl, wikiId, feedId));
                }

                return true;
            }
        } catch (IOException e) {
            logger.error(String.format(
                    "caculate img finger print error, url: %s wikiId: %d feed: %s", picUrl, wikiId,
                    feed), e);
        }
        return false;
    }

    /**
     * 计算图片的指纹
     * <p>
     * 拿到图片之后先进行缩放，缩放成一个8*8的图片然后再对图片颜色进行灰度处理。<br>
     * 计算出平均RGB值，用缩略图的每一个像素的RGB值与平均比较,大于平均值的取1小于平均值的取0<br>
     * 得到一个64位的long型数据，也就是图片指纹
     * 
     * @param imgUrl 图片url
     * @return long 图片指纹
     * @throws IOException
     */
    private long caculateImageFingerPrint(String imgUrl) throws IOException {
        URL url = new URL(imgUrl);
        InputStream is;
        is = url.openStream();
        BufferedImage origin = ImageIO.read(is);

        int width = 8;
        int height = 8;
        BufferedImage resized = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);

        Graphics2D graphics = resized.createGraphics();
        graphics.drawImage(origin, 0, 0, width, height, null);

        BufferedImageOp colorOp = new ColorConvertOp(ColorSpace.getInstance(ColorSpace.CS_GRAY),
                null);
        resized = colorOp.filter(resized, null);
        //                ImageIO.write(resized, "JPG",
        //                        new File("/Users/ivanlee/Documents/Temp/resized." + imgUrl.hashCode() + ".jpg"));

        int avgRGB = getAvgRGB(resized);
        long hash = caculateHash(avgRGB, resized);

        return hash;
    }

    /**
     * 获取图片的平均RGB值
     * 
     * @param resizedImg 缩成8*8的图片
     * @return 平均RGB值
     * @throws IOException
     */
    private int getAvgRGB(BufferedImage resizedImg) throws IOException {
        int sumReversed = 0;
        // caculate finger print
        for (int width = 0; width < 8; width++) {
            for (int height = 0; height < 8; height++) {
                int rgb = resizedImg.getRGB(width, height);
                sumReversed += rgb;
            }
        }

        int avgRGB = sumReversed / 64;
        return avgRGB;
    }

    /**
     * 计算图片指纹
     * 
     * @param avgRGB 图片平均RGB值
     * @param resizedImage 缩成8*8大小的图片
     * @return long 图片指纹
     */
    private long caculateHash(int avgRGB, BufferedImage resizedImage) {
        long fingerPrint = 0L;
        for (int width = 0; width < 8; width++) {
            for (int height = 0; height < 8; height++) {
                int rgb = resizedImage.getRGB(width, height);
                if (rgb >= avgRGB) {
                    int offset = width * 8 + height;
                    fingerPrint += (1L << offset);
                }
            }
        }

        if (logger.isDebugEnabled()) {
            logger.debug(Long.toBinaryString(fingerPrint));
        }

        return fingerPrint;
    }

    @Override
    public String getStrategyName() {
        return "ImageHashFingerPrintStrategy";
    }

    @Override
    public boolean supportFeedType(int feedType) {
        if (feedType == 701 || feedType == 709 || feedType == 110) {
            return true;
        }
        return false;
    }

    public static class ImgDebugInfo {

        private long feedId;

        private String url;

        public ImgDebugInfo(long feedId, String url) {
            this.feedId = feedId;
            this.url = url;
        }

        public long getFeedId() {
            return feedId;
        }

        public String getUrl() {
            return url;
        }

    }

    public static void main(String args[]) {
        ImgHashFingerPrintStrategy imgHash = new ImgHashFingerPrintStrategy();
        ImageFingerPrintDict dict = new ImageFingerPrintDict(10);
        try {
            long a = imgHash
                    .caculateImageFingerPrint("http://fmn.rrfmn.com/fmn058/20120529/2125/original_59fA_23e700000c56125f.jpg");
            long b = imgHash
                    .caculateImageFingerPrint("http://fmn.rrimg.com/fmn044/20100319/1120/p_large_I6kh_336100003f9f2d14.jpg");
            System.out.println("Diff: " + dict.getDiffPrintLevel(a, b));
            System.out.println(a ^ b);
            System.out.println("Is equal: " + (a == b) + " a: " + a + " b: " + b);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
