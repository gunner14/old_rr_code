package com.xiaonei.reg.guide.logic.fillinfo;

import java.util.List;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;

import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.pojo.DXFillinfoPojo;

public class DXFillinfoLogic extends CommonFillinfoLogic {

    static class SingletonHolder {
        static DXFillinfoLogic instance = new DXFillinfoLogic();
    }

    public static DXFillinfoLogic getInstance() {
        return SingletonHolder.instance;
    }

    public void saveDataByUniv(DXFillinfoPojo c) {
    	//2010-01-20 拆分UserBasic
    	// 2010-01-20 这里其实什么都没干了，所以给注掉了。
        /*try {
            //UserBasic oceBasic = new UserBasic();
            //oceBasic.setId(c.getHost().getId());
            //oceBasic.setUnivYear(c.getUnivYear());
            // TODO 2009-12-10 16:51:04 UnivYear没了
            //UserAdapter.setBasic(oceBasic);
        } catch (Exception e) {
            //e.printStackTrace(System.err);
        }*/
        try {
            // 大学网络
            UniversityInfo universityInfo = AdapterFactory.getNetworkAdapter().getUniversityInfoByUnivId(c.getHost().getId(), UserAdapter.get(c.getHost()).getUniv());
            if (universityInfo != null) {
                universityInfo.setDepartment(c.getDepartment());
                universityInfo.setEnrollYear(c.getUnivYear());
                AdapterFactory.getNetworkAdapter().setUniversityInfo(universityInfo);
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        try {
            // 高中
            List<HighSchoolInfo> highSchoolList = AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(c.getHost().getId());
            if (highSchoolList == null || highSchoolList.size() == 0) { // 没有高中
                HighSchoolInfo highSchoolInfo = new HighSchoolInfo();
                highSchoolInfo.setHighSchoolId(c.getHighSchoolId());
                highSchoolInfo.setHighSchoolName(c.getHighSchoolName());
                highSchoolInfo.setEnrollYear(c.getHighSchoolYear());
                highSchoolInfo.setUserId(c.getHost().getId());
                highSchoolInfo.setHClass1("0");
                highSchoolInfo.setHClass2("0");
                highSchoolInfo.setHClass3("0");
                AdapterFactory.getNetworkAdapter().addHighSchoolInfo(highSchoolInfo);
            } else {
                HighSchoolInfo highSchoolInfo = highSchoolList.get(0);
                if (highSchoolInfo != null) {
                    highSchoolInfo.setHighSchoolId(c.getHighSchoolId());
                    highSchoolInfo.setHighSchoolName(c.getHighSchoolName());
                    highSchoolInfo.setEnrollYear(c.getHighSchoolYear());
                    AdapterFactory.getNetworkAdapter().setHighSchoolInfo(highSchoolInfo);
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }


        // 收集院系和宿舍
        CommonFillinfoLogic.collectDept(c.getHost().getId(), UserAdapter.get(c.getHost()).getUniv(), c.getOtherDepartment());

        // 加入新生活动群
        if (c.getUnivYear() == 2008) {
            CommonFillinfoLogic.joinGroup(c.getHost().getId(), 251492476);
        }
    }

}
