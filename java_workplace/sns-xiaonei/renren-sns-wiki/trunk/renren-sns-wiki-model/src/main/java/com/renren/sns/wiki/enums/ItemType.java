package com.renren.sns.wiki.enums;

/**
 * 内容类型,对应这日志 视频 照片 状态
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 * 
 */
public enum ItemType {

    STATUS("status", 502), VIDEO("video", 110), PHOTO("photo", 701, 709), BLOG("blog", 601), ALL(
            "all", 110, 502, 601, 701, 709), WIHTOUT_STATUS("without_status", 110, 601, 701, 709);

    private String value; //默认头图

    private int[] feedTypes;

    private ItemType(String value, int... feedTypes) {
        this.value = value;
        this.feedTypes = feedTypes;
    }

    public String getValue() {
        return value;
    }

    public int[] getFeedTypes() {
        return feedTypes;
    }

    /**
     * 根据value获取枚举类型, 匹配不到类型时会返回all类型
     * 
     * @param value
     * @return
     */
    public static ItemType getItemTypeByValue(String value) {
        ItemType[] itemTypes = values();
        for (ItemType itemType : itemTypes) {
            if (itemType.getValue().equals(value)) {
                return itemType;
            }
        }
        return ALL;
    }

}
