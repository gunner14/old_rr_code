package com.xiaonei.reg.guide.logic.mobile;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.model.College;
import com.xiaonei.platform.core.model.HighSchool;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.form.MobileBLFillInfoForm;
import com.xiaonei.reg.guide.form.MobileDXFillInfoForm;
import com.xiaonei.reg.guide.form.MobileGZFillInfoForm;
import com.xiaonei.reg.guide.logic.FillInfoLogic;
import com.xiaonei.reg.register.dao.CollegeDAO;
import com.xiaonei.reg.register.logic.pretreatment.validate.Antispam;

/**
 * MobileFillinfoLogic
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:16:12
 */
public class MobileFillinfoLogic {
    /**
     * 检查正确
     */
    public static final String CHECK_OK = null;

    private static final String CHECK_GENDER = "请填写性别";

    private static final String CHECK_LOCATION = "请填写居住地";

    private static final String CHEKC_COMPANY_NOT_SAFE = "公司名称包含不恰当内容，请检查";

    private static final String CHECK_HOMETOWN = "请填写家乡";

    private static final String CHECK_JUNIOR = "请填写初中";

    private static final String CHECK_JUNIOR_ENROLLYEAR = "请填写初中入学年";

    private static final String CHECK_ELEMENTARY = "请填写小学";

    private static final String CHECK_ELEMENTARY_ENROLLYEAR = "请填写小学入学年";

    private static final String CHECK_HIGH_ENROLLYEAR = "请填写高中入学年";

    private static final String CHECK_HIGH = "请填写高中";

    private static final String CHECK_TECH = "请填写学校";

    private static final String CHECK_TECH_ENROLLYEAR = "请填写学校入学年";

    private static final String CHECK_TYPEOFCOURSE = "请选择大学类型";

    private static final String CHECK_UNIV = "请填写大学";

    private static final String CHECK_UNIV_NOT_EXIST = "没有这个大学";

    private static final String CHECK_HIGHSCHOOL_NOT_EXIST = "没有这个高中";

    //private static final String CHEKC_JUNIORSCHOOL_NOT_EXIST = "没有这个初中";

    private static final String CHECK_TECH_NOT_EXIST = "没有这个学校";

    private static final String CHECK_ELEMENTARY_UNSAFE = "小学名称包含不恰当内容，请检查";

    private static final String CHECK_COLLEGE_UNSAFE = "院系名称包含不恰当内容，请检查";

    private static final String CHECK_COLLEGE = "请填写院系";

    private static MobileFillinfoLogic _instance = new MobileFillinfoLogic();

    public static MobileFillinfoLogic getInstance() {
        return _instance;
    }

    private MobileFillinfoLogic() {
        super();
    }

    FillInfoLogic userInfoLogic = FillInfoLogic.getInstance();

    /**
     * 保存保存性别<br>
     * User
     *
     * @param host
     * @param gender
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:17:35
     */
    public void saveGender(User host, String gender) {
        userInfoLogic.saveGender(host, gender);
    }

    /**
     * 保存家乡<br>
     * userbasic
     *
     * @param host
     * @param hometownProvince
     * @param hometownCity
     * @param hometownCityCode
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午10:33:15
     */
    public void saveHometown(User host, String hometownProvince,
                             String hometownCity, String hometownCityCode) {
        userInfoLogic.saveHometown(host, hometownProvince, hometownCity,
                hometownCityCode);
    }

    /**
     * 保存高中信息<br>
     * 网络 资料
     *
     * @param host
     * @param highSchoolCode
     * @param highSchoolEnrollYear
     * @param highSchoolName
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:01:37
     */
    public void saveHighSchool(User host, int highSchoolCode,
                               int highSchoolEnrollYear, String highSchoolName, boolean isNet,
                               int status) {
        userInfoLogic.saveHighSchool(host, highSchoolCode,
                highSchoolEnrollYear, highSchoolName, isNet, status);
    }

