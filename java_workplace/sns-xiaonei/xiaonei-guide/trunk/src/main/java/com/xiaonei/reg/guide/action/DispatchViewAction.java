package com.xiaonei.reg.guide.action;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.json.JSONObject;

import com.dodoyo.dispatch.DispatchViewManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.BaseAction;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.xce.log.TimeCost;

public class DispatchViewAction extends BaseAction {

    public final static int TYPE_ALL = 0; //所有新鲜事
    public final static int TYPE_REPLY = 1; //留言回复
    public final static int TYPE_Fangle = 2; //朋友新鲜事
    public final static int TYPE_GROUP = 3; //小圈子新鲜事
    public final static int TYPE_NEW = 4;
    public final static int TYPE_CLASS_MEM_FANGLE = 5;  //班级成员新鲜事
    public final static int TYPE_CLASS_FANGLE = 6;  //班级新鲜事

    private final static Logger logger = Logger.getLogger(DispatchViewAction.class);

    public ActionForward executeBt(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws SQLException {

        TimeCost cost = TimeCost.begin("DispatchViewAction");
        try {
            String ids = request.getParameter("iiiiiiiiii");
            User host = null;
            if (StringUtils.isBlank(ids)) {
                host = BaseThreadUtil.currentHost(request);
            } else {
                host = UserAdapter.getUser(Integer.parseInt(ids));
                GuideLogger.printLog(" host:"+host.getId()+" NOHERE! ",GuideLogger.ERROR);
            }
            cost.endStep("1");
            DispatchViewManager manager = DispatchViewManager.getInstance();
            JSONObject obj = new JSONObject();
            int curpage;
            try {
                curpage = Integer.parseInt(request.getParameter("curpage"));
            } catch (Exception e) {
                curpage = 0;
            }
            //是否显示少数条记录，1：显示少数记录 0：显示全部记录
            int isfirst;
            try {
                isfirst = Integer.parseInt(request.getParameter("isfirst"));
            } catch (Exception e) {
                isfirst = 0;
            }
            //新鲜事类型
            int type;
            try {
                type = Integer.parseInt(request.getParameter("type"));
            } catch (Exception e) {
                type = 0;
            }
            int flag;
            try {
                flag = Integer.parseInt(request.getParameter("f"));
            } catch (Exception e) {
                flag = 0;
            }
            int uid;
            try {
                uid = Integer.parseInt(request.getParameter("uu"));
            } catch (Exception e) {
                uid = 0;
            }
            int userno = host.getId();
            if (flag == 1)
                userno = uid;            
            cost.endStep("2");
            if (type == TYPE_REPLY) {
                cost.endStep("3");
                cost.endStep("4");
                if (isfirst == 0) {
                    cost.endStep("5");
                    obj.put("page", curpage);
                } else {
                    cost.endStep("6");                 
                }
            }		
            else if (type == TYPE_GROUP) {
                cost.endStep("7");   
                cost.endStep("8");
                if (isfirst == 0) {
                    cost.endStep("9");   
                    obj.put("page", curpage);
                } else {   
                    cost.endStep("10");              
                }
            } else if (type == TYPE_CLASS_FANGLE) {
            } else if (type == TYPE_CLASS_MEM_FANGLE) {     
                cost.endStep("11");      
                cost.endStep("12");
                if (isfirst == 0) {           
                    cost.endStep("13");                   
                    obj.put("page", curpage);
                } else {                
                    cost.endStep("14");                   
                }
            } else {
                obj = this.getJSONAllDispatchObj(userno, manager);
                cost.endStep("15");
            }
            response.setContentType("text/html;charset=utf-8");
            PrintWriter pw = response.getWriter();
            pw.println(obj);
            pw.flush();
            pw.close();
            return null;
        } catch (IOException e) {
            logger.error(e);
        } catch (Exception e) {
            logger.error(e);
        } finally {
            cost.endFinally();
        }
        try {
            response.setContentType("text/html;charset=utf-8");
            PrintWriter pw = response.getWriter();
            pw.println("-1");
            pw.flush();
            pw.close();
        } catch (Exception e) {
            logger.error(e);
        }
        return null;
    }

    private JSONObject getJSONAllDispatchObj(int userno, DispatchViewManager manager) {
        JSONObject root = new JSONObject();
        JSONObject jsc = new JSONObject();
        root.put("count", jsc);
        return root;
    }
}
