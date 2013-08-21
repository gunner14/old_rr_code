package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import java.io.IOException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.struts.action.ActionForward;
import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.component.friends.selector.SelectorContext;
import com.xiaonei.platform.component.friends.selector.filter.GroupFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.NetworkFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.buddygroup.BuddyGroupAdapter;


/**
 * 给好友选择器吐数据的
 * @author inshion(xin.yin@opi-corp.com)
 * @since 2010-10-29
 */
public abstract class AbstractCharacterSelectorController {

    protected abstract List<Integer> getCandidate(User host, Map<String, String> params,
            HttpServletRequest request);

    protected abstract List<Integer> getCandidate(User host, Map<String, String> params);

    protected abstract IPinyinFilter getPinyinFilter();

    /** 在目标host用户上的好友搜索，默认是当前登录用户; 目的是可能在好友中好友搜索 */
    protected ThreadLocal<User> targetHost = new ThreadLocal<User>();

    @Autowired
    private HostHolder hostHolder;

    @SuppressWarnings("unchecked")
    public String get(Invocation inv) {

        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();

        User host = null;
        try {
            host = this.hostHolder.getUser();
            setTargetHost(host);
            if (host == null) {
                throw new Exception("get host error");
            }
        } catch (Exception e) {
            //LogUtils.getInstance().logThrowable("", e);
            return null;
        }
        String jstring = request.getParameter("p");// json参数
        JSONObject param;
        if ((jstring == null) || (jstring.trim().length() == 0)) {
            //LogUtils.getInstance().logThrowable("Selector:(jstring == null) || (jstring.trim().length() == 0).");
            return null;
        }
        try {
            //原始参数
            param = new JSONObject(jstring);
        } catch (ParseException e) {
            //LogUtils.getInstance().logThrowable("Selector:param = new JSONObject(jstring)",e);
            return null;
        }
        JSONObject callerJParam;
        try {
            callerJParam = param.getJSONObject("param");
        } catch (NoSuchElementException e1) {
            //LogUtils.getInstance().logThrowable("Selector:callerJParam = param.getJSONObject(\"param\")", e1);
            return null;
        }
        Iterator<String> iter = callerJParam.keys();// 组装成map
        Map<String, String> callerParam = new HashMap<String, String>();

        while (iter.hasNext()) {
            String key = iter.next();
            callerParam.put(key, callerJParam.getString(key));
        }

        List<Integer> candidate = getCandidate(host, callerParam, inv.getRequest());// 取得候选id
        if (candidate == null) {
            candidate = new ArrayList<Integer>();
        }
        Map<Integer, WUserCache> wucMap = null;
        List<WUserCache> ucl = new ArrayList<WUserCache>();
        try {
            wucMap = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(candidate);
            // 为了保证顺序 
            for (int index = 0; index < candidate.size(); index++) {
                WUserCache wu = wucMap.get(candidate.get(index));
                if (wu != null) {
                    ucl.add(wu);
                }
            }
        } catch (Exception e) {
            //LogUtils.getInstance().logThrowable("SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(candidate)", e);
            ucl = new ArrayList<WUserCache>();
        }
        CharacterChain filter = new CharacterChain();

        JSONObject jo = new JSONObject();
        try {
            if (param.optBoolean("init")) { // 初始化操作
                init(jo, param, getTargetHost(), ucl);
            } else { // 查询操作
                query(filter, param, getTargetHost(), ucl);
            }

            jo.put("candidate", getResult(host.getId(), ucl, param));
            writeAjax(response, jo.toString());
        } catch (Exception e) {
            e.printStackTrace();
            //LogUtils.getInstance().logThrowable("init or query", e);
        }
        return null;
    }

    /**
     * 加上host
     * 
     * @param host
     * @param ucl
     * @param param
     * @return
     */
    protected JSONArray getResult(int host, List<WUserCache> ucl, JSONObject param) {
        JSONArray retCandidate = new JSONArray();
        for (int index = 0; index < ucl.size(); index++) {
            WUserCache userCache = ucl.get(index);
            JSONObject joinner = new JSONObject();
            if (param.optBoolean("uid")) {
                joinner.put("id", userCache.getId());
                joinner.put("isOnLine", userCache.isOnline());
            }
            if (param.optBoolean("uhead")) {
                joinner.put("head", userCache.getTiny_Url());
            }
            if (param.optBoolean("uname")) {
                joinner.put("name", userCache.getName());
            }
            joinner.put("netName", userCache.getUnivName());
            retCandidate.put(joinner);
        }
        return retCandidate;

    }

