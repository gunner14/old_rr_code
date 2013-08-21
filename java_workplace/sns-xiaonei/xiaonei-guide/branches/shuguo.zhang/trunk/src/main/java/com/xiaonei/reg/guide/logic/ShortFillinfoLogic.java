package com.xiaonei.reg.guide.logic;

import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.form.ShortFormFillInfoHsForm;
import com.xiaonei.reg.guide.form.ShortFormFillInfoMajorForm;
import com.xiaonei.reg.guide.form.ShortFormFillInfoUnivForm;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.pojo.RegUserConfigPojo;
import com.xiaonei.reg.register.pojo.Upload;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserStage;

public class ShortFillinfoLogic {

    private static final ShortFillinfoLogic _instance = new ShortFillinfoLogic();

    private static final FillInfoLogic userInfoLogic = FillInfoLogic
            .getInstance();

    public static ShortFillinfoLogic getInstance() {
        return _instance;
    }

    public void saveLocation(User host, String homeprovince,
                             String homecityName, int homecityCode) {
        userInfoLogic.saveLocation(host, homeprovince, homecityName,
                homecityCode);
    }

    public void saveStage(User host, int stage) {
        /*UserBasic userBasic = new UserBasic();
        userBasic.setId(host.getId());
        userBasic.setStage(stage);
        WUserBasicAdapter.getInstance().setUserBasic(host.getId(), userBasic);*/
        
        //2010-01-20 拆分UserBasic
        try {
			UserStage stg = SnsAdapterFactory.getUserStageAdapter().getUserStage(host.getId());
			stg.setStage(stage);
			SnsAdapterFactory.getUserStageAdapter().setUserStage(host.getId(), stg);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
    }

    public void saveHead(User host, ShortFormFillInfoMajorForm form) {
        saveHead(host, form.getTinyUrl(), form.getHeadUrl(), form.getMainUrl(),
                form.getLargeUrl());
    }

    public void saveHead(User host, ShortFormFillInfoHsForm form) {
        saveHead(host, form.getTinyUrl(), form.getHeadUrl(), form.getMainUrl(),
                form.getLargeUrl());
    }

    public void saveHead(User host, ShortFormFillInfoUnivForm form) {
        saveHead(host, form.getTinyUrl(), form.getHeadUrl(), form.getMainUrl(),
                form.getLargeUrl());
    }

    private void saveHead(User host, String tinyUrl, String headUrl,
                          String mainUrl, String largeUrl) {
        if (RegPortraitLogic.isValidUrl(largeUrl)
                && RegPortraitLogic.isValidUrl(headUrl)
                && RegPortraitLogic.isValidUrl(mainUrl)
                && RegPortraitLogic.isValidUrl(tinyUrl)) {
            Upload up = new Upload(largeUrl, mainUrl, headUrl, tinyUrl);

            RegPortraitLogic.setPortrait(host, up);
        }
    }

    public void saveUniv(User host, ShortFormFillInfoUnivForm form) {
        userInfoLogic.saveUniv(host, form.getUniversityyear(), form
                .getUniversityid(), form.getUniversityname(), form
                .getTypeOfCourse(), form.getDepartment(),
                NetworkStatus.CURRENT_NETWORK);
        userInfoLogic.saveHighSchool(host, form.getHighSchoolCode(), form
                .getHighschoolyear(), form.getHighSchoolName(), false, -1);
    }

    public void saveHs(User host, ShortFormFillInfoHsForm form) {
        userInfoLogic.saveHighSchool(host, form.getHighSchoolCode(), form
                .getHighschoolyear(), form.getHighSchoolName(), true,
                NetworkStatus.CURRENT_NETWORK);
        userInfoLogic.saveJuniorSchool(host, 0, form.getJuniorhighschoolyear(),
                form.getJuniorhighschoolname(), false, -1);
    }

    public void saveJs(User host, ShortFormFillInfoHsForm form) {
        userInfoLogic.saveJuniorSchool(host, 0, form.getJuniorhighschoolyear(),
                form.getJuniorhighschoolname(), true,
                NetworkStatus.CURRENT_NETWORK);
        userInfoLogic.saveElementarySchool(host,
                form.getElementaryschoolyear(), form.getElementaryschoolname());
    }

    public void saveTs(User host, ShortFormFillInfoHsForm form) {
        userInfoLogic.saveTechSchool(host, form.getTechSchoolCode(), form
                .getTechschoolyear(), form.getTechSchoolName(), true,
                NetworkStatus.CURRENT_NETWORK);
        userInfoLogic.saveJuniorSchool(host, 0, form.getJuniorhighschoolyear(),
                form.getJuniorhighschoolname(), false, -1);
    }

    public void setHsUc(User host) {
        RegUserConfigPojo uc = new RegUserConfigPojo(host);
        RegUserConfigLogic.getInstance().setHsMenu(uc);
    }

    public void setUnivUc(User host) {
        RegUserConfigPojo uc = new RegUserConfigPojo(host);
        RegUserConfigLogic.getInstance().setUnivMenu(uc);
    }

    public void setMajorUc(User host) {
        RegUserConfigPojo uc = new RegUserConfigPojo(host);
        RegUserConfigLogic.getInstance().setMajorMenu(uc);
    }

    public void setCompany(User host, String company) {
        userInfoLogic.saveCompany(host, company);
    }
}
