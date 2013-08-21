package com.renren.sns.guide.action;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.datum.cache.DatumInfoMgr;
import com.xiaonei.platform.component.space.home.SpaceHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;

public class AjaxGuideModifyInfoAction extends BaseAction {
    public ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException{     
        User host = BaseThreadUtil.currentHost(request);             
        // 通过中间层得到部分数据
        List<UniversityInfo> uis = new ArrayList<UniversityInfo>(5);
        List<HighSchoolInfo> his = new ArrayList<HighSchoolInfo>(2);        
        // 得到的大学信息列表
        try {
            List<UniversityInfo> list = DatumInfoMgr.getInstance().getUniversityInfos(host.getId());            
            int actualLen = list.size();
            for (int i = 0; i < 5; i++) {
                if (i < actualLen) {
                    uis.add(list.get(i));
                } else {
                    if (i == 0) {
                        uis.add(new UniversityInfo());
                    } else {
                        uis.add(null);
                    }
                }
            }             
            request.setAttribute("actualUniversityListLength", actualLen);
            request.setAttribute("myUniversityList", uis);
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        // 得到高中信息列表
        try {
            List<HighSchoolInfo> list = DatumInfoMgr.getInstance().getHighSchoolInfos(host.getId());
            int actualLen = list.size();
            for (int i = 0; i < 2; i++) {
                if (i < actualLen) {
                    his.add(list.get(i));
                } else {
                    if (i == 0) {
                        his.add(new HighSchoolInfo());
                    } else {
                        his.add(null);
                    }
                }
            }
            request.setAttribute("actualHighschoolListLength", actualLen);
            request.setAttribute("myHighschoolList", his);//
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        // 得到技校信息
        List<CollegeInfo> coi = null;
        try {
            coi = DatumInfoMgr.getInstance().getCollegeInfos(host.getId());
            if (null != coi && coi.size() > 0) {
                request.setAttribute("collegeInfo", coi.get(0));
            }
        } catch (Exception e) {
        }
        // 得到中学和小学信息
        try {
            ElementarySchoolInfo eleInfo = DatumInfoMgr.getInstance().getElementarySchoolInfos(host.getId()); // 小学资料信息
            JuniorHighSchoolInfo junInfo = DatumInfoMgr.getInstance().getJuniorHighSchoolInfos(host.getId()); // 初中资料信息
            request.setAttribute("userEle", eleInfo);
            request.setAttribute("userJun", junInfo);
        } catch (Exception e) {
            e.printStackTrace();
        }
        request.setAttribute("userWorkplaceInfoList", SpaceHome.getInstance().getWorkplaceInfoList(host.getId()));
        return mapping.findForward("succ");
    }

}
