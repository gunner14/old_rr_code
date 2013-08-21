package com.xiaonei.reg.register.constants;

import com.dodoyo.opt.DodoyoResource;

public interface IRegCheckCodeDef {
    public static final String OK = "OK";
    // 错误信息对照表
    public static final String ERR_SYSTEM = DodoyoResource.getString("errors.system");
    public static final String ERR_REG_SYSTEM = DodoyoResource.getString("errors.reg.system") ;
    public static final String ERR_MAIL_NOT_EMPTY = DodoyoResource.getString("errors.mail.not.empty") ;
    public static final String ERR_MAIL_EXIST = DodoyoResource.getString("errors.mail.exist") ;
    public static final String ERR_MAIL_MOBILE_EXIST = DodoyoResource.getString("errors.mail.mobile.exist") ;
    public static final String ERR_MAIL_NOT_VALID = DodoyoResource.getString("errors.mail.not.valid") ;
    public static final String ERR_MAIL_NOT_QQ = DodoyoResource.getString("errors.mail.not.qq") ;
    public static final String ERR_MOBILE_NOT_VALID = DodoyoResource.getString("errors.mobile.not.valid") ;
    public static final String ERR_MOBILE_BIND = DodoyoResource.getString("errors.mobile.bind") ;
    public static final String ERR_MOBILE_NOT_EMPTY = DodoyoResource.getString("errors.mobile.not.empty") ;
    public static final String ERR_MAIL_LENGTH_EXEED = DodoyoResource.getString("errors.mail.length.exeed") ;
    public static final String ERR_PWD_NOT_EMPTY = DodoyoResource.getString("errors.pwd.not.empty") ;
    public static final String ERR_PWD_NOT_ENOUGH = DodoyoResource.getString("errors.pwd.not.enough") ;
    public static final String ERR_PWD_TOO_LONG = DodoyoResource.getString("errors.pwd.too.long") ;
    public static final String ERR_PWD_NOT_EQUAL = DodoyoResource.getString("errors.pwd.not.equal") ;
    public static final String ERR_NAME_NOT_EMPTY = DodoyoResource.getString("errors.name.not.empty") ;
    public static final String ERR_NAME_NOT_SAFE = DodoyoResource.getString("errors.name.not.safe") ;
    public static final String ERR_NAME_TOO_LONG = DodoyoResource.getString("errors.name.too.long") ;
    public static final String ERR_NAME_NOT_REAL = DodoyoResource.getString("errors.name.not.real") ;
    public static final String ERR_NAME_ILLEGLE_CHAR = DodoyoResource.getString("errors.name.illegle.char") ;
    public static final String ERR_SEX_NOT_EMPTY = DodoyoResource.getString("errors.sex.not.empty") ;
    public static final String ERR_USER_SIGNED = DodoyoResource.getString("errors.user.signed") ;
    public static final String ERR_CODE_ERR = DodoyoResource.getString("errors.code.err") ;
    public static final String ERR_NONE_STAGE = DodoyoResource.getString("errors.none.stage") ;
    public static final String ERR_INVALID_STAGE = DodoyoResource.getString("errors.invalid.stage") ;
    public static final String ERR_WORKPLACENAME_NOT_EMPTY = DodoyoResource.getString("errors.workplacename.not.empty") ;
    public static final String ERR_WORKPLACENAME_NOT_SAFE = DodoyoResource.getString("errors.workplacename.not.safe") ;

    public static final String ERR_NO_SELECT_PLACE = DodoyoResource.getString("errors.no.select.place") ;
    public static final String ERR_NO_INPUT_UNIV = DodoyoResource.getString("errors.no.input.univ") ;
    public static final String ERR_UNIV_NOT_EXIST = DodoyoResource.getString("errors.univ.not.exist") ;
    public static final String ERR_NO_INPUT_HIGHSCHOOL = DodoyoResource.getString("errors.no.input.highschool") ;
    public static final String ERR_HIGHSCHOOL_NOT_EXIST = DodoyoResource.getString("errors.highschool.not.exist") ;
    public static final String ERR_NO_INPUT_JUNIORSCHOOL = DodoyoResource.getString("errors.no.input.juniorschool") ;
    public static final String ERR_JUNIORSCHOOL_NOT_EXIST = DodoyoResource.getString("errors.juniorschool.not.exist") ;
    public static final String ERR_NO_INPUT_COLLEGE = DodoyoResource.getString("errors.no.input.college") ;
    public static final String ERR_COLLEGE_NOT_EXIST = DodoyoResource.getString("errors.college.not.exist") ;
    public static final String ERR_NO_SELECT_DEGREE = DodoyoResource.getString("errors.no.select.degree") ;
    public static final String ERR_NO_INPUT_ELEMENTARYSCHOOL = DodoyoResource.getString("errors.no.input.elementaryschool") ;

    public static final String ERR_NO_INPUT_DEPARTMENT = DodoyoResource.getString("errors.no.input.department") ;
    public static final String ERR_NO_INPUT_DORMITORY = DodoyoResource.getString("errors.no.input.dormitory") ;
    public static final String ERR_NO_INPUT_COVER = DodoyoResource.getString("errors.no.input.cover") ;
    public static final String ERR_NO_INPUT_HIGHSCHOOLCLASS = DodoyoResource.getString("errors.no.input.highschoolclass") ;
    public static final String ERR_NO_INPUT_ENROLLYEAR = DodoyoResource.getString("errors.no.input.enrollyear") ;
    public static final String ERR_FILTER_MAJIA = DodoyoResource.getString("errors.filter.majia") ;
    public static final String ERR_FILTER_MAJIA_SAMEIP = DodoyoResource.getString("errors.filter.majia.sameip") ;

    public static final String ERR_MOBILE_FORMAT = DodoyoResource.getString("errors.mobile.format") ;

    public static final String ERR_NIKNAME_FORMAT = DodoyoResource.getString("errors.nikname.format") ;
    public static final String ERR_NIKNAME_EXIST = DodoyoResource.getString("errors.nikname.exist") ;

    public static final String ERR_BAISCCONTROL_EMPTY = DodoyoResource.getString("errors.baisccontrol.empty") ;

    public static final String ERR_BIRTHDAY_EMPTY = DodoyoResource.getString("errors.birthday.empty") ;
    public static final String ERR_BIRTHDAY_ERR = DodoyoResource.getString("errors.birthday.err") ;

}