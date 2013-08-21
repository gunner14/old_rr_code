package com.xiaonei.reg.guide.action;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.logic.ClickStatLogLogic;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.sql.SQLException;

/**
 * <ul>
 * <li>
 * 插入页面点击统计</li>
 * </ul>
 */
public class GuideClickStatAction extends ProtectAction {

    @Override
    public ActionForward executePt(ActionMapping mapping, ActionForm form,
                                   HttpServletRequest request, HttpServletResponse response)
            throws SQLException {
        try {
            User host = BaseThreadUtil.currentHost(request);
            String gl = request.getParameter("gl");
            String url = request.getParameter("url");
            if (!StringUtils.isBlank(url)) {
                try {
                    url = URLDecoder.decode(url, "utf-8");
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
            } else {
                url = "";
            }

            if (gl != null && !"".equals(gl) && !"null".equals(gl)) {
                String[] gls = gl.split("_");
                if (gls.length == 2) {
                    int group = NumberUtils.toInt(gls[0]);
                    int linkid = NumberUtils.toInt(gls[1]);

                    if (group > 0 && linkid > 0) {
                        // 业务类别1：guide页统计
                        // type1:用户阶段
                        // type2:用户登录次数
                        // type3:链接所在区域
                        // type4:链接编号
                        // type5:默认0
                        ClickStatLogLogic.getInstance().insertCountClick(1,
                                host.getId(), UserAdapter.get(host).getStage(),
                                host.getLoginCount(), group, linkid, url);
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        // 无返回值
        return null;
    }
}
