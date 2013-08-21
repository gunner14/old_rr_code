package com.renren.sns.guide.business.handler;

import java.sql.SQLException;
import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;

import com.dodoyo.datum.dao.UniversityDAO;
import com.renren.sns.guide.business.GuideWindowHandler;
import com.renren.sns.guide.dao.StarDAO;
import com.renren.sns.guide.view.UserTinyView;
import com.xiaonei.platform.core.model.University;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.network.NetworkManager;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

public class GuideHotStarHandler implements GuideWindowHandler {

    public static final String GUIDE_HOTSTAR_KEY = "starList_IDS";

    @Override
    public void execute(Invocation inv, Window window, CacheManager cm) {
        User host = UserProvider.getInstance().get(cm);
        List<UserTinyView> starList = null;
        try {// 人气之星
            if ((host.getUniv() >= 1001) && (host.getUniv() <= 9999999)) {
                starList = StarDAO.getInstance().getByNetwork(host.getUniv());
                University starUniv = NetworkManager.getInstance().getUnivById(host.getUniv());
                if ((starList == null) || (starList.size() == 0)) {
                    starUniv = UniversityDAO.getInstance().getOnePopUniv(host.getUniv());
                    if (starUniv == null) {
                        starList = StarDAO.getInstance().getByTop();
                    } else {
                        starList = StarDAO.getInstance().getByNetwork(starUniv.getId());
                        if ((starList == null) || (starList.size() == 0)) {
                            starUniv = null;
                            starList = StarDAO.getInstance().getByTop();
                        }
                    }
                }
                inv.addModel("starNetwork", starUniv);
            } else if (((host.getUniv() >= 10000001) && (host.getUniv() <= 19999999))
                    || ((host.getUniv() >= 40000001) && (host.getUniv() <= 49999999))
                    || ((host.getUniv() >= 80000000) && (host.getUniv() <= 89999999))) {
                starList = StarDAO.getInstance().getByNetwork(host.getUniv());
                if (starList.size() < 20) {
                    // 数量不足时的填补
                    List<UserTinyView> sl = null;
                    sl = StarDAO.getInstance().getByNetwork(host.getUniv() + 1);
                    if (sl != null) {
                        starList.addAll(sl);
                    }
                }
                inv.addModel("starNetwork", NetworkManager.getInstance().getHighSchoolById(host.getUniv()));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        if (starList != null) {
            inv.addModel(GUIDE_HOTSTAR_KEY, starList);
        }

    }

}