    protected void init(JSONObject jo, JSONObject param, User host, List<WUserCache> ucl) {

        if (param.optBoolean("group")) {// 取host的分组
            JSONArray ja = getHostGroup(host.getId());
            if (ja != null) {
                jo.put("groups", ja);
            }
        }
        if (param.optBoolean("net")) {// 取好友所在的网络
            JSONArray ja = getHostNet(host.getId());
            if (ja != null) {
                jo.put("net", ja);
            }
        }
        // 查询用的key
        Map<Integer, String> candidateMap = new HashMap<Integer, String>();
        for (WUserCache userCache : ucl) {
            if (userCache != null) {
                candidateMap.put(userCache.getId(), userCache.getName());
            }
        }
        String key = getPinyinFilter().buildPinyinData(candidateMap);
        jo.put("qkey", key);

    }

    protected void query(CharacterChain filter, JSONObject param, User host, List<WUserCache> ucl) {
        try {
            SelectorContext context = new SelectorContext();
            context.setHost(host);
            if (param.optString("group").trim().length() != 0) {
                filter.addCommand(new GroupFilter());
                context.setGroup(param.getString("group"));
            }
            if (param.optString("net").trim().length() != 0) {
                filter.addCommand(new NetworkFilter());
                int net = 0;
                try {
                    net = Integer.parseInt(param.getString("net"));
                } 
                catch (NumberFormatException e){
                	GuideLogger.printLog(" e:"+e.toString(),GuideLogger.ERROR);
                }
                catch (Exception e) {
                	GuideLogger.printLog(" e:"+e.toString(),GuideLogger.ERROR);
                	}
                context.setNetwork(net);
            }
            if ((param.optString("query").trim().length() != 0)
                    && (param.optString("qkey").trim().length() != 0)) {
                filter.addCommand(getPinyinFilter());
                context.setQuery(param.getString("query").trim());
                context.setKey(param.getString("qkey").trim());
                context.setLimit(param.optInt("limit"));
            }
            for (int i = ucl.size() - 1; i >= 0; i--) {
                if (ucl.get(i) == null) {
                    ucl.remove(i);
                    continue;
                }

                try {
                    context.setUser(ucl.get(i));
                    if (!filter.execute(context)) {
                        ucl.remove(i);
                    }
                } catch (Exception e) {
                    //LogUtils.getInstance().logThrowable("", e);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            //LogUtils.getInstance().logThrowable("query in", e);
        }
    }

    protected JSONArray getHostNet(int hostId) {
        List<WFriendDistCount> univlist = SnsAdapterFactory.getFriendDistAdapter()
                .getFriendDistListAll(hostId);
        if ((univlist != null) && !univlist.isEmpty()) {
            // 去掉0网络
            JSONArray ja = new JSONArray();
            for (int i = 0; i < univlist.size(); i++) {
                JSONObject joinner = new JSONObject();
                if (univlist.get(i).getNetworkId() == 0) {
                    univlist.remove(i);
                    break;
                } else {
                    joinner.put("id", univlist.get(i).getNetworkId());
                    joinner.put("name", univlist.get(i).getNetworkName());
                }
                ja.put(joinner);
            }
            return ja;
        }
        return null;
    }

    protected JSONArray getHostGroup(int hostId) {
        List<String> groups = BuddyGroupAdapter.getInstance().getGroupList(hostId);
        JSONArray ja = new JSONArray();
        for (String group : groups) {
            ja.put(group);
        }
        return ja;
    }

    public User getTargetHost() {
        return targetHost.get();
    }

    public void setTargetHost(User targetHost) {
        this.targetHost.set(targetHost);
    }

    protected ActionForward writeAjax(final HttpServletResponse response, final String str) {
        try {
            response.setContentType("text/html; charset=UTF-8");
            response.getWriter().print(str);
            response.getWriter().flush();
            response.getWriter().close();
        } catch (IOException e) {

        } finally {}
        return null;
    }

    static class CharacterChain implements Chain {

        List<Command> commands = new ArrayList<Command>();

        public void addCommand(Command com) {
            commands.add(com);
        }

        public boolean execute(Context con) throws Exception {
            for (Command command : commands) {
                if (command.execute(con) == false) {
                    return false;
                }
            }
            return true;
        }
    }

    public String toString() {
        return "rose AbstractCharacterSelectorController";
    }

}