    /**
     * 保存技校信息<br>
     * 网络 资料
     *
     * @param host
     * @param techSchoolCode
     * @param techSchoolEnrollYear
     * @param techSchoolName
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:01:37
     */
    public void saveTechSchool(User host, int techSchoolCode,
                               int techSchoolEnrollYear, String techSchoolName, boolean isNet,
                               int status) {
        userInfoLogic.saveTechSchool(host, techSchoolCode,
                techSchoolEnrollYear, techSchoolName, isNet, status);
    }

    /**
     * 保存初中信息<br>
     * 资料 网络
     *
     * @param host
     * @param juniorSchoolCode
     * @param juniorSchoolEnrollYear
     * @param juniorSchoolName
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:02:10
     */
    public void saveJuniorSchool(User host, int juniorSchoolCode,
                                 int juniorSchoolEnrollYear, String juniorSchoolName, boolean isNet,
                                 int status) {
        userInfoLogic.saveJuniorSchool(host, juniorSchoolCode,
                juniorSchoolEnrollYear, juniorSchoolName, isNet, status);
    }

    /**
     * 保存小学信息<br>
     * 资料
     *
     * @param host
     * @param elementarySchoolEnrollYear
     * @param elementarySchoolName
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:02:46
     */
    public void saveElementarySchool(User host, int elementarySchoolEnrollYear,
                                     String elementarySchoolName) {
        userInfoLogic.saveElementarySchool(host, elementarySchoolEnrollYear,
                elementarySchoolName);
    }

    /**
     * 保存大学信息
     *
     * @param host
     * @param univEnrollyear
     * @param univId
     * @param univName
     * @param typeOfCourse
     * @param department
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:12:40
     */
    public void saveUniv(User host, int univEnrollyear, int univId,
                         String univName, String typeOfCourse, String department, int status) {
        userInfoLogic.saveUniv(host, univEnrollyear, univId, univName,
                typeOfCourse, department, status);
    }

    /**
     * 保存居住地
     *
     * @param host
     * @param locationProvince
     * @param locationCity
     * @param locationCode
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:39:48
     */
    public void saveLocation(User host, String locationProvince,
                             String locationCity, int locationCode) {
        userInfoLogic.saveLocation(host, locationProvince, locationCity,
                locationCode);
    }

    /**
     * 保存公司
     *
     * @param host
     * @param company
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:40:11
     */
    public void saveCompany(User host, String company) {
        userInfoLogic.saveCompany(host, company);
    }

    /**
     * 检查白领fillinfo form
     *
     * @param fillForm
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午04:28:35
     */
    public String checkBLForm(MobileBLFillInfoForm fillForm) {
        if (!StringUtils.equals("男生", fillForm.getGender())
                && !StringUtils.equals("女生", fillForm.getGender())) {
            return CHECK_GENDER;
        }
        if (StringUtils.isEmpty(fillForm.getLocationCity())
                || StringUtils.isEmpty(fillForm.getLocationProvince())) {
            return CHECK_LOCATION;
        }
        if (Stage.STAGE_MAJOR == fillForm.getStage()) {
            if (antiSpamWords(fillForm.getCompany()) == true) {
                return CHEKC_COMPANY_NOT_SAFE;
            }
        }
        return CHECK_OK;
    }

