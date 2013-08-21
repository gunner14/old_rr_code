package com.xiaonei.reg.guide.logic.msn;

import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.jdbc.dao.UniversityDAO;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.dao.UserBootStatusDAO;
import com.xiaonei.reg.guide.view.NewGuideView;
import com.xiaonei.search2.SearchAdapter;
import com.xiaonei.search2.SearchResult;
import com.xiaonei.search2.SearchViewFactory;
import com.xiaonei.search2.condition.GuideCondition;
import com.xiaonei.search2.condition.subcondition.UniversityCondition;
import com.xiaonei.search2.model.IndexType;
import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.UniversityInfo;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 仅白领阶段的msn导入逻辑
 *
 * @author dream
 * @date 2008-10-15
 */
public class CommonImportMsnLogic {

    // 沒有被邀請
    public static final int REG_INVITE_NO = 90;
    // 通過邀請鏈接，改變了email
    public static final int REG_INVITE_CHANGEMAIL = 91;
    // 沒有點邀請鏈接, 但被邀請
    public static final int REG_INVITE_NOTLINK = 92;
    // 通過邀請鏈接
    public static final int REG_INVITE_LINK = 93;

    public static final int MAJOR_AUDIT_NOPASS = 0;

    public static final int MAJOR_AUDIT_PASSED = 2;

    public static final int MAJOR_AUDIT_FAIL = 1;

    public static final String STAT_COOKIE_Name = "__sui_xiaonei";

    public static final String STAT_FROM = "__sui_xiaonei_f";

    //private static final String MEM_CACHE_REG_SURNAME_LIST = "mem_cache_reg_surname_list";

    static class SingletonHolder {
        static CommonImportMsnLogic instance = new CommonImportMsnLogic();
    }

    public static CommonImportMsnLogic getInstance() {
        return SingletonHolder.instance;
    }

    public static boolean isInvited(User host) {
        boolean isInvited = false;

        try {
            if (UserAdapter.get(host).getSource() > CommonImportMsnLogic.REG_INVITE_NO) {
                isInvited = true;
            } else {
                int status = UserBootStatusDAO.getInstance().getInviteStatus(host.getId());
                if (status > CommonImportMsnLogic.REG_INVITE_NO) {
                    isInvited = true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }

        return isInvited;
    }

    public static boolean isMSNEmail(String email) {
        if (email != null
                && (email.endsWith("hotmail.com") || email.endsWith("msn.com") || email.endsWith("live.com") || email.endsWith("live.cn") || email
                .endsWith("passport.com"))) {
            return true;
        }
        return false;
    }

    public static boolean notCompanyUser(User host) {
        boolean isMsnUser = false;

        if (isInvited(host) || isMSNEmail(host.getAccount()) || isWokrplaceEmail(host.getAccount())) {
            isMsnUser = true;
        }
        return isMsnUser;
    }

    /**
     * 判断是否为公司邮箱
     *
     * @param email
     * @return
     */
    public static boolean isWokrplaceEmail(String email) {
        boolean isWokrplaceEmail = false;
        List<Workplace> workplaceList = NetworkManager.getInstance().getWps();

        if (workplaceList != null) {
            for (int i = 0; i < workplaceList.size(); i++) {
                String emailList = workplaceList.get(i).getEmail();
                if (emailList != null) {
                    String[] emails = emailList.split(";");
                    for (int n = 0; n < emails.length; n++) {
                        if (emails[n].trim().length() > 0 && email.endsWith("@".concat(emails[n].trim()))) {
                            isWokrplaceEmail = true;
                            break;
                        }
                    }
                    if (isWokrplaceEmail)
                        break;
                }
            }
        }

        return isWokrplaceEmail;
    }

    public University getUniversity(User host) {
        try {
            return UniversityDAO.getInstance().get(UserAdapter.get(host).getUniv());
        } catch (SQLException e) {
            e.printStackTrace(System.err);
        }
        return null;
    }

    public NewGuideView getUniversityData(User host) {
        List<UniversityInfo> univList = AdapterFactory.getNetworkAdapter().getUniversityInfoList(host.getId());
        if (univList != null && univList.size() > 0) {

            try {
                UniversityInfo univInfo = null;
                univInfo = univList.get(0);
                if (univInfo != null) {
                    String univName = "";
                    univName = univInfo.getUniversityName();

                    GuideCondition builder = new GuideCondition();
                    builder.addOpUser(host.getId());
                    UniversityCondition univCondition = new UniversityCondition(0, univName, "", "", 0);
                    builder.addUniversityInfo(univCondition);

                    Map<IndexType, SearchViewFactory> factories = new HashMap<IndexType, SearchViewFactory>();
                    factories.put(IndexType.User, UserGuideWebViewFactory.getInstance());

                    SearchResult result = SearchAdapter.getInstance().search(builder, factories, 0, 7);
                    NewGuideView ngv = new NewGuideView();
                    ngv.setResult(result.getContents());
                    ngv.setTitle(univName);
                    ngv.setUnivId(univInfo.getUniversityId());
                    ngv.setTotalCount(result.getTotalCount());
                    ngv.setUnivId(univInfo.getUniversityId());
                    return ngv;
                }
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
        }
        return null;
	}
	
}
