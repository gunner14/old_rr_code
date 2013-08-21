package com.xiaonei.reg.guide.action;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.FilterSearchCondition;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.RegionCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.condition.subcondition.WorkplaceCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.xce.log.TimeCost;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

/**
 * 展示用户所选的家乡，地域网络（目前所在地）和公司中的人数
 *
 * @author wang-tai
 */
public class AjaxShowHowManyPeopleInMyGroup extends BaseAction {

    /**
     * 根据用户填写的资料信息返回对应的人数
     *
     * @author wang-tai
     * @see com.xiaonei.platform.core.opt.base.action.BaseAction#executeBt(org.apache.struts.action.ActionMapping,
     *      org.apache.struts.action.ActionForm,
     *      javax.servlet.http.HttpServletRequest,
     *      javax.servlet.http.HttpServletResponse) ?group=hometown 返回家乡人数
     *      ?group=home 返回地域网络人数 ?group=corp 返回公司人数 ？group=highschool 返回高中人数
     *      ?group=univ 返回大学人数
     */
    @Override
    public ActionForward executeBt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws Exception {
        TimeCost cost = TimeCost.begin("AjaxShowHowManyPeopleInMyGroup");

        String strSubFlag = (String) request.getParameter("group");
        if (strSubFlag == null) {
            return null;
        }
        String strNumber = "0";
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        try {
            // 计算家乡人数
            if (strSubFlag.equals("hometown")) {
                // 获得家乡名
                String strhometownprovince = request.getParameter("argv0");
                String strhometownname = request.getParameter("argv1");
                // 查询
                // GuideCondition builder = new GuideCondition();
                FilterSearchCondition builder = new FilterSearchCondition();
                builder.addOpUser(host.getId());
                builder.addCurrentStage(Stage.STAGE_MAJOR);
                builder.addHomeProvince(strhometownprovince);
                builder.addHomeCity(strhometownname);
                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories
                        .put(IndexType.User, UserGuideWebViewFactory.getInstance());
                cost.endStep("1");
                SearchResult result = SearchAdapter.getInstance().search(builder,
                        factories, 0, 0);
                cost.endStep("2");
                strNumber = "" + result.getTotalCount();
                cost.endStep("3");
                // 输出页面
                this.output(response, strNumber);
            }
            // 计算地域网络人数
            else if (strSubFlag.equals("home")) {
                // 计算人数
                // 获得地域名
                String strhomeprovince = (String) request.getParameter("argv0");
                String strhomename = (String) request.getParameter("argv1");
                // 查询
                FilterSearchCondition builder = new FilterSearchCondition();
                RegionCondition regCondition = new RegionCondition(0,
                        strhomeprovince, strhomename);
                builder.addRegionInfo(regCondition);
                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories
                        .put(IndexType.User, UserGuideWebViewFactory.getInstance());
                cost.endStep("4");
                SearchResult result = SearchAdapter.getInstance().search(builder,
                        factories, 0, 5);
                cost.endStep("5");
                result.getTotalCount();
                cost.endStep("6");
                // 输出页面
                this.output(response, strNumber);
            }
            // 计算公司人数
            else if (strSubFlag.equals("corp")) {
                // 计算
                // 获得公司名
                String strWorkplaceName = (String) request.getParameter("argv0");
                // 计算人数
                GuideCondition builder = new GuideCondition();
                WorkplaceCondition wpCondition = new WorkplaceCondition(0,
                        strWorkplaceName);
                // 设置公司查询条件
                builder.addWorkplaceInfo(wpCondition);
                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories
                        .put(IndexType.User, UserGuideWebViewFactory.getInstance());
                cost.endStep("7");
                SearchResult result = SearchAdapter.getInstance().search(builder,
                        factories, 0, 5);
                cost.endStep("8");
                // 设置公司总人数
                strNumber = "" + result.getTotalCount();
                cost.endStep("9");
                // 输出页面
                this.output(response, strNumber);
            }
            // 计算高中人数
            else if (strSubFlag.equals("highschool")) {
                // 计算
                // 获得高中名
                String strHighSchool = (String) request.getParameter("argv0");
                if (strHighSchool == null || strHighSchool.length() == 0) {
                    strHighSchool = "";
                }
                // 获得高中入学年
                int iHighSchoolYear = 0;
                try {
                    iHighSchoolYear = (new Integer(((String) request
                            .getParameter("argv1")).trim())).intValue();
                    if (iHighSchoolYear < 0) {
                        iHighSchoolYear = 0;
                    }
                } catch (Exception e) {
                    iHighSchoolYear = 0;
                }
                cost.endStep("10");
                // 计算人数
                GuideCondition builder = new GuideCondition();
                HighSchoolCondition conditionHighSchool = new HighSchoolCondition(
                        0, strHighSchool, "", "", "", iHighSchoolYear);
                // 设置查询条件
                builder.addHighSchoolInfo(conditionHighSchool);
                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories
                        .put(IndexType.User, UserGuideWebViewFactory.getInstance());
                cost.endStep("11");
                //为了性能优化要改成批量处理。
                SearchResult result = SearchAdapter.getInstance().search(builder,
                        factories, 0, 5);
                cost.endStep("12");
                // 设置总人数
                strNumber = "" + result.getTotalCount();
                cost.endStep("13");
                // 输出页面
                this.output(response, strNumber);
            }
            // 计算大学人数
            else if (strSubFlag.equals("univ")) {
                // 计算
                // 获得大学名
                String strUnivSchool = (String) request.getParameter("argv0");
                if (strUnivSchool == null) {
                    strUnivSchool = "";
                }
                // 获得院系
                String strUnivDepart = (String) request.getParameter("argv1");
                if (strUnivDepart == null) {
                    strUnivDepart = "";
                }
                // 获得大学入学年
                int iUnivYear = 0;
                try {
                    iUnivYear = (new Integer(((String) request
                            .getParameter("argv2")).trim())).intValue();
                    if (iUnivYear < 0) {
                        iUnivYear = 0;
                    }
                } catch (Exception e) {
                    iUnivYear = 0;
                }
                cost.endStep("14");
                // 计算人数
                GuideCondition builder = new GuideCondition();
                UniversityCondition conditionUniv = new UniversityCondition(0,
                        strUnivSchool, strUnivDepart, "", iUnivYear);
                // 设置查询条件
                builder.addUniversityInfo(conditionUniv);
                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories
                        .put(IndexType.User, UserGuideWebViewFactory.getInstance());
                cost.endStep("15");
                SearchResult result = SearchAdapter.getInstance().search(builder,
                        factories, 0, 5);
                cost.endStep("16");
                // 设置总人数
                strNumber = "" + result.getTotalCount();
                cost.endStep("17");
                // 输出页面
                this.output(response, strNumber);
            }
        } catch (Exception e) {
            cost.endException(e);
            e.printStackTrace();
        } finally {
            cost.endFinally();
        }
        return null;
    }

    private void output(HttpServletResponse response, String num) {
        response.setContentType("text/html;charset=utf-8");
        
        String msg = DodoyoResource.getString("messages.peopleallreadyin",num);
        try {
            PrintWriter out = response.getWriter();
            out.print(msg);
        } catch (Exception e) {
            e.printStackTrace(System.err);
		}
	}

}
