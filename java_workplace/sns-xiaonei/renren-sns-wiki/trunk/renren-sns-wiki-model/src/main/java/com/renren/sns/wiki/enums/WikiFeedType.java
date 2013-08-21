package com.renren.sns.wiki.enums;

public enum WikiFeedType {

    LATEST("latest"), HOT("hot"), FRIEND("friend");

    private String value;

    private WikiFeedType(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    /**
     * 根据name获取枚举
     * 
     * @param name
     * @return
     */
    public static WikiFeedType getEnum(String value) {
        WikiFeedType[] wikiFeedTypes = values();
        for (WikiFeedType wikiFeedType : wikiFeedTypes) {
            if (wikiFeedType.getValue().equals(value)) {
                return wikiFeedType;
            }
        }
        return null;
    }

}
