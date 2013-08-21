/**
 *
 */
package com.xiaonei.reg.register.logic.additional;

import com.xiaonei.album.home.AlbumHome;
import com.xiaonei.platform.component.exp.model.DailyExp;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.register.dao.DailyExpDAO;

/**
 * InitializtionCount.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-25 下午06:24:32
 */
public class InitializtionCountLogic {
    private static InitializtionCountLogic _instance = new InitializtionCountLogic();

    public static InitializtionCountLogic getInstance() {
        return _instance;
    }

    private InitializtionCountLogic() {
        super();
    }

    /**
     * 初始化新用户的相册，留言等数量
     *
     * @param oceUser
     * @author (wang-tai)tai.wang@opi-corp.com 2008-9-25 下午06:29:13
     */
    public void init(User oceUser) {

        if(oceUser !=null){
           /* try {
                dao.save(oceUser.getId());
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }*/
    
            try {
                //AlbumHome.getInstance().saveAlbumInfo(oceUser.getId());
            } catch (Exception e) {
                e.printStackTrace(System.err);
            }
    
            try {
                DailyExpDAO.getInstance().save(new DailyExp(oceUser.getId()));
            } catch (Exception e) {
                e.printStackTrace(System.err);
    		}
        }
	}
}