    /**
     * 检查高中fillinfo form
     *
     * @param fillForm
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午04:31:32
     */
    public String checkGZForm(MobileGZFillInfoForm fillForm) {
        if (!StringUtils.equals("男生", fillForm.getGender())
                && !StringUtils.equals("女生", fillForm.getGender())) {
            return CHECK_GENDER;
        }
        if (StringUtils.isEmpty(fillForm.getHometownCity())
                || StringUtils.isEmpty(fillForm.getHometownProvince())) {
            return CHECK_HOMETOWN;
        }
        if (StringUtils.isEmpty(fillForm.getJuniorSchoolName())
                || 0 == fillForm.getJuniorSchoolCode()) {
            return CHECK_JUNIOR;
        }
        if (0 == fillForm.getJuniorSchoolEnrollYear()) {
            return CHECK_JUNIOR_ENROLLYEAR;
        }
        
        //总是提示找不着初中 是不是数据不同步了？ 注了吧先 inshion 2010-9-16 15:42:29
        /*try {
            JuniorSchool juniorSchool = JuniorSchoolDAO.getInstance()
                    .getByName(fillForm.getJuniorSchoolName());
            if (juniorSchool == null) {
                return CHEKC_JUNIORSCHOOL_NOT_EXIST;
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }*/
        if (StringUtils.isEmpty(fillForm.getElementarySchoolName())) {
            return CHECK_ELEMENTARY;
        }
        if (0 == fillForm.getElementarySchoolEnrollYear()) {
            return CHECK_ELEMENTARY_ENROLLYEAR;
        }
        if (true == this.antiSpamWords(fillForm.getElementarySchoolName())) {
            return CHECK_ELEMENTARY_UNSAFE;
        }
        if (MobileGZFillInfoForm.HIGH_SCHOOL_TYPE == fillForm.getSchoolType()) {
            if (StringUtils.isEmpty(fillForm.getHighSchoolName())
                    || 0 == fillForm.getHighSchoolCode()) {
                return CHECK_HIGH;
            }
            if (0 == fillForm.getHighSchoolEnrollYear()) {
                return CHECK_HIGH_ENROLLYEAR;
            }
            HighSchool highSchool = NetworkManager.getInstance()
                    .getHighSchoolById(fillForm.getHighSchoolCode());
            if (highSchool == null) {
                return CHECK_HIGHSCHOOL_NOT_EXIST;
            }
        } else if (MobileGZFillInfoForm.TECH_SCHOOL_TYPE == fillForm
                .getSchoolType()) {
            if (StringUtils.isEmpty(fillForm.getTechSchoolName())
                    || 0 == fillForm.getTechSchoolCode()) {
                return CHECK_TECH;
            }
            if (0 == fillForm.getTechSchoolEnrollYear()) {
                return CHECK_TECH_ENROLLYEAR;
            }
            try {
                College college = CollegeDAO.getInstance().getByName(
                        fillForm.getTechSchoolName());
                if (college == null) {
                    return CHECK_TECH_NOT_EXIST;
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        return CHECK_OK;
    }

    /**
     * 检查大学fillinfo form
     *
     * @param fillForm
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午04:33:48
     */
    public String checkDXForm(MobileDXFillInfoForm fillForm) {
        if (!StringUtils.equals("男生", fillForm.getGender())
                && !StringUtils.equals("女生", fillForm.getGender())) {
            return CHECK_GENDER;
        }
        if (StringUtils.isEmpty(fillForm.getTypeOfCourse())) {
            return CHECK_TYPEOFCOURSE;
        }
        if (StringUtils.isEmpty(fillForm.getUnivName())
                || fillForm.getUnivId() == 0) {
            return CHECK_UNIV;
        }
        University university = NetworkManager.getInstance().getUnivById(
                fillForm.getUnivId());
        if (university == null) {
            return CHECK_UNIV_NOT_EXIST;
        }
        if (StringUtils.isEmpty(fillForm.getDepartment())) {
            return CHECK_COLLEGE;
        }
        if (antiSpamWords(fillForm.getDepartment()) == true) {
            return CHECK_COLLEGE_UNSAFE;
        }
        if (StringUtils.isEmpty(fillForm.getHighSchoolName())
                || 0 == fillForm.getHighSchoolCode()) {
            return CHECK_HIGH;
        }
        if (0 == fillForm.getHighSchoolEnrollYear()) {
            return CHECK_HIGH_ENROLLYEAR;
        }
        HighSchool highSchool = NetworkManager.getInstance().getHighSchoolById(
                fillForm.getHighSchoolCode());
        if (highSchool == null) {
            return CHECK_HIGHSCHOOL_NOT_EXIST;
        }
        return CHECK_OK;
    }

    /**
     * antispam 检查
     *
     * @param word
     * @return 拦截 true
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午05:00:58
     */
    private boolean antiSpamWords(String word) {
		CheckResult cr = Antispam.checkAndFilterCR(word);
		if (cr.getFlag() == CheckResult.PROHIBITED) {
			return true;
		} else if (cr.getFlag() == CheckResult.SYSTEM_ERROR) {
			return true;
		}
		return false;
	}
}
