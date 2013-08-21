package com.xiaonei.xnmonitor.controllers.group;

import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.springframework.web.multipart.MultipartFile;

import com.xiaonei.xnmonitor.dto.RenRenGroup;
import com.xiaonei.xnmonitor.utils.UtilTools;

import net.paoding.rose.ar.Ar;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.ReqMapping;

public class GroupClassficationController {
	public String index(Invocation inv) {
		List<RenRenGroup> renrenGroupList = UtilTools.listRenRenGroups();
		inv.addModel("renrenGroupList", renrenGroupList);
		List<String> ungroupedDomainsList = UtilTools.listUngroupedDomains();
		inv.addModel("ungroupedDomainsList", ungroupedDomainsList);
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
		if (!UtilTools.isStrEmptyOrNull(groupName, groupFullName, groupEmail,
				leaderName, leaderEmail, leaderMobile)) {
			String sql = "insert into groups (group_name,group_ful_name,group_email,"
					+ " leader_name,leader_email,leader_mobile) values (?,?,?,?,?,?)";
			Ar.exesql(sql, groupName, groupFullName, groupEmail, leaderName,
					leaderEmail, leaderMobile);
		}
		// return "/group/groupClassfication/createGroup";
		return "groupClassfication-createGroup";
	}

	// public String updateGroupIndex(Invocation inv,
	// @Param("groupName") String groupName,
	// @Param("groupFullName") String groupFullName,
	// @Param("leaderName") String leaderName,
	// @Param("leaderEmail") String leaderEmail,
	// @Param("leaderMobile") String leaderMobile,
	// @Param("groupEmail") String groupEmail) {
	// RenRenGroup renrenGroup = UtilTools.listRenRenGroup(groupName);
	// if (renrenGroup != null) {
	// groupFullName = renrenGroup.getGroupFullName();
	// groupEmail = renrenGroup.getGroupEmail();
	// leaderName = renrenGroup.getLeaderName();
	// leaderEmail = renrenGroup.getLeaderEmail();
	// leaderMobile = renrenGroup.getLeaderName();
	// }
	// return "groupClassfication-updateGroup";
	// }

	public String updateGroup(Invocation inv,
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

		if (!UtilTools.isStrEmptyOrNull(groupName, groupFullName, groupEmail,
				leaderName, leaderEmail, leaderMobile)) {
			String sql = "update groups set group_ful_name=? , "
					+ "leader_name=?,leader_email=?,leader_mobile=?,group_email=? where group_name=?";
			Ar.exesql(sql, groupFullName, leaderName, leaderEmail,
					leaderMobile, groupEmail, groupName);
			return "groupClassfication-updateGroup";

		} else {
			RenRenGroup renrenGroup = UtilTools.listRenRenGroup(groupName);
			if (renrenGroup != null) {
				if (StringUtils.isBlank(groupFullName)) {
					groupFullName = renrenGroup.getGroupFullName();
					inv.addModel("groupFullName",groupFullName);
				}
				if (StringUtils.isBlank(groupEmail)) {
					groupEmail = renrenGroup.getGroupEmail();
					inv.addModel("groupEmail",groupEmail);
				}
				if (StringUtils.isBlank(leaderName)) {
					leaderName = renrenGroup.getLeaderName();
					inv.addModel("leaderName",leaderName);
				}
				if (StringUtils.isBlank(leaderEmail)) {
					leaderEmail = renrenGroup.getLeaderEmail();
					inv.addModel("leaderEmail",leaderEmail);
				}
				if (StringUtils.isBlank(leaderMobile)) {
					leaderMobile = renrenGroup.getLeaderName();
					inv.addModel("leaderMobile",leaderMobile);
				}
			}

			return "groupClassfication-updateGroup";
			// return "/group/groupClassfication/updateGroup";

		}
	}

	public String groupAndDomains(Invocation inv,
			@Param("groupName") String groupName) {
		List<String> groupedDomainsList = UtilTools
				.listGroupedDomains(groupName);
		inv.addModel("groupedDomainsList", groupedDomainsList);
		List<String> ungroupedDomains = UtilTools.listUngroupedDomains();
		inv.addModel("ungroupedDomains", ungroupedDomains);
		return "groupClassfication-groupAndDomains";
	}

	public String deleteGroupDomain(Invocation inv,
			@Param("groupName") String groupName, @Param("domain") String domain) {
		// domain | group_name
		String sql = "Select domain,group_name from domainclassfication where domain=? and group_name=?";
		List<Object[]> resultList = Ar.sql(sql, domain, groupName);
		if (resultList.size() != 0) {
			String deleteSql = "delete from domainclassfication where domain = ? and group_name =? ";
			int changedTotal = Ar.exesql(deleteSql, domain, groupName);
		}

		return "r:groupAndDomains?groupName=" + groupName;
	}

	public String addGroupDomain(Invocation inv,
			@Param("groupName") String groupName, @Param("domain") String domain) {
		if (!StringUtils.isBlank(groupName) && !StringUtils.isBlank(domain)) {
			String sqlExist = "select group_name,domain from domainclassfication where domain=?  ";
			List<Object[]> result = Ar.sql(sqlExist, domain);
			System.out.println("it has the record" + result.size() + "..>>>>");
			if (result.size() < 1) {
				String sql = "insert into domainclassfication(group_name,domain) values (?,?)";
				Ar.exesql(sql, groupName, domain);
			}
		}
		return "r:groupAndDomains?groupName=" + groupName;
	}

	public void deleteGroup() {
		// TODO 删除一个Group
	}
	@ReqMapping(path="test")
	public String test(){
		return "/views/http/test.vm";
	}

}
