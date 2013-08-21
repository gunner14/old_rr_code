/**
 * 
 */
package com.xiaonei.login.logic;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactoryUtils;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Service;

import com.xiaonei.login.logic.babordips.BabordIPsManager;
import com.xiaonei.login.logic.rrfarm.AppBonusLogic;
import com.xiaonei.login.logic.rrfarm.AppBonusLogicImpl;
import com.xiaonei.login.logic.rrfarm.RRFarmLogicImpl;
import com.xiaonei.login.logic.status.LoginStatusOprImpl;
import com.xiaonei.login.logic.status.Validation;
import com.xiaonei.login.logic.status.ValidationImpl;
import com.xiaonei.login.logic.ticket.PassportTicketOpr;
import com.xiaonei.login.logic.userscore.PassportUserScore;
import com.xiaonei.login.logic.userscore.PassportUserScoreImpl;

/**
 * LoginLogicFactory <br>
 * 
 * @author tai.wang@opi-corp.com Mar 2, 2010 - 10:22:22 AM
 */
@Service
public class LoginLogicFactory implements ApplicationContextAware {

    private final static Map<Class<?>,Object> map = new ConcurrentHashMap<Class<?>, Object>();

    private static ApplicationContext applicationContext;

    /**
     * getAppBonusLogic<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Mar 11, 2010 - 5:06:39 PM
     */
    public static AppBonusLogic getAppBonusLogic() {
        return AppBonusLogicImpl.getInstance();
    }

    public static BabordIPsManager getBabordIPsManager() {
        return getT(BabordIPsManager.class);
    }

    public static DomainOpr getDomainOpr() {
        return getT(DomainOprImpl.class);
    }

    public static LoginStatusOprImpl getLoginStatusOprImpl() {
        return getT(LoginStatusOprImpl.class);
    }

    public static PassportLogin getPassportLogin() {
        return getT(PassportLoginImpl.class);
    }

    public static PassportTicketOpr getPassportTicketOpr() {
        return getT(PassportTicketOpr.class);
    }

    public static PassportUserScore getPassportUserScore() {
        return getT(PassportUserScoreImpl.class);
    }

    public static RRFarmLogicImpl getRRFarmLogicImpl() {
        return getT(RRFarmLogicImpl.class);
    }

    public static Validation getValidation() {
        return getT(ValidationImpl.class);
    }

    @SuppressWarnings("unchecked")
    private static <T> T getT(final Class<T> clazz) {
        if(map.get(clazz) == null){
            if (applicationContext == null) {
                return null;
            }
            map.put(clazz, BeanFactoryUtils.beanOfType(applicationContext, clazz));
        }
        return (T) map.get(clazz);
    }

    @Override
    public void setApplicationContext(final ApplicationContext applicationContext) throws BeansException {
        LoginLogicFactory.applicationContext = applicationContext;
    }
}
