package com.renren.sns.wiki.enums;

/**
 * 运营首页的类型
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
public enum OperateIndexType {

    LATEST("latest"), HOT("hot");

    private String value;

    private OperateIndexType(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    public static OperateIndexType getEnum(String value) {
        OperateIndexType[] operateIndexTypes = values();
        for (OperateIndexType operateIndexType : operateIndexTypes) {
            if (operateIndexType.getValue().equals(value)) {
                return operateIndexType;
            }
        }
        return null;
    }

}
