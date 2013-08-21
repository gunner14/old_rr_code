package com.xiaonei.reg.guide.logic.preview;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.action.a090821.UserProfileUtil;
import com.xiaonei.reg.guide.logic.NetworkUtils;
import com.xiaonei.reg.guide.logic.msn.UserGuideWebViewFactory;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.AbstractCondition;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.CollegeCondition;
import com.xiaonei.search2.condition.subcondition.HighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.JuniorHighSchoolCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.model.IndexType;
import com.xiaonei.search2.model.client.SearchView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;

public class CommonPreviewLogic {

    static class SingletonHolder {
        static CommonPreviewLogic instance = new CommonPreviewLogic();
    }

    public static CommonPreviewLogic getInstance() {
        return SingletonHolder.instance;
    }

    /**
     * 可以递归的混排两个list
     *
     * @param tempList1 待混排的list1
     * @param tempList2 待混排的list2
     * @return list对象
     * @author seraph
     */
    public List<SearchView> mixList(List<SearchView> tempList1, List<SearchView> tempList2) {
        List<SearchView> mixList = new ArrayList<SearchView>();
        int size1 = 0, size2 = 0;
        if (tempList1 != null && tempList1.size() > 0) {
            size1 = tempList1.size();
            mixList = tempList1;
        }
        if (tempList2 != null && tempList2.size() > 0) {
            size2 = tempList2.size();
            mixList = tempList2;
        }
        if (tempList1 == null || tempList2 == null) {
            return mixList;
        }

        int maxSize = (size1 >= size2) ? size1 : size2;
        boolean flag = false;

        for (int i = 0; i < maxSize; i++) {
            if (flag == false) {
                if (size1 > i) {
                    mixList.add(tempList1.get(i));
                }
                if (size2 > i) {
                    mixList.add(tempList2.get(i));
                }
            } else if (flag == true) {
                if (size2 > i) {
                    mixList.add(tempList2.get(i));
                }
                if (size1 > i) {
                    mixList.add(tempList1.get(i));
                }
            }

            flag = !flag;
        }

        return mixList;
	}
    //private static final ProfileLogic profileLogic = new ProfileLogic();
	public NewGuideView getHSGuideView(User host, int count) {
		NewGuideView ngv = null;
		// 大学
		if (isProfile(host, ngv)) {
			try {
				UniversityInfo univInfo = UserProfileUtil.loadUniversity(host);
				if(univInfo == null) return null;
//				UniversityInfo univInfo = profileLogic.getUniversityInfoList(host.getId()).get(0);
				if ("".equals(univInfo.getUniversityName())) {
					throw new Exception();
				}
				GuideCondition builder = new GuideCondition();
				builder.addOpUser(host.getId());
				builder.addUniversityInfo(new UniversityCondition(univInfo
						.getUniversityId(), univInfo.getUniversityName(),
						univInfo.getDepartment(), null, univInfo
								.getEnrollYear()));
				ngv = searchG(univInfo.getUniversityName(), count, builder);
			} catch (Exception e) {
			}
		}
		// 高中
		if (NetworkUtils.isHighSchoolNetwork(UserAdapter.get(host).getUniv())
				|| isProfile(host, ngv)) {
			try {
//				List<HighSchoolInfo> highSchoolList = profileLogic.getHighschoolInfoList(host.getId());
//				HighSchoolInfo highSchoolInfo = highSchoolList.get(0);
				HighSchoolInfo highSchoolInfo = UserProfileUtil.loadHighschool(host);
				if(highSchoolInfo == null) return null;
				GuideCondition builder = new GuideCondition();
				builder.addOpUser(host.getId());
				builder.addHighSchoolInfo(new HighSchoolCondition(0,
						highSchoolInfo.getHighSchoolName(), "", "", "",
						highSchoolInfo.getEnrollYear()));
				ngv = searchG(highSchoolInfo.getHighSchoolName(), count,
						builder);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		// 中专技校
		if (NetworkUtils.isCollegeNetwork(UserAdapter.get(host).getUniv())
				|| isProfile(host, ngv)) {
			try {
				List<CollegeInfo> cos = AdapterFactory.getNetworkAdapter()
						.getCollegeInfoList(host.getId());
				GuideCondition builder = new GuideCondition();
				builder.addOpUser(host.getId());
				builder.addCollegeInfo(new CollegeCondition(0, cos.get(0)
						.getCollegeName(), cos.get(0).getDepartment(), cos.get(
						0).getEnrollYear()));
				ngv = searchG(cos.get(0).getCollegeName(), count, builder);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		// 初中
		if (NetworkUtils.isJuniorNetwork(UserAdapter.get(host).getUniv())
				|| isProfile(host, ngv)) {
			try {
				JuniorHighSchoolInfo jhsInfo = AdapterFactory
						.getNetworkAdapter().getJuniorHighSchoolInfo(
								host.getId());
				GuideCondition builder = new GuideCondition();
				builder.addOpUser(host.getId());
				builder.addJuniorHighSchoolInfo(new JuniorHighSchoolCondition(
						jhsInfo.getJunHighentarySchoolId(), null, jhsInfo
								.getJunHighentarySchoolYear()));
				ngv = searchG(jhsInfo.getJunHighentarySchoolName(), count,
						builder);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return ngv;
	}
	private boolean isProfile(User host, Object ngv) {
		return !NetworkUtils.isMiddleSchoolNetwork(UserAdapter.get(host).getUniv())
				&& null == ngv;
	}

	private NewGuideView searchG(String schName, int count,
			AbstractCondition builder) {
		Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
		factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());
		SearchResult result = null;
		try {
			result = SearchAdapter.getInstance().search(builder, factories, 0,
					count);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		NewGuideView ngv = new NewGuideView();
		if (null != result) {
			ngv.setResult(result.getContents());
			ngv.setTitle(schName);
		}
		return ngv;
	}

}
