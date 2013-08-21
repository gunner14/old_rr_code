package com.renren.sns.wiki.enums;

/**
 * 用户对某个wiki兴趣的值
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-26
 * 
 */
public enum InterestType {

    WISH("wish"), DO("do"), COLLECT("collect");

    private String value;

    private InterestType(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }
    
    public static InterestType getEnumByValue(String value) {
        InterestType[] interestTypes = values();
        for (InterestType interestType : interestTypes) {
            if (interestType.getValue().equals(value)) {
                return interestType;
            }
        }
        return null;
    }

}
