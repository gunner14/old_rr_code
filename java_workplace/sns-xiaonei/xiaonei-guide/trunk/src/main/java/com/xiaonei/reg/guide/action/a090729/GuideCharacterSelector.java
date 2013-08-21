package com.xiaonei.reg.guide.action.a090729;

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

import org.apache.commons.chain.Chain;
import org.apache.commons.chain.Command;
import org.apache.commons.chain.Context;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.json.JSONArray;
import org.json.JSONObject;

import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.component.friends.selector.SelectorContext;
import com.xiaonei.platform.component.friends.selector.filter.GroupFilter;
import com.xiaonei.platform.component.friends.selector.filter.IPinyinFilter;
import com.xiaonei.platform.component.friends.selector.filter.NetworkFilter;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WFriendDistCount;
import com.xiaonei.platform.core.model.WUserCache;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.action.ProtectAction;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.impl.WFriendDistAdapter;

public abstract class GuideCharacterSelector  extends ProtectAction  {
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

	@SuppressWarnings({ "unchecked", "deprecation" })
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		User host = BaseThreadUtil.currentHost(request);
		String jstring = request.getParameter("p");// json参数
		GuideLogger.printLog(jstring);
		JSONObject param;
		if ((jstring == null) || (jstring.trim().length() == 0)) {
			System.err.println("Selector:Error JSONObject form.1");
			return null;
		}
		try {
			param = new JSONObject(jstring);
		} catch (ParseException e) {
			System.err.println("Selector:Error JSONObject form2.");
			return null;
		}
		JSONObject callerJParam;
		try {
			callerJParam = param.getJSONObject("param");
		} catch (NoSuchElementException e1) {
			System.err.println("Selector:Error JSONObject form3.");
			return null;
		}
		Iterator<String> iter = callerJParam.keys();// 组装成map
		Map<String, String> callerParam = new HashMap<String, String>();

		while (iter.hasNext()) {
			String key = iter.next();
			callerParam.put(key, callerJParam.getString(key));
			GuideLogger.printLog(key);
		}
		
		List<Integer> candidate = getCandidate(host, callerParam);// 取得候选id

		List<WUserCache> ucl = SnsAdapterFactory.getUserCacheAdapter().getUsersCacheList(candidate);

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
				if (userCache != null) {
					candidateMap.put(userCache.getId(), userCache.getName());
				}
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
				joinner.put("netName", userCache.getUnivName());
				retCandidate.put(joinner);
			}
		}
		jo.put("candidate", retCandidate);
		return writeAjax(response, jo.toString());
	}


	@SuppressWarnings("deprecation")
	protected JSONArray getHostNet(int hostId) {
		List<WFriendDistCount> univlist = WFriendDistAdapter.getInstance()
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
		List<String> groups = SnsAdapterFactory.getBuddyGroupAdapter().getGroupList(hostId);
		//List<String> groups = BuddyGroupAdapter.getInstance().getGroupList(hostId);
		JSONArray ja = new JSONArray();
		for (String group : groups) {
			ja.put(group);
		}
		return ja;
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

	protected abstract List<Integer> getCandidate(User host,
			Map<String, String> params);
	protected abstract List<Integer> getCandidateWithRegion(User host,
			Map<String, String> callerParam, RegionInfo info);

	protected abstract IPinyinFilter getPinyinFilter();
}
