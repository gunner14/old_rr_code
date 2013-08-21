package com.xiaonei.login.logic.rrfarm;

import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.dao.RRFarmBonusDAO;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.logic.rrfarm.RRFarmBonusModel.BonusType;
import com.xiaonei.platform.component.application.home.UserAppHome;
import com.xiaonei.platform.component.application.notification.AppNotificationMgr;
import com.xiaonei.platform.component.application.notification.exception.AppNotificationException;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Service
public class RRFarmLogicImpl implements RRFarmBonusLogic, RRFarmBonusRule{

    @Autowired
    private RRFarmBonusDAO rrfarmBonusDAO;

    @Override
    public void addBonus(int userId, RRFarmBonusModel bonus) {
        /* 发送通知到app */
        try {
            notifyTheAppPlusBonus(userId, bonus);
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        /* 保存到本地 */
        if (bonus.getType() == BonusType.item) {
            try {
                rrfarmBonusDAO.setAppsList(userId, bonus.getItem().getItemId());
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public List<RRFarmBonusModel> getBonus(RRFarmBonusConditions conditions) {
        List<RRFarmBonusModel> list = new ArrayList<RRFarmBonusModel>();
        if (!"kaixin.com".equals(conditions.getDomain())) {
            return list;
        }
        if (!isInstalledRRFarm(conditions.getUserId())) {
            return list;
        }
        if (!conditions.isSignedIn()) {
            list.add(RRFarmBonusDef.goldCoins50);
        }
        if (conditions.getConsecutiveDays() == 3) {
            list.add(RRFarmBonusDef.cherry);
        }
        if (conditions.getConsecutiveDays() == 5) {
            list.add(RRFarmBonusDef.turkey);
        }
        if (conditions.getConsecutiveDays() == 10) {
            list.add(RRFarmBonusDef.roseBouquet);
        }
        if (conditions.getConsecutiveDays() == 20) {
            list.add(RRFarmBonusDef.nest);
        }
        if (conditions.getConsecutiveDays() == 30) {
            list.add(RRFarmBonusDef.whiteMarbleFountain);
        }
        if (conditions.getConsecutiveDays() == 50) {
            list.add(RRFarmBonusDef.doubleSwing);
        }
        if (conditions.getFriendsNum() >= 10) {
            list.add(RRFarmBonusDef.roseSeed);
        }
        // 去掉已经获得的奖励
        List<RRFarmBonusModel> aList = new ArrayList<RRFarmBonusModel>();
        for (int i = list.size() - 1; i >= 0; i--) {
            RRFarmBonusModel model = list.get(i);
            if (model.getType() == BonusType.item) {
                int indexOf = conditions.getBonusIdsList().indexOf(
                        Integer.valueOf(model.getItem().getItemId()));
                if (indexOf != -1) {
                    aList.add(model);
                    // list.remove(indexOf);
                }
            }
        }
        list.removeAll(aList);

        return list;
    }

    @Override
    public List<Integer> getGivenBonuses(int hostId) {
        List<Integer> bonuses = new ArrayList<Integer>();
        String ids;
        try {
            ids = rrfarmBonusDAO.getAppsList(hostId);
        } catch (SQLException e) {
            e.printStackTrace();
            return bonuses;
        }
        if (StringUtils.isEmpty(ids)) {
            return bonuses;
        }
        for (String id : ids.split(":")) {
            try {
                bonuses.add(new Integer(id));
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }
        return bonuses;
    }

    @Override
    public String getNotification(String userName, RRFarmBonusModel model) {
        String format1 = "Hi，%s，%s，我们给你赠送了<a href='http://apps.kaixin.com/rr_farm'>%s</a>，<a href='http://apps.kaixin.com/rr_farm'>快去使用吧</a> 或者 <a href='http://www.kaixin.com/RRFarmActivity'>查看活动详情</a> 。";
        String format2 = "Hi，%s，%s，我们给你赠送了<a href='http://apps.kaixin.com/rr_farm'>%s</a>作为奖励，<a href='http://apps.kaixin.com/rr_farm'>快去使用吧</a> 或者 <a href='http://www.kaixin.com/RRFarmActivity'>查看活动详情</a> 。";
        String notification = "";
        if (model.getType() == BonusType.money) {
            if (model.getMoney().getMoney() == 50) {
                notification = String.format(format1, userName, "登录开心网，每天都会有奖品噢", "50个金币");
            }
        }
        if (model.getType() == BonusType.item) {
            if (model == RRFarmBonusDef.oven) {
                notification = String.format(format2, userName, "你收藏了人人农场的游戏", "一个烤肉架");
            }
            if (model == RRFarmBonusDef.cherry) {
                notification = String.format(format2, userName, "你连续3天登录了开心网", "一棵樱桃树");
            }
            if (model == RRFarmBonusDef.turkey) {
                notification = String.format(format2, userName, "你连续5天登录了开心网", "一只火鸡");
            }
            if (model == RRFarmBonusDef.roseSeed) {
                notification = String.format(format2, userName, "你在开心网上加够了10个好友", "玫瑰花种子");
            }
            if (model == RRFarmBonusDef.roseBouquet) {
                notification = String.format(format2, userName, "你连续10天登录了开心网", "一枝花束");
            }
            if (model == RRFarmBonusDef.nest) {
                notification = String.format(format2, userName, "你连续15天登录了开心网", "一个鸟窝");
            }
            if (model == RRFarmBonusDef.whiteMarbleFountain) {
                notification = String.format(format2, userName, "你连续20天登录了开心网", "一个白色大理石喷泉");
            }
            if (model == RRFarmBonusDef.doubleSwing) {
                notification = String.format(format2, userName, "你连续30天登录了开心网", "一个双人秋千");
            }
        }
        return notification;
    }

    @Override
    public int getRRFarmActivateDays(int userId) {
        int consecutiveDays;
        try {
            consecutiveDays = LoginLogicFactory.getPassportUserScore().getLoginConsecutiveCount(
                    userId);
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
        long onlineDays = getOnlineDays();
        consecutiveDays = (int) (onlineDays < consecutiveDays ? onlineDays : consecutiveDays);
        return consecutiveDays;
    }

    /**
     * isInstalledRRFarm<br>
     * 
     * @param userId
     * @return
     * 
     * @author tai.wang@opi-corp.com Mar 12, 2010 - 12:46:01 PM
     */
    public boolean isInstalledRRFarm(int userId) {
        int rrfarmId = 53429; // rrfarm's app id
        try {
            return UserAppHome.getInstance().isAppUser(rrfarmId, userId);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        // return true;
    }

    @Override
    public void notifyTheAppPlusBonus(int userId, RRFarmBonusModel bonus) {
        if (RRFarmBonusModel.BonusType.item == bonus.getType()) {
            LoginLogicFactory.getAppBonusLogic().rewardItem(userId, bonus.getItem().getItemId(),
                    bonus.getItem().getItemNum());
        } else if (RRFarmBonusModel.BonusType.money == bonus.getType()) {
            LoginLogicFactory.getAppBonusLogic().rewardMoney(userId, bonus.getMoney().getMoney());
        } else if (RRFarmBonusModel.BonusType.prodect == bonus.getType()) {
            LoginLogicFactory.getAppBonusLogic().rewardProduct(userId, bonus.getItem().getItemId(),
                    bonus.getItem().getItemNum());
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.xiaonei.login.logic.rrfarm.RRFarmBonusLogic#sendNotificationToYourself
     * (int, com.xiaonei.login.logic.rrfarm.RRFarmBonusModel)
     */
    @Override
    public void sendNotificationToYourself(int userId, RRFarmBonusModel bonus) {
        Set<Integer> receiverIds = new HashSet<Integer>();
        receiverIds.add(userId);
        User user = null;
        try {
            user = SnsAdapterFactory.getUserAdapter().get(userId);
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        if (user == null) {
            try {
                user = SnsAdapterFactory.getUserAdapter().get(userId);
            } catch (Exception e1) {
                e1.printStackTrace();
                return;
            }
        }
        if (Globals.RENREN_DOMAIN.equals(user.getDomain())) {
            return;
        }
        String notification = getNotification(user.getName(), bonus);
        try {
            LoginLoggerDef.login.debug("send notification rrfarm " + user.getId() + ", "
                    + user.getName() + ", " + notification);
            AppNotificationMgr.sendNotification(93472, userId, receiverIds, notification,
                    AppNotificationMgr.NOTIFICATION_TYPE_USER_TO_USER);
        } catch (AppNotificationException e) {
            LoginLoggerDef.login.debug("AppNotificationMgr.sendNotification " + userId + " "
                    + e.toString());
        }
    }

    /**
     * getOnlineDays<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Mar 15, 2010 - 9:44:07 AM
     */
    private long getOnlineDays() {
        try {
            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");
            long to = new Date().getTime();
            String onlineDate = "2010-3-17";
            long from = df.parse(onlineDate).getTime();
            return (to - from) / (1000 * 60 * 60 * 24);
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return 0l;
    }

}
