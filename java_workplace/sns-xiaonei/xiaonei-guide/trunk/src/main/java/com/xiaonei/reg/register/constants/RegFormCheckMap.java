package com.xiaonei.reg.register.constants;

import com.xiaonei.reg.register.form.base.userganerate.*;
import com.xiaonei.reg.register.logic.pretreatment.validate.*;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class RegFormCheckMap implements Serializable {

    /**  */
    private static final long serialVersionUID = -7291235060641813383L;

    private static Map<Class<?>, Class<? extends BaseRegCheckLogic>> map = null;

    static {
        map = new HashMap<Class<?>, Class<? extends BaseRegCheckLogic>>();
        map.put(IBasicControl.class, CheckBasicControl.class);
        map.put(IBirthday.class, CheckBirthday.class);
        map.put(IGender.class, CheckGender.class);
        map.put(IHeadupload.class, CheckHeadupload.class);
        map.put(IHomecity.class, CheckHomecity.class);
        map.put(IInvite.class, CheckInvite.class);
        map.put(IMiddleschool.class, CheckMiddleschool.class);
        map.put(IName.class, CheckName.class);
        map.put(IPwd.class, CheckPwd.class);
        map.put(IStage.class, CheckStage.class);
        map.put(IUniversity.class, CheckUniversity.class);
        map.put(IWorkplace.class, CheckWorkplace.class);
        map.put(IAccType.class, CheckAccType.class);
    }

    public static Map<Class<?>, Class<? extends BaseRegCheckLogic>> getMap() {
        return map;
    }
}
