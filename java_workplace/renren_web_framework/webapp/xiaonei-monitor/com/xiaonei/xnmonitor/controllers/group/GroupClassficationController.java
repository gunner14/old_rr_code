package com.xiaonei.xnmonitor.controllers.group;

import java.util.List;

import com.xiaonei.xnmonitor.dto.RenRenGroup;
import com.xiaonei.xnmonitor.utils.UtilTools;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;

public class GroupClassficationController {
	public String index(Invocation inv) {
		/**
		 * 这部分代码是列出现有的所有Group名称的。
		 */
		List<RenRenGroup> renrenGroupList = UtilTools.listRenRenGroups();
		inv.addModel("renrenGroupList", renrenGroupList);

		/**
		 * 这部分代码是列出所有没有Group的域名的
		 */
		List<String> allNotGroupDomains = UtilTools.listAllNotGroupDomains();
		inv.addModel("allNotGroupDomains", allNotGroupDomains);

		return "groupClassfication-index";
	}

	/**
	 * 创建一个Group
	 * 
	 * @param inv
	 * @param groupName
	 * @param groupFullName
	 * @param leaderName
	 * @param leaderEmail
	 * @param leaderMobile
	 * @param groupEmail
	 * @return
	 */

	public String createGroup(Invocation inv,
			@Param("groupName") String groupName,
			@Param("groupFullName") String groupFullName,
			@Param("leaderName") String leaderName,
			@Param("leaderEmail") String leaderEmail,
			@Param("leaderMobile") String leaderMobile,
			@Param("groupEmail") String groupEmail) {
		/**
		 * 这部分代码是列出现有的所有Group名称的。
		 */
		List<RenRenGroup> renrenGroupList = UtilTools.listRenRenGroups();
		inv.addModel("renrenGroupList", renrenGroupList);
		/**
		 * 判断需要创建的部门是不是已经存在了。
		 */
		if (UtilTools.isGroupExsits(groupName)) {
			inv.addModel("errMsg", "所要添加的组名已经存在，不可以重复创建相同的组");
			return "groupClassfication-createGroup";
		}
		if (UtilTools.isStrNotEmptyAndNotNull(groupName, groupFullName,
				groupEmail, leaderName, leaderEmail, leaderMobile)) {
			String sql = "insert into groups (group_name,group_ful_name,group_email,"
					+ " leader_name,leader_email,leader_mobile) values (?,?,?,?,?,?)";
			System.out.println("GroupFullName"+groupFullName);
			Ar.exesql(sql, groupName, groupFullName, groupEmail, leaderName,
					leaderEmail, leaderMobile);
			inv.addModel("errMsg", "创建Group成功");
		} else {
			inv.addModel("errMsg", "");
		}
		return "groupClassfication-createGroup";
	}

	// public String updateGroup(Invocation inv,
	// @Param("groupName") String groupName,
	// @Param("groupFullName") String groupFullName,
	// @Param("leaderName") String leaderName,
	// @Param("leaderEmail") String leaderEmail,
	// @Param("leaderMobile") String leaderMobile,
	// @Param("groupEmail") String groupEmail,
	// @Param("originGroupName") String originGroupName) {
	// if (UtilTools.isStrNotEmptyAndNotNull(groupName, groupFullName,
	// groupEmail, leaderName, leaderEmail, leaderMobile)) {
	// System.out.println("originGroupName" + originGroupName
	// + "groupName" + groupName);
	// if (!groupName.equals(originGroupName)) {
	// inv.addModel("errMsg", "不能修改GroupName");
	// return "groupClassfication-updateGroup";
	// }
	// String sql = "update groups set group_name=?,group_ful_name=? , "
	// +
	// " group_email=? ,leader_name=?,leader_email=?,leader_mobile=? where group_name=?";
	// int changed = Ar.exesql(sql, groupName, groupFullName, groupEmail,
	// leaderName, leaderEmail, leaderMobile, groupName);
	// System.out.println("changed:" + changed + ":::" + sql);
	// inv.addModel("errMsg", "修改成功");
	// }
	// // RenRenGroup renrenGroup= UtilTools.listRenRenGroup();
	// return "groupClassfication-updateGroup";
	// }

	public String updateGroup(Invocation inv,
			@Param("groupName") String groupName) {
		if (!UtilTools.isStrNotEmptyAndNotNull(groupName)) {
			return "@GroupName is null or the Group is not exits";
		}
		RenRenGroup renRenGroup = UtilTools.getRenRenGroup(groupName);
		inv.addModel("renRenGroup", renRenGroup);
		// RenRenGroup renrenGroup= UtilTools.listRenRenGroup();
		return "groupClassfication-updateGroup";
	}

	public String saveGroupChanges(Invocation inv,
			@Param("groupName") String groupName,
			@Param("groupFullName") String groupFullName,
			@Param("leaderName") String leaderName,
			@Param("leaderEmail") String leaderEmail,
			@Param("leaderMobile") String leaderMobile,
			@Param("groupEmail") String groupEmail,
			@Param("originGroupName") String originGroupName) {
		if (UtilTools.isStrNotEmptyAndNotNull(groupName, groupFullName,
				groupEmail, leaderName, leaderEmail, leaderMobile,
				originGroupName)) {
			if (originGroupName.equals(groupName)) {// 同时不能改变Group的名字。
				String sql = "update groups set group_name=?,group_ful_name=? , "
						+ " group_email=? ,leader_name=?,leader_email=?,leader_mobile=? where group_name=?";
				int changed = Ar.exesql(sql, groupName, groupFullName,
						groupEmail, leaderName, leaderEmail, leaderMobile,
						groupName);
				System.out.println("changed:" + changed + ":::" + sql);
			}

		}
		return "r:/group/groupClassfication/updateGroup?groupName=" + groupName;

	}

	public String groupAndDomains(Invocation inv,
			@Param("groupName") String groupName) {
		/**
		 * 列出这个Group现在已经有了的domain。
		 * 
		 */
		List<String> groupedDomains = UtilTools.getGroupedDomain(groupName);
		/**
		 * 列出所有还没有grouped的domain
		 * 
		 */
		List<String> notGroupedDomains = UtilTools.listAllNotGroupDomains();
		inv.addModel("groupedDomains", groupedDomains);
		inv.addModel("notGroupedDomains", notGroupedDomains);
		return "groupClassfication-groupAndDomains";
	}

	/**
	 * 添加Group和domain现在没有的约束。
	 * 
	 * @param inv
	 * @param groupName
	 * @param domain
	 * @param type
	 * @return
	 */

	public String groupAddDomain(Invocation inv,
			@Param("groupName") String groupName,
			@Param("domain") String domain, @Param("type") String type) {
		// TODO 验证groupName ,domain是否合法 groupname是不是还没有group和存在。
		UtilTools.groupAddDomain(groupName, domain);
		return "r:/group/groupClassfication/groupAndDomains?groupName="
				+ groupName;
	}

	/**
	 * 删除Group和domain 现在已经有的约束
	 * 
	 * @param inv
	 * @param groupName
	 * @param domain
	 * @param type
	 * @return
	 */
	public String groupDelDomain(Invocation inv,
			@Param("groupName") String groupName,
			@Param("domain") String domain, @Param("type") String type) {
		// TODO验证要删除的group 和Domain关系是否合法。
		UtilTools.groupDelDomain(groupName, domain);
		return "r:/group/groupClassfication/groupAndDomains?groupName="
				+ groupName;
	}

	public void deleteGroup() {
		// TODO 删除一个Group
	}

}
