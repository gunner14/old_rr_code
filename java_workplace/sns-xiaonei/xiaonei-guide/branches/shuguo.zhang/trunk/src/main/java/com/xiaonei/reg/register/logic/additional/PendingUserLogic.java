/**
 *
 */
package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.platform.core.model.User;

import java.sql.SQLException;

import org.springframework.beans.factory.annotation.Autowired;

/**
 * PendingUserLogic.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-12 下午08:39:26
 */
public class PendingUserLogic {

	@Autowired
	private static com.xiaonei.admin.common.logic.audit.PendingUserLogic p;
	
    public static void save(User user) throws SQLException {
		int pendingWhere  = com.xiaonei.admin.common.logic.audit.PendingUserLogic.REG;
		//com.xiaonei.admin.common.logic.audit.PendingUserLogic.save(user.getId(), 0, pendingWhere);
		p.save(user.getId(), 0, pendingWhere);
	}
}
