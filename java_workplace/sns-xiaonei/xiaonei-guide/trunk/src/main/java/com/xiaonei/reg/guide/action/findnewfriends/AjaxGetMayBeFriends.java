/**
 * 
 */
package com.xiaonei.reg.guide.action.findnewfriends;

import java.io.IOException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.json.JSONArray;
import org.json.JSONObject;

import com.xiaonei.platform.component.friends.selector.CharacterSelector;
import com.xiaonei.platform.component.friends.selector.SelectorContext;
import com.xiaonei.platform.component.friends.selector.filter.FriendsPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.GroupFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.NetworkFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.ice.WUserCacheAdapter;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * @author Administrator
 *
 */
public class AjaxGetMayBeFriends extends CharacterSelector{
	static class CharacterChain implements Chain {
		List<Command> commands = new ArrayList<Command>();

		public void addCommand(Command com) {
			commands.add(com);
		}

		public boolean execute(Context con) throws Exception {
			for (Command command : commands) {
				if (command.execute(con) == false)
					return false;
			}
			return true;
		}
	}
	
	@Override
	protected List<Integer> getCandidate(User host, Map<String, String> arg1) {
		GuideLogger.writeLine(this , "getCandidate() - hostid:"+ host.getId()+ " - start");
		
		
		GuideLogger.writeLine(this , "getCandidate() - hostid:"+ host.getId()+ " - end");
		return null;
	}

	@SuppressWarnings("unchecked")
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
//		return super.executePt(mapping, form, request, response);
		User host = BaseThreadUtil.currentHost(request);
		
//		host.setUniv(univ)
//		host.set
		
		String jstring = request.getParameter("p");// json参数
		JSONObject param;
		if ((jstring == null) || (jstring.trim().length() == 0)) {
			System.err.println("Selector:Error JSONObject form.");
			return null;
		}
		try {
			param = new JSONObject(jstring);
		} catch (ParseException e) {
			System.err.println("Selector:Error JSONObject form.");
			return null;
		}
		JSONObject callerJParam = param.getJSONObject("param");// 调用者传入参数
		Iterator<String> iter = callerJParam.keys();// 组装成map
		Map<String, String> callerParam = new HashMap<String, String>();

		while (iter.hasNext()) {
			String key = iter.next();
			callerParam.put(key, callerJParam.getString(key));
		}

		List<Integer> candidate = getCandidate(host, callerParam);// 取得候选id
		List<WUserCache> ucl = WUserCacheAdapter.getInstance()
				.getUsersCacheList(candidate);

		if (ucl == null) {
			ucl = new ArrayList<WUserCache>();
		}

		CharacterChain filter = new CharacterChain();

		JSONObject jo = new JSONObject();

		if (!param.optBoolean("init")) {// 查询操作

			SelectorContext context = new SelectorContext();
			context.setHost(host);
			if (param.optString("group").trim().length() != 0) {
				filter.addCommand(new GroupFilter());
				context.setGroup(param.getString("group"));
			}
			if (param.optString("net").trim().length() != 0) {
				filter.addCommand(new NetworkFilter());
				context.setNetwork(Integer.parseInt(param.getString("net")));
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
				}
			}

		} else {// 初始化操作
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
				if (userCache != null)
					candidateMap.put(userCache.getId(), userCache.getName());
			}
			String key = getPinyinFilter().buildPinyinData(candidateMap);
			jo.put("qkey", key);
		}
		// 返回数据
		JSONArray retCandidate = new JSONArray();
		for (WUserCache userCache : ucl) {
			if (userCache != null) {
				JSONObject joinner = new JSONObject();
				if (param.optBoolean("uid")) {
					joinner.put("id", userCache.getId());
				}
				if (param.optBoolean("uhead")) {
					joinner.put("head", userCache.getTiny_Url());
				}
				if (param.optBoolean("uname")) {
					joinner.put("name", userCache.getName());
				}
				retCandidate.put(joinner);
			}
		}
		jo.put("candidate", retCandidate);
		return writeAjax(response, jo.toString());
	}
	

	private ActionForward writeAjax(final HttpServletResponse response,
			final String str) {
		try {
			response.setContentType("text/html; charset=UTF-8");
			response.getWriter().print(str);
			response.getWriter().flush();
			response.getWriter().close();
		} catch (IOException e) {

		} finally {
		}
		return null;
	}
	
	@Override
	protected IPinyinFilter getPinyinFilter() {
		return new FriendsPinyinFilter();
	}

}
