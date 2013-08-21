package com.xiaonei.platform.core.opt.permission.define;

import mop.hi.oce.domain.model.Relation;

/**
 * 关系定义
 * 
 * @author yunlong.bai@opi-corp.com
 * 
 */

public class RelationDef {
    
    /** 10000 交换名片,一个临时方案, '10000'>1 && '10000'<2 */
    public static final int uuExchangeCard = 10000; //10000 交换名片,一个临时方案, '10000'>1 && '10000'<2

    public static final int uuBlocked = Relation.uuBlocked; //100

    public static final int uuNoPath = Relation.uuNoPath; //99

    public static final int uuBlock = Relation.uuBlock; //90

    public static final int uuPoked = 51; // 51

    public static final int uuPoke = 50; //50

    public static final int uuSameStage = 40; //40

    public static final int uuFriendFriend = 5; //5 好友的好友

    public static final int uuRequestGuest = Relation.uuRequestGuest; //4

    public static final int uuSchoolmate = Relation.uuSchoolmate; //3 同城
    
    public static final int uuGuestRequest = Relation.uuGuestRequest; //1

    public static final int uuFriend = Relation.uuFriend; //0

    public static final int uuSelf = Relation.uuSelf; //-1

    public static final int uuAdmin = -10; // -10

    public static final int notInit = -1000; // -1000
}
