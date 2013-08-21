package com.renren.sns.wiki.stat.importer;

/**
 * 用来计数
 * 
 * @author yi.li@renren-inc.com since 2012-6-27
 * 
 */
public class Counter {

    private int photo;

    private int album;

    private int video;

    private int blog;

    private static final int MAX_COUNT = 300;

    public void doCount(int type) {
        switch (type) {
            case 601:
                blog++;
                break;
            case 701:
                photo++;
                break;
            case 709:
                album++;
                break;
            case 110:
                video++;
                break;
            default:
                break;
        }
    }

    public boolean needSweep(int stype) {
        int count = 0;

        switch (stype) {
            case 601:
                count = blog;
                break;
            case 701:
                count = photo;
                break;
            case 709:
                count = album;
                break;
            case 110:
                count = video;
                break;
            default:
                break;
        }

        if (count >= MAX_COUNT) {
            return true;
        }
        return false;
    }

    public int getPhoto() {
        return photo;
    }

    public int getAlbum() {
        return album;
    }

    public int getVideo() {
        return video;
    }

    public int getBlog() {
        return blog;
    }
}
