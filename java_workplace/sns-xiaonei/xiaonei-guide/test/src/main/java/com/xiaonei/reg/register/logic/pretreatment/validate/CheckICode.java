package com.xiaonei.reg.register.logic.pretreatment.validate;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.icode.verify.CodeValidateInfo;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.constants.IRegMobileWebInfoMap;
import com.xiaonei.reg.register.dao.RegMobileWebInfoMapDAO;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.pretreatment.RegMobileLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import com.xiaonei.reg.thread.timelimit.TimeLimitConfiguration;
import com.xiaonei.reg.thread.timelimit.TimeLimitController;
import com.xiaonei.reg.thread.timelimit.TimeLimitThread;

public class CheckICode extends BaseRegCheckLogic implements IRegMobileWebInfoMap{

    protected static final RegLogger logger = RegLogger.getLogger("regLog", "CheckICode");

    /**
     * 验证码判断
     * 
     * @param request
     * @return
     */
    public static String checkICode(HttpServletRequest req) {
        String cRt;
        int accType = StringUtils.isNotEmpty(req.getParameter("accType")) && StringUtils.isNumeric(req.getParameter("accType")) 
                    ? Integer.valueOf(req.getParameter("accType")) : -1;
        switch (accType) {
            case IAccountType.MOBILE:
                cRt = processMobileDownCode(req);
                break;
            default:
                cRt = processWebCode(req, accType);
        }
        return cRt;
    }

    public static String checkICodeByUserid(HttpServletRequest req,int userid) {
    	String user_key_id = req.getParameter("key_id");
    	String user_icode = req.getParameter("icode");
        String cRt = processWebCodeByUserid(user_key_id, user_icode, userid);
        return cRt;
    }
    
    /**
     * 检查下行验证码
     * 
     * @param req
     * @return
     */
    private static String processMobileDownCode(HttpServletRequest req) {
        String mobile = StringUtils.trimToEmpty(req.getParameter("regEmail"));
        String downCode = req.getParameter("icode");
        try {
            if(RegMobileLogic.getInstance().checkDownCode(mobile, downCode)){
                RegMobileWebInfoMapDAO.getInstance().updateStatus(check_ok, mobile, downCode);
                return OK;
            }else{
                RegMobileWebInfoMapDAO.getInstance().updateStatus(check_fail, mobile, downCode);
                return ERR_CODE_ERR;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return ERR_CODE_ERR;
        }
    }

    /**
     * 检查普通验证码
     * 
     * @param req
     * @return
     */
    private static String processWebCode(HttpServletRequest req, int accType) {
        final String inputIcode = StringUtils.trimToEmpty(req.getParameter("icode"));
        final String account = StringUtils.isEmpty(req.getParameter("regEmail")) ? req
                .getParameter("nicknameId") : req.getParameter("regEmail");
        final String cookieIck = CookieManager.getInstance().getCookie(req, "ick");
        final String key_id = "register_" + req.getParameter("key_id");

        if (null == StringUtils.trimToNull(req.getParameter("icode"))) {
            //TODO 需要优化
            return ERR_CODE_ERR;
        }
        if (null != BaseThreadUtil.currentHost(req)) {
            return ERR_USER_SIGNED;
        }
        // 处理验证码
        logger.debug(account + " " + getICodeInMemCache(cookieIck, key_id));
        logger.debug(account + " " + inputIcode + " " + key_id + " " + cookieIck);
        CodeValidateInfo vinfo = validCode(inputIcode, cookieIck, key_id); 
        //logger.debug(account + " " + vinfo!=null ? ("rank=" +vinfo.getRank() + ",check=" + vinfo.isValidate()): "");
        if (vinfo != null) {
            if(accType == IAccountType.XID){
                //ID注册的情况
                if(vinfo.getRank() != 300 || !vinfo.isValidate()){
                    logger.info(account + " " + "icode err");
                    return ERR_CODE_ERR;
                }
            }else{
                //邮箱注册的情况
                if(!vinfo.isValidate()){
                    logger.info(account + " " + "icode err");
                    return ERR_CODE_ERR;
                }
            }
        }

        logger.info(account + " " + "icode ok");
        return OK;
    }
    
    private static final int IDREG_ICODE_DEFAULT_LEVEL = 600;
    private static String processWebCodeByUserid(String user_key_id,String user_icode, int userid) {
        final String inputIcode = user_icode;
        
        //coocie ick
        //String cookieIck = CookieManager.getInstance().getCookie(req, "ick");
                
        
        //keyid
        String kid = user_key_id;
        if(kid == null ){
        	kid = "";
        }
        kid = kid.trim();
        final String key_id = "register_" + kid;

        // 处理验证码
        logger.debug(userid + " " + getICodeInMemCache(userid+"", key_id));
        logger.debug(userid + " " + inputIcode + " " + key_id + " " + userid+"");
        CodeValidateInfo vinfo = validCode(inputIcode, userid+"", key_id); 
        //logger.debug(account + " " + vinfo!=null ? ("rank=" +vinfo.getRank() + ",check=" + vinfo.isValidate()): "");
        if (vinfo != null) {
            //ID注册&email注册用一样的情况
            if(vinfo.getRank() < IDREG_ICODE_DEFAULT_LEVEL || !vinfo.isValidate()){
                logger.info(userid + " " + "icode err");
                return ERR_CODE_ERR;
            }
        }

        logger.info(userid + " " + "icode ok");
        return OK;
    }
    
    
    private static CodeValidateInfo validCode(final String inputIcode, final String cookieIck,
            final String key_id) {
        TimeLimitConfiguration tlthread = TimeLimitController.getTimeLimit(4, 1l);
        try {
            tlthread.setSingleConfig("validCode", new TimeLimitThread(tlthread) {

                @Override
                public Object work() {
                    return CodeUtil.getInstance().validCodeFull(inputIcode, key_id, cookieIck);
                }
            });
            return (CodeValidateInfo) tlthread.getSingleReturn();
            // return true;
        } catch (Throwable e) {
            e.printStackTrace();
            logger.error("icode mem cache failed");
        }
        return null;
    }

    private static String getICodeInMemCache(String cookieIck, String key_id) {
        if (Globals.DEBUG_ENV) {
            return (String) MemCacheFactory.getInstance().getManagerByPoolName(
                    MemCacheKeys.pool_icode).get(MemCacheKeys.key_icode_user + key_id + cookieIck);
        } else {
            return "not query";
        }
    }

    @Override
    public String check(BaseRegForm regForm) {
        return OK;
    }

}
