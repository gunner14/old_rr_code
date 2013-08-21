package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.portrait.PendingUserLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.Utils;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;

/**
 * 白领阶段的上传头像页
 *
 * @author dream
 * @date 2008-10-13
 */
public class BLPortraitAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form, HttpServletRequest request,
                                   HttpServletResponse response) throws SQLException {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        request.setAttribute("sex_flag", UserAdapter.get(host).getGender());

        String skip = request.getParameter("skip");
        /*if(skip != null && "1".equalsIgnoreCase(skip)){//如果MSN导入是点击跳过跳到的这一步，则在这里计入log
              UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(),
                      String.valueOf(RegOprDefine.IMPORT_MSN_SKIP), host.getStage(), null, null, null, null, 0);
          }*/
        // 当上传头像成功之后则进行跳转
        if (!Utils.getInstance().validateForString(skip) && "2".equalsIgnoreCase(skip)) {
            // 存入加星系统
            try {
                PendingUserLogic.save(host);
            } catch (Exception e) {
                e.printStackTrace();
            }
            UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(),
                    String.valueOf(RegOprDefine.UPLAOD_HEAD_RESULT), UserAdapter.get(host).getStage(), null, null, null, null, 0);
            return mapping.findForward("skip");
        }
        // 页面初始化
        else {

            // 如果在审核过程中上传过头像，就直接跳过本步骤
            if (!UserAdapter.get(host).getHeadUrl().equals(Globals.userMenHeadUrl)
                    && !(UserAdapter.get(host).getHeadUrl().equals(Globals.userWomenHeadUrl))
                    && !(UserAdapter.get(host).getHeadUrl().equals(Globals.userHeadUrl))) {
                /*UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(),
                            String.valueOf(RegOprDefine.UPLAOD_HEAD_AUTO_SKIP), host.getStage(), null, null, null, null, 0);*/
                return new ActionForward(Globals.urlGuide + "/guide.do?skip=1", true);
            }
            // 跳转到head.upload域下
            else {
                /*UserTraceLogic.getInstance().insertRegHis(request, host, host.getAccount(),
                            String.valueOf(RegOprDefine.UPLAOD_HEAD), host.getStage(), null, null, null, null, 0);*/
                String sex_flag = "1";
                if ("女".equalsIgnoreCase(UserAdapter.get(host).getGender())) {
                    sex_flag = "0";
                }
                return new ActionForward("http://head.upload."+Globals.domain+"/portraitbl.do?sex_flag=" + sex_flag, true);
            }
        }

    }
}
