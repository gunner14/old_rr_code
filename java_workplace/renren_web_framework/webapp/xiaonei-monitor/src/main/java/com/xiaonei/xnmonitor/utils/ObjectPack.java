package com.xiaonei.xnmonitor.utils;

import java.util.Date;
import java.util.LinkedList;
import java.util.List;

import com.xiaonei.xnmonitor.dto.ErrInfo;
import com.xiaonei.xnmonitor.dto.IMInfo;
import com.xiaonei.xnmonitor.dto.RenRenGroup;

public class ObjectPack {

	public static List<ErrInfo> getErrInfoList(List<Object[]> objectList) {
		List<ErrInfo> errInfoList = new LinkedList<ErrInfo>();
		for (Object[] ob : objectList) {
			ErrInfo info = new ErrInfo();
			// err_type,pre_url,cur_url,count(id) as tota
			info.setErrServer(ob[0].toString());
			info.setErrType(ob[1].toString());
			info.setPreURL(ob[2].toString());
			info.setCurURL(ob[3].toString());
			info.setCounter(UtilTools.strToLong(ob[4].toString()));
			errInfoList.add(info);
		}
		return errInfoList;
	}

	public static List<ErrInfo> getErrInfoFullUrlList(List<Object[]> objectList) {
		List<ErrInfo> errInfoFullUrlList = new LinkedList<ErrInfo>();
		for (Object[] ob : objectList) {
			ErrInfo info = new ErrInfo();
			// err_type,pre_url,cur_url,count(id) as tota
			info.setHappenTime(ob[0].toString());
			if (null == ob[1] || "".equals(ob[1].toString())) {
				info.setPreFulUrl("undefined");
			} else {
				info.setPreFulUrl(ob[1].toString());
			}
			if (null == ob[2] || "".equals(ob[2].toString())) {
				info.setCurFulUrl("undefined");
			} else {
				info.setCurFulUrl(ob[2].toString());
			}
			errInfoFullUrlList.add(info);
		}
		return errInfoFullUrlList;
	}

	public static List<IMInfo> getImInfoList(List<Object[]> objectList) {
		List<IMInfo> imInfoList = new LinkedList<IMInfo>();
		for (Object[] ob : objectList) {
			IMInfo info = new IMInfo();
			// url,type,average(cost_time) as average_cost ,count(id) as total,
			info.setUrlOrHost(ob[0].toString());
			info.setType(ob[1].toString());
			info.setAverageCost(Double.parseDouble(ob[2].toString()));
			info.setTotal(Integer.parseInt(ob[3].toString()));
			imInfoList.add(info);
		}
		return imInfoList;
	}

	public static List<IMInfo> getImInfoDetailList(List<Object[]> objectList) {
		List<IMInfo> imInfoDetailList = new LinkedList<IMInfo>();
		for (Object[] ob : objectList) {
			IMInfo info = new IMInfo();
			// type,cost_time,url,happen_time,IP
			info.setType(ob[0].toString());
			info.setCostTime(UtilTools.strToLong(ob[1].toString()));
			info.setUrlOrHost(ob[2].toString());
			info.setHappenTime(ob[3].toString());
			info.setIP(ob[4].toString());
			imInfoDetailList.add(info);
		}
		return imInfoDetailList;
	}

	/**
	 * @param objectList
	 * @return
	 */
	public static List<IMInfo> getIMDNSDataList(List<Object[]> objectList) {
		List<IMInfo> IMDNSDataList = new LinkedList<IMInfo>();
		for (Object[] ob : objectList) {
			// cost_time,happen_time
			IMInfo iminfo = new IMInfo();
			iminfo.setCostTime(UtilTools.strToLong(ob[0].toString()));
			iminfo.setHappenTime(ob[1].toString());
			IMDNSDataList.add(iminfo);
		}
		return IMDNSDataList;
	}

	public static List<ErrInfo> getDateForHttpErrPic(List<Object[]> result,
			int step) {
		List<ErrInfo> errInfoList = new LinkedList<ErrInfo>();
		for (Object[] objects : result) {
			ErrInfo errInfo = new ErrInfo();
			errInfo.setHappenTimeDate(new Date(UtilTools.strToLong(objects[0]
					.toString())
					* step * 1000));
			errInfo.setTotal(UtilTools.strToInt(objects[1].toString()));
			errInfoList.add(errInfo);
		}
		return errInfoList;
	}

	/**
	 * 负责package Group 返回所有的Group列表，里面包含Group的所有属性的值Group 包含 ugc,sns等
	 * 
	 * @param result
	 * @return 一个包含所有组的List ，里面包含了每个组的所有属性。
	 */
	public static List<RenRenGroup> getRenRenGroupList(List<Object[]> result) {
		List<RenRenGroup> renrenGroupList = new LinkedList<RenRenGroup>();
		for (Object[] objectArr : result) {
			RenRenGroup renrenGroup = new RenRenGroup();
			// group_name | group_ful_name | leader_name | leader_email |
			// group_email | leader_mobile |
			renrenGroup.setGroupName(objectArr[0].toString());
			renrenGroup.setGroupFullName(objectArr[1].toString());
			renrenGroup.setLeaderName(objectArr[2].toString());
			renrenGroup.setLeaderEmail(objectArr[3].toString());
			renrenGroup.setGroupEmail(objectArr[4].toString());
			renrenGroup.setLeaderMobile(objectArr[5].toString());
			renrenGroupList.add(renrenGroup);
		}
		return renrenGroupList;
	}

	/**
	 * 返回包含域名的List<String>
	 * 
	 * @param result
	 *            Ar.sql的结果 List<Object[]>
	 * @return
	 */
	public static List<String> getDomainsList(List<Object[]> resultList) {
		List<String> domainsList = new LinkedList<String>();
		for (Object[] resultArr : resultList) {
			domainsList.add(resultArr[0].toString());
		}
		return domainsList;
	}

}
