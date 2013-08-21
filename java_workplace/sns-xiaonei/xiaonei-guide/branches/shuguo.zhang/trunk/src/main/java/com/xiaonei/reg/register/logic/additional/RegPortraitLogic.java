package com.xiaonei.reg.register.logic.additional;

import java.sql.SQLException;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.MathUtils;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.UploadDAO;
import com.xiaonei.reg.register.pojo.Upload;
import com.xiaonei.xce.userurl.UserUrlInfo;

public class RegPortraitLogic {
    /**
     * 保存一份密钥到memc中,上传头像业务验证使用
     *
     * @return tsc
     * @author tai.wang@opi-corp.com (2008-10-22 - 下午02:45:54)
     */
    public static String getTsc() {
        String tsc = MathUtils.MD5(Long.toString(System.currentTimeMillis()));
        MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
        String key = "guide_upload_" + tsc;
        mem.set(key, "upload", (int) (MemCacheKeys.minute * 30));
        return tsc;
    }

    /**
     * 获得头像
     *
     * @param portraitId
     * @return com.xiaonei.reg.register.pojo.Upload
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:38:59
     */
    public static Upload getPortrait(String portraitId) {
        com.xiaonei.reg.register.model.Upload up;
        try {
            up = UploadDAO.getInstance().get(portraitId);
        } catch (SQLException e) {
            up = new com.xiaonei.reg.register.model.Upload();
            e.printStackTrace();
        }
        if (null == up) {
            up = new com.xiaonei.reg.register.model.Upload();
        }
        return new Upload(up.getSize1(), up.getSize2(), up.getSize3(), up.getSize4());
    }

    /**
     * 保存头像
     *
     * @param uid
     * @param up
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-11 - 下午08:34:00
     */
    public static void setPortrait(int uid, Upload up) {
        String headUrl = up.getHead();
        String largeUrl = up.getLarge();
        String tinyUrl = up.getTiny();
        String mainUrl = up.getMain();

        //2010-01-20 拆分UserBasic
        /*UserBasic ub = new UserBasic();
        ub.setId(uid);
        ub.setHeadUrl(headUrl);
        ub.setTinyUrl(tinyUrl);
        try {
            UserAdapter.setBasic(ub);
        } catch (Exception e) {
            e.printStackTrace();
        }*/
        
        

        UserUrlInfo userUrlInfo = new UserUrlInfo();
        userUrlInfo.setHeadUrl(headUrl);
        userUrlInfo.setLargeUrl(largeUrl);
        userUrlInfo.setMainUrl(mainUrl);
        userUrlInfo.setTinyUrl(tinyUrl);
        UserAdapter.getUrlAdapter().setUserUrl(uid, userUrlInfo);

        try {
            PendingUserLogic.save(UserAdapter.getUser(uid));
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * 保存头像
     *
     * @param user
     * @param up
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-11 - 下午08:34:14
     */
    public static void setPortrait(User user, Upload up) {
        setPortrait(user.getId(), up);
    }

    /**
     * 用户是否上传过头像
     *
     * @param user
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2009-3-11 - 下午09:46:06
     */
    public static boolean hasUploaded(User user) {
        return !StringUtils.equals(UserAdapter.get(user).getHeadUrl(), Globals.userWomenHeadUrl)
                && !StringUtils.equals(UserAdapter.get(user).getHeadUrl(), Globals.userMenHeadUrl)
                && !StringUtils.equals(UserAdapter.get(user).getHeadUrl(), Globals.userHeadUrl);

    }


    /**
     * 非空 hd开头 字母数字 .jpg结尾
     *
     * @param url
     * @return
     */
    public static boolean isValidUrl(String url) {
        final String[] allowed = {"/", ".", "_"};
        if (StringUtils.isBlank(url)) {
            return false;
        }
        if (!url.startsWith("hd")) {
            return false;
        }
        if (!url.endsWith(".jpg")) {
            return false;
        }
        for (int i = 0; i < url.length(); i++) {
            String c = url.substring(i, i + 1);
            if (!StringUtils.isAlphanumeric(c) && !ArrayUtils.contains(allowed, c)) {
				return false;
			}
		}
		return true;
	}
}
