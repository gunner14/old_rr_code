package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.register.dao.RegNoActivateRecordDAO;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 9, 2010 9:48:32 AM
 * 保存直接激活的用户到表
 */
public class RegNoActivateRecordLogic {
    private static RegNoActivateRecordLogic _instance = new RegNoActivateRecordLogic();

    public static RegNoActivateRecordLogic getInstance() {
        return _instance;
    }

    private RegNoActivateRecordLogic() {
        super();
    }
    
    /**
     * 保存用户的免激活信息
     * @param user
     */
    public void save(User user){
        final int uid = user.getId();
        //考虑异步保存信息
        Thread aaa = new Thread() {
            @Override
            public void run() {
                try {
                    RegNoActivateRecordDAO.getInstance().save(uid);
                } catch (SQLException e) {
                }
            }
        };
        RunnableTaskExecutor.getInstance().runTask(aaa);
    }
}
