package com.xiaonei.reg.jump.admin.controllers;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.jump.data.logic.MlinkRegToGameMapLogic;
import com.xiaonei.reg.jump.data.model.MlinkRegToGameMapModel;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * ManageController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 新建文件
 * 2010-9-15 添加了set功能
 */
@Path("admin/jumpmanage")
@LoginRequired
public class ManageController {
    private static final String INIT = "/views/jump/admin/init.jsp";
    private static final String V_SET = "/views/jump/admin/set.jsp";
    //private static final String V_DEL = "/views/jump/admin/del.jsp";
    private static final String V_ADD = "/views/jump/admin/add.jsp";
    private static final String V_EXIT = "r:http://www.renren.com";
    
    private static final String THE_ACTION = "theAction";
    private static final String ACTION_INIT = "init";
    private static final String ACTION_SET = "set";
    private static final String ACTION_SET_SAVE = "set_save";
    private static final String ACTION_ADD = "add";
    private static final String ACTION_ADD_SAVE = "add_save";
    private static final String ACTION_DEL = "del";
    private static final String ACTION_EXIT = "exit";
    
    
    @Get
    @Post
    public String index(Invocation inv,CacheManager cm){
        User host = UserProvider.getInstance().get(cm);
        if(host == null){
            GuideLogger.printLog(" host null!",GuideLogger.WARN);
            return "@Login Required";
        }
        String ret = INIT;
        String action = inv.getParameter(THE_ACTION);
        GuideLogger.printLog(" host:"+host.getId()+" action:"+action);
        if(MyStringUtils.isNullOrEmpty(action)){
            doInit(host,inv);
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_INIT)){
            doInit(host,inv);
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_SET)){
            doSet(host,inv);
            ret = V_SET;
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_SET_SAVE)){
            doSave(host,inv);
            doInit(host,inv);
            ret = INIT;
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_ADD)){
            doAdd(host,inv);
            doInit(host,inv);
            ret = V_ADD;
            
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_ADD_SAVE)){
            doAppend(host, inv);
            doInit(host,inv);
            ret = INIT;
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_DEL)){
            doDel(host,inv);
            doInit(host,inv);
            ret = INIT;
            
        }
        else if(MyStringUtils.equalsIgnoreCase(action, ACTION_EXIT)){
            doExit(host,inv);
            ret = V_EXIT;
            
        }
        
        GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
        return ret;
    }


    private void doExit(User host, Invocation inv) {
        //do nothing
        GuideLogger.printLog(" host:"+host.getId()+" doExit:");
    }


    private void doSave(User host, Invocation inv) {
        MlinkRegToGameMapModel model = extractModel(inv);
        String remark = MyStringUtils.getSubstring(model.getRemark(), 200)+" [by:"+host.getId()+"]";
        model.setRemark(remark);
        MlinkRegToGameMapLogic.getInstance().update(host, model);
        GuideLogger.printLog(" host:"+host.getId()+" doSave:"+model);
    }

    private void doAppend(User host,Invocation inv){
        MlinkRegToGameMapModel model = extractModel(inv);
        String remark = MyStringUtils.getSubstring(model.getRemark(), 200)+" [by:"+host.getId()+"]";
        model.setRemark(remark);
        MlinkRegToGameMapLogic.getInstance().insert(host, model);
        GuideLogger.printLog(" host:"+host.getId()+" doAdd:"+model);
    }

    private MlinkRegToGameMapModel extractModel(Invocation inv) {
        MlinkRegToGameMapModel model = new MlinkRegToGameMapModel();
        
        try {
            model.setId(MyStringUtils.stringToInt(inv.getParameter("id")));
            model.setSs_id(MyStringUtils.stringToInt(inv.getParameter("ss_id")));
            model.setRt_id(MyStringUtils.stringToInt(inv.getParameter("rt_id")));
            model.setX_id(MyStringUtils.stringToInt(inv.getParameter("x_id")));
            model.setY_id(MyStringUtils.stringToInt(inv.getParameter("y_id")));
            model.setGame_stat_id(MyStringUtils.stringToInt(inv.getParameter("game_stat_id")));
            model.setCode_id(inv.getParameter("code_id"));
            model.setTo_url(inv.getParameter("to_url"));
            model.setRemark(inv.getParameter("remark"));
            model.setInsert_time(inv.getParameter("insert_time"));
            model.setUpdate_time(inv.getParameter("update_time"));
        } catch (Exception e) {
            GuideLogger.printLog(" e:"+e.toString(),GuideLogger.ERROR);
            e.printStackTrace();
        }
        return model;
    }

    private void doDel(User host, Invocation inv) {
        MlinkRegToGameMapModel model = new MlinkRegToGameMapModel();
        String theId = inv.getParameter("theId");
        int n_theId = MyStringUtils.stringToInt(theId);
        model.setId(n_theId);
        MlinkRegToGameMapLogic.getInstance().delete(host,model);
        GuideLogger.printLog(" host:"+host.getId()+" doDel:"+model);
    }

    private void doAdd(User host, Invocation inv) {
        inv.addModel("theId",0);
        List<MlinkRegToGameMapModel> list = MlinkRegToGameMapLogic.getInstance().select(host);
        inv.addModel("modellist", list);
        MlinkRegToGameMapModel model = new MlinkRegToGameMapModel();
        model.setTo_url("");
        model.setCode_id("");
        inv.addModel("newitem",model);
        GuideLogger.printLog(" host:"+host.getId()+" doDel:"+model);
    }

    private void doSet(User host, Invocation inv) {
        String theId = inv.getParameter("theId");
        int n_theId = MyStringUtils.stringToInt(theId);
        inv.addModel("theId",n_theId);
        List<MlinkRegToGameMapModel> list = MlinkRegToGameMapLogic.getInstance().select(host);
        for (MlinkRegToGameMapModel model : list) {
            if(model.getId() == n_theId){
                String remark = model.getRemark();
                String [] rs = remark.split("\\[by");
                remark = rs[0];
                model.setRemark(remark);
            }
        }
        inv.addModel("modellist", list);
        GuideLogger.printLog(" host:"+host.getId()+" doSet:"+theId);
    }

    private void doInit(User host,Invocation inv) {
        List<MlinkRegToGameMapModel> list = MlinkRegToGameMapLogic.getInstance().select(host);
        inv.addModel("modellist", list);
        GuideLogger.printLog(" host:"+host.getId()+" doInit:"+list.size());
    }
    
    public static void main(String[] args) {
        String remark = "";
        String [] rs = remark.split("\\[by");
        remark = rs[0];
        System.out.println(remark);
    }
}
