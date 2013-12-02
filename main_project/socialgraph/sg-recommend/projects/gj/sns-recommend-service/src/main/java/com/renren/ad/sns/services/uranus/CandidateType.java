package com.renren.ad.sns.services.uranus;


public enum CandidateType {
    CategoryCandidatePicker(1), 
    CFCandidatePicker(2),
    HotGoods(3),
    UnKnown(-1);

    private int value;

    CandidateType(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public static CandidateType getCandidateType(int value) {
        for (CandidateType ct : CandidateType.values()) {
            if (ct.getValue() == value) {
                return ct;
            }
        }
        return null;
    }
}
