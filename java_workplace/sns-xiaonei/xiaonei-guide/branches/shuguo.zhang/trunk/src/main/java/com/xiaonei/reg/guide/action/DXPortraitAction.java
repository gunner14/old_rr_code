package com.xiaonei.reg.guide.action;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.platform.core.head.HeadManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.ImgPro;
import com.xiaonei.reg.guide.form.PortraitForm;
import com.xiaonei.reg.guide.logic.portrait.CommonPortraitLogic;
import com.xiaonei.reg.guide.pojo.CommonPortraitPojo;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import magick.MagickImage;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.upload.FormFile;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.sql.SQLException;
import java.util.Map;

/**
 * @author dream
 * @date 2008-10-21
 * @deprecated
 */
public class DXPortraitAction extends ProtectAction {

    @SuppressWarnings("unchecked")
    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        User host = BaseThreadUtil.currentHost(request);
        GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
        UserTraceLogic.getInstance().insertRegHis(request, host,
                host.getAccount(), String.valueOf(RegOprDefine.UPLAOD_HEAD),
                UserAdapter.get(host).getStage(), null, null, null, null, 0);
        String succ = "succ";
        String successUrl = Globals.urlGuide + "guide.do";
        request.setAttribute("sex_flag", UserAdapter.get(host).getGender());

        if (!CommonPortraitLogic.getInstance().maxLengthExceeded(request)) {
            BaseActionUtil.addErr(request, "文件太大");
            return mapping.findForward(succ);
        }

        request.setAttribute("userDesc", CommonPortraitLogic.getInstance().getUserDesc(host));
        if (!"post".equalsIgnoreCase(request.getMethod())) {
            return mapping.findForward(succ);
        }
        PortraitForm portraitForm = (PortraitForm) form;
        FormFile file = portraitForm.getPortrait();
        if (file == null) {
            addErr(request, "请上传照片");
            return mapping.findForward(succ);
        }
        String filename = file.getFileName().toLowerCase();//图片的url地址
        if (CommonPortraitLogic.getInstance().isValidImg(filename)) {
            addErr(request, "无效的照片格式");
            return mapping.findForward(succ);
        }

        MagickImage source = null;
        try {
            source = ImgPro.getMagickImage(file.getFileData());
            String comment = "xiaonei" + host.getId();
            if (!ImgPro.checkcomment(source, comment)) {
                addErr(request, "不能盗用别人照片");
                return mapping.findForward(succ);
            }
            source = ImgPro.setcomment(source, comment);
            Map<String, String> heads = HeadManager.getInstance().processHead(source);

            CommonPortraitPojo b = new CommonPortraitPojo();
            b.setHead(heads.get(HeadManager.HEADNAME_HEAD));
            b.setMain(heads.get(HeadManager.HEADNAME_MAIN));
            b.setLarge(heads.get(HeadManager.HEADNAME_LARGE));
            b.setTiny(heads.get(HeadManager.HEADNAME_TINY));

            CommonPortraitLogic.getInstance().updateHeadUrl(host, b);

//            host.setLargeUrl(heads.get(HeadManager.HEADNAME_LARGE));
//            host.setHeadUrl(heads.get(HeadManager.HEADNAME_HEAD));
            

            // 存入加星系统
            CommonPortraitLogic.save(host);

            //如果是被邀请的用户，写入memcache，给home页用,上传头像成功才执行这一步
            CommonPortraitLogic.getInstance().setNewInvitee2Mem(host.getId());
        } catch (Exception e) {
        	LogTimeUtils.logger.error(e);
        } finally {
            file.destroy();
            if (source != null)
                source.destroyImages();
        }
        return new ActionForward(successUrl, true);
    }


}
