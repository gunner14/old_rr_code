package com.renren.sns.wiki.enums;

/**
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
public enum WikiType {

    MOVIE(1, "movie", "http://a.xnimg.cn/apps/wiki/cssimg/cover_movie.png",
            "http://s.xnimg.cn/apps/wiki/cssimg/head_movie.png"), // 电影 
    BOOK(2, "book", "http://a.xnimg.cn/apps/wiki/cssimg/cover_book.png",
            "http://s.xnimg.cn/apps/wiki/cssimg/head_book.png"), // 书籍
    GAME(3, "game", "http://a.xnimg.cn/apps/wiki/cssimg/cover_game.png",
            "http://s.xnimg.cn/apps/wiki/cssimg/head_game.png"), // 游戏
    PLACE(4, "place", "http://a.xnimg.cn/apps/wiki/cssimg/cover_view.png",
            "http://s.xnimg.cn/apps/wiki/cssimg/head_view.png"); // 景点

    private int wikiType;

    private String typeName;

    private String defaultHeadImage; //默认头像

    private String defaultCoverImage; //默认头图

    /**
     * @param wikiType int类型
     * @param typeName 类型的英文名
     * @param defaultCoverImage 默认头图
     * @param defaultHeadImage 默认头像
     */
    private WikiType(int wikiType, String typeName, String defaultCoverImage,
            String defaultHeadImage) {
        this.wikiType = wikiType;
        this.typeName = typeName;
        this.defaultCoverImage = defaultCoverImage;
        this.defaultHeadImage = defaultHeadImage;
    }

    public int getWikiType() {
        return this.wikiType;
    }

    public String getTypeName() {
        return this.typeName;
    }

    public String getDefaultCoverImage() {
        return this.defaultCoverImage;
    }

    public String getDefaultHeadImage() {
        return this.defaultHeadImage;
    }

    /**
     * 根据int类型的type获取到枚举类型
     * 
     * @param type
     * @return
     */
    public static WikiType getEnum(int type) {
        WikiType[] wikiTypes = values();
        for (WikiType wikiType : wikiTypes) {
            if (type == wikiType.getWikiType()) {
                return wikiType;
            }
        }
        return null;
    }

    /**
     * 根据name类型获取
     * 
     * @param type
     * @return
     */
    public static WikiType getEnumByName(String typeName) {
        WikiType[] wikiTypes = values();
        for (WikiType wikiType : wikiTypes) {
            if (typeName.equals(wikiType.getTypeName())) {
                return wikiType;
            }
        }
        return null;
    }
}
