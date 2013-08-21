package com.xiaonei.reg.guide.logic.preview;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.NetworkUtils;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.guide.view.DXPreviewView;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.model.IndexType;
import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 大学阶段的加好友引导
 *
 * @author dream
 * @date 2008-10-15
 */
public class DXPreviewLogic {

    static class SingletonHolder {
        static DXPreviewLogic instance = new DXPreviewLogic();
    }

    public static DXPreviewLogic getInstance() {
        return SingletonHolder.instance;
    }

    /**
     * 搜索大学同学
     *
     * @param request
     * @param host
     * @param count          要搜索的个数
     * @param universityId   大学的ID
     * @param universityName 大学的名称
     * @param universityYear 大学的入学年
     * @param department     院系
     * @return List<SearchResult> 搜索结果
     */
    public DXPreviewView searchUnivResult(User host) {
        UniversityInfo universityInfo = null;
        if (UserAdapter.get(host).getStage() == Stage.STAGE_UNIV) {
            try {
                universityInfo = AdapterFactory
                        .getNetworkAdapter()
                        .getUniversityInfoByUnivId(host.getId(), UserAdapter.get(host).getUniv());
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }

        }
        if (universityInfo != null) {
            DXPreviewView d = new DXPreviewView();
            d.setUniversityId(universityInfo.getUniversityId());
            d.setDepartment(universityInfo.getDepartment());
            d.setUniversityName(universityInfo.getUniversityName());
            d.setUniversityYear(universityInfo.getEnrollYear());
            return d;
        }
        return null;
    }

    public NewGuideView getGuideView(DXPreviewView d, User host, int count) {
        try {
            if (d != null
                    && !Utils.getInstance()
                    .validateForString(d.getDepartment())) {
                GuideCondition builder = new GuideCondition();
                builder.addOpUser(host.getId());
                UniversityCondition univCondition = new UniversityCondition(d
                        .getUniversityId(), d.getUniversityName(), d
                        .getDepartment(), null, d.getUniversityYear());
                builder.addUniversityInfo(univCondition);

                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                factories.put(IndexType.User, UserGuideWebViewFactory
                        .getInstance());
                // 条件够了就开始搜索
                SearchResult result = SearchAdapter.getInstance().search(
                        builder, factories, 0, count);
                // 搜索的结果存在NewGuideView对象中
                NewGuideView ngv = new NewGuideView();
                ngv.setResult(result.getContents());
                ngv.setTitle(d.getUniversityName());
                ngv.setTotalCount(result.getTotalCount());
                return ngv;
            }
        } catch (Exception e) {
        }
        return null;
    }

    public List<HighSchoolInfo> getHSInfoList(User host) {
        try {
            return AdapterFactory.getNetworkAdapter().getHighSchoolInfoList(
                    host.getId());
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    /**
     * modified by llb 增加中专技校
     * <ul>
     * <li></li>
     * </ul>
     *
     * @param host
     * @param count
     * @return
     */
    public NewGuideView getHSGuideView(User host, int count) {
        try {
        	System.out.println(UserAdapter.get(host).getUniv());
            if (NetworkUtils.isCollegeNetwork(UserAdapter.get(host).getUniv())) {// 中专技校
                List<CollegeInfo> cos = AdapterFactory.getNetworkAdapter().getCollegeInfoList(host.getId());
                if (cos != null) {
                    for (CollegeInfo co : cos) {
                        if (co != null) {
                        	System.out.println(co.getCollegeId());
                            if (co.getCollegeId() > 0) {
                            	System.out.println(co.getCollegeName());
                                GuideCondition builder = new GuideCondition();
                                builder.addOpUser(host.getId());
                                CollegeCondition hsCondition = new CollegeCondition(0, co.getCollegeName(), co.getDepartment(), co.getEnrollYear());
                                builder.addCollegeInfo(hsCondition);
                                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                                factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
                                SearchResult result = null;
                                try {
                                    result = SearchAdapter.getInstance().search(builder, factories, 0, count);
                                } catch (Exception e) {
                                    e.printStackTrace(System.err);
                                }
                                NewGuideView ngv = new NewGuideView();
                                if (null != result) {
                                    ngv.setResult(result.getContents());
                                    ngv.setTitle(co.getCollegeName());
                                }
                                return ngv;
                            }
                        }
                    }
                }
            } else {// 高中
                if (NetworkUtils.isHighSchoolNetwork(UserAdapter.get(host).getUniv())) {// 加入是高中
                    List<HighSchoolInfo> highSchoolList = getHSInfoList(host);
                    try {
						System.out.println(highSchoolList.size());
					} catch (Exception e1) {
						e1.printStackTrace();
					}
                    if (highSchoolList != null && highSchoolList.size() > 0) {
                        HighSchoolInfo highSchoolInfo = highSchoolList.get(0);
                        try {
							System.out.println(highSchoolInfo.getHighSchoolName());
						} catch (Exception e1) {
							e1.printStackTrace();
						}
                        if (highSchoolInfo != null) {
                            try {
                                if (!Utils.getInstance().validateForString(highSchoolInfo.getHighSchoolName())) {
                                    GuideCondition builder = new GuideCondition();
                                    builder.addOpUser(host.getId());
                                    HighSchoolCondition hsCondition = new HighSchoolCondition(0, highSchoolInfo.getHighSchoolName(), "", "", "", highSchoolInfo.getEnrollYear());
                                    builder.addHighSchoolInfo(hsCondition);
                                    Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                                    factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
                                    SearchResult result = null;
                                    try {
                                        result = SearchAdapter.getInstance().search(builder, factories, 0, count);
                                    } catch (Exception e) {
                                        e.printStackTrace(System.err);
                                    }
                                    NewGuideView ngv = new NewGuideView();
                                    if (null != result) {
                                        ngv.setResult(result.getContents());
                                        ngv.setTitle(highSchoolInfo.getHighSchoolName());
                                    }
                                    return ngv;
                                }
                            } catch (Exception e) {
                                e.printStackTrace(System.err);
                            }
                        }
                    }
                } else {// 初中
                    if (NetworkUtils.isJuniorNetwork(UserAdapter.get(host).getUniv())) {
                        JuniorHighSchoolInfo jhsInfo = AdapterFactory.getNetworkAdapter().getJuniorHighSchoolInfo(host.getId());
                        try {
							System.out.println(jhsInfo.getJunHighentarySchoolName());
						} catch (Exception e1) {
							e1.printStackTrace();
						}
                        if (jhsInfo != null) {
                            if (jhsInfo.getJunHighentarySchoolId() > 0) {
                                GuideCondition builder = new GuideCondition();
                                builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(jhsInfo.getJunHighentarySchoolId(), null, jhsInfo.getJunHighentarySchoolYear()));
                                builder.addOpUser(host.getId());
                                Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                                factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
                                SearchResult result = null;
                                try {
                                    result = SearchAdapter.getInstance().search(builder, factories, 0, count);
                                } catch (Exception e) {
                                    e.printStackTrace(System.err);
                                }
                                NewGuideView ngv = new NewGuideView();
                                if (null != result) {
                                    ngv.setResult(result.getContents());
                                    ngv.setTitle(jhsInfo.getJunHighentarySchoolName());
                                }
                                return ngv;
                            }
                        }
                    }

                }
            }
        } catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
}
