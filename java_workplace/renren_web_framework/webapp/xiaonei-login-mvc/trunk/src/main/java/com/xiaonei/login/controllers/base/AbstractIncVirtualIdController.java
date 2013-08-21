/**
 * 
 */
package com.xiaonei.login.controllers.base;

import java.util.List;

import net.paoding.rose.web.annotation.AsSuperController;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.PassportAdapterFactory;
import com.xiaonei.xce.passport.Login.Type;



/**
 * @author Shunlong.jin @@version
 *
 */
@AsSuperController
@IgnorePassportValidation
public abstract class AbstractIncVirtualIdController extends AbstractController {

    /**
     * 通过loginVirtual方法来获取Account，考虑Page管理员的情况
     */
    @Override
    protected Account login(List<Type> types, String user, String password, String site,
            int expirePeriod, boolean reuseTicket) {
        return PassportAdapterFactory.getWebLogin().loginVirtual(types, user, password, site, expirePeriod, reuseTicket);
    }


}
