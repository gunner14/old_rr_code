package com.xiaonei.reg.register.logic.additional;

import mop.hi.oce.adapter.AdapterFactory;
import mop.hi.oce.domain.network.CollegeInfo;
import mop.hi.oce.domain.network.ElementarySchoolInfo;
import mop.hi.oce.domain.network.HighSchoolInfo;
import mop.hi.oce.domain.network.JuniorHighSchoolInfo;
import mop.hi.oce.domain.network.UniversityInfo;
import mop.hi.oce.domain.network.WorkplaceInfo;

import com.dodoyo.datum.client.DatumInfoClient;
import com.dodoyo.datum.client.DatumNetworkClient;
import com.dodoyo.datum.model.RegionInfo;
import com.xiaonei.platform.core.model.NetworkStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.Workplace;
import com.xiaonei.platform.core.opt.ice.WFriendDistAdapter;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.pojo.RegNetworkHSPojo;
import com.xiaonei.reg.register.pojo.RegNetworkMajorPojo;
import com.xiaonei.reg.register.pojo.RegNetworkReginPojo;
import com.xiaonei.reg.register.pojo.RegNetworkUnivPojo;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;
import com.xiaonei.sns.platform.core.opt.ice.model.UserStage;

/**
 * 注册网络控制 <br>
 * 大学网络 白领网络 地域网络
 * <p/>
 * RegNetworkLogic.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-23 下午08:50:19
 */
public class RegNetworkLogic {
	private static RegNetworkLogic _instance = new RegNetworkLogic();

	public static RegNetworkLogic getInstance() {
		return _instance;
	}

	private RegNetworkLogic() {
		super();
	}

	/**
	 * 设置大学网络
	 * 
	 * @param net
	 * @param networkStatus
	 */
	public void setUnivStageNetwork(final RegNetworkUnivPojo net,
			final int networkStatus) {
		if (0 == net.getUnivsityid()
				&& (null == net.getUnivsityName() || "".equals(net
						.getUnivsityName()))) {
			return;
		}
		this.saveUniv(UserAdapter.getUser(net.getUserid()), 0, net
				.getUnivsityid(), net.getUnivsityName(), net.getDegree(), "",
				networkStatus);
	}

	/**
	 * 设置白领网络
	 * 
	 * @param net
	 * @param networkStatus
	 */
	public void setWorkplaceStageNetwork(final RegNetworkMajorPojo net,
			final int networkStatus) {
		if (0 == net.getNetid()
				&& (null == net.getNetname() || "".equals(net.getNetname()))
				&& (net.getWorkplacename() == null || "".equals(net
						.getWorkplacename()))) {
			return;
		}
		this.saveWorkplace(UserAdapter.getUser(net.getUserid()), net
				.getWorkplacename(), networkStatus);
	}

	/**
	 * 设置地域网络
	 * 
	 * @param net
	 * @param networkStatus
	 */
	public void setReginStageNetwork(final RegNetworkReginPojo net,
			final int networkStatus) {
		// 所在地
		// ################### 加入地域网络 START
		User user = null;
		try {
			user = UserAdapter.getUser(net.getUserid());
		} catch (final Exception ignored) {
		}
		if (null != user) {
			final boolean addOk = this.saveLocation(user,
					net.getHomeprovince(), net.getHomecityname(), net
							.getHomecitycode());
			if (addOk) {
				// 对好友分布进行改变
				WFriendDistAdapter.getInstance().rebuildFriendDist(
						net.getUserid());
			}
		}
		// ################### 加入地域网络 END
	}

	/**
	 * 中学网络通用
	 * 
	 * @param net
	 * @param networkStatus
	 */
	public void setHSNetwork(final RegNetworkHSPojo net, final int networkStatus) {
		final HighSchoolInfo highSchool = new HighSchoolInfo();
		highSchool.setHighSchoolName(net.getHighSchoolName());
		highSchool.setHighSchoolId(net.getJuniorHighSchoolId());
		highSchool.setUserId(net.getUserid());
		try {
			DatumNetworkClient.getInstance()
					.addOneHighSchoolNetwork(UserAdapter.getUser(net.getUserid()), highSchool);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 保存家乡<br>
	 * userbasic
	 * 
	 * @param host
	 * @param hometownProvince
	 * @param hometownCity
	 * @param hometownCityCode
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午10:33:15
	 */
	public void saveHometown(final User host, final String hometownProvince,
			final String hometownCity, final String hometownCityCode) {
		//2010-01-20 拆分UserBasic
		/*final UserBasic ub = WUserBasicAdapter.getInstance().get(host.getId());
		ub.setHomeCity(hometownCity);
		ub.setHomeProvince(hometownProvince);
		ub.setCityCode(hometownCityCode);
		UserAdapter.setBasic(ub);*/
		
		try {
			UserBorn ub = SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
			ub.setHomeCity(hometownCity);
			ub.setHomeProvince(hometownProvince);
			ub.setCityCode(hometownCityCode);
			SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), ub);
		} catch (Exception e) {
			e.printStackTrace();
		}
 		
	}

	/**
	 * 保存高中信息<br>
	 * 网络 资料
	 * 
	 * @param host
	 * @param highSchoolCode
	 * @param highSchoolEnrollYear
	 * @param highSchoolName
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:01:37
	 */
	public void saveHighSchool(final User host, final int highSchoolCode,
			final int highSchoolEnrollYear, final String highSchoolName,
			final boolean isNet, final int status) {
		try {
			final HighSchoolInfo highSchool = new HighSchoolInfo();
			highSchool.setHighSchoolName(highSchoolName);
			highSchool.setHighSchoolId(highSchoolCode);
			highSchool.setEnrollYear(highSchoolEnrollYear);
			highSchool.setUserId(host.getId());
			DatumInfoClient.getInstance()
					.modifyHighSchoolInfo(host, highSchool);
			if (isNet) {
				DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host,
						highSchool);
				// add by erxi.qian 20090225
				setUserbasicUniv(host.getId(), highSchoolCode);
			}
		} catch (final Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 保存技校信息<br>
	 * 网络 资料
	 * 
	 * @param host
	 * @param techSchoolCode
	 * @param techSchoolEnrollYear
	 * @param techSchoolName
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:01:37
	 */
	public void saveTechSchool(final User host, final int techSchoolCode,
			final int techSchoolEnrollYear, final String techSchoolName,
			final boolean isNet, final int status) {
		final CollegeInfo collegeInfo = new CollegeInfo();
		collegeInfo.setCollegeName(techSchoolName);
		collegeInfo.setCollegeId(techSchoolCode);
		collegeInfo.setEnrollYear(techSchoolEnrollYear);
		collegeInfo.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance().modifyCollegeInfo(host, collegeInfo);
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (isNet) {
			try {
				DatumNetworkClient.getInstance().addOneCollSchoolNetwork(host,
						collegeInfo);
			} catch (Exception e) {
				e.printStackTrace();
			}
			// add by erxi.qian 20090225
			setUserbasicUniv(host.getId(), techSchoolCode);
		}
	}

	/**
	 * 保存初中信息<br>
	 * 资料 网络
	 * 
	 * @param host
	 * @param juniorSchoolCode
	 * @param juniorSchoolEnrollYear
	 * @param juniorSchoolName
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:02:10
	 */
	public void saveJuniorSchool(final User host, final int juniorSchoolCode,
			final int juniorSchoolEnrollYear, final String juniorSchoolName,
			final boolean isNet, final int status) {
		final JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
		info.setJunHighentarySchoolId(juniorSchoolCode);
		info.setJunHighentarySchoolName(juniorSchoolName);
		info.setJunHighentarySchoolYear(juniorSchoolEnrollYear);
		info.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance()
					.modifyJuniorHighSchoolInfo(host, info);
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		if (isNet) {
			try {
				DatumNetworkClient.getInstance().addOneJunSchoolNetwork(host,
						info);
			} catch (Exception e) {
				e.printStackTrace();
			}
			// add by erxi.qian 20090225
			setUserbasicUniv(host.getId(), juniorSchoolCode);
		}
	}

	/**
	 * 保存小学信息<br>
	 * 资料
	 * 
	 * @param host
	 * @param elementarySchoolEnrollYear
	 * @param elementarySchoolName
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 上午11:02:46
	 */
	public void saveElementarySchool(final User host,
			final int elementarySchoolEnrollYear,
			final String elementarySchoolName) {
		final ElementarySchoolInfo info = new ElementarySchoolInfo();
		info.setElementarySchoolId(0);
		info.setElementarySchoolName(elementarySchoolName);
		info.setElementarySchoolYear(elementarySchoolEnrollYear);
		info.setUserId(host.getId());
		AdapterFactory.getNetworkAdapter().addElementarySchoolInfo(
				host.getId(), info);
	}

	/**
	 * 保存大学信息
	 * 
	 * @param host
	 * @param univEnrollyear
	 * @param univId
	 * @param univName
	 * @param typeOfCourse
	 * @param department
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:12:40
	 */
	public void saveUniv(final User host, final int univEnrollyear,
			final int univId, final String univName, final String typeOfCourse,
			final String department, final int status) {
		if (host == null) {
			return;
		}
		final UniversityInfo university = new UniversityInfo();
		university.setDepartment(department);
		university.setEnrollYear(univEnrollyear);
		university.setTypeOfCourse(typeOfCourse);
		university.setUniversityId(univId);
		university.setUniversityName(univName);
		university.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance()
					.modifyUniversityInfo(host, university);
		} catch (Exception e1) {
			e1.printStackTrace();
		}

		try {
			DatumNetworkClient.getInstance()
					.addOneUnivNetwork(host, university);
		} catch (Exception e) {
			e.printStackTrace();
		}
		// add by erxi.qian 20090225
		setUserbasicUniv(host.getId(), univId);
	}

	/**
	 * 设置user_basic表里面的univ字段为对应的networkid<br>
	 * 在初始化网络为NetworkStatus.CURRENT_NETWORK时，应进行设置
	 * 
	 * @param userId
	 * @param univ
	 */
	private void setUserbasicUniv(final int userId, final int univ) {
		//2010-01-20 拆分UserBasic
		/*final UserBasic ub = new UserBasic();
		ub.setId(userId);
		ub.setUniv(univ);
		UserAdapter.setBasic(ub);*/
		
		try {
			UserStage us = SnsAdapterFactory.getUserStageAdapter().getUserStage(userId);
			us.setId(userId);
			us.setUniv(univ);
			SnsAdapterFactory.getUserStageAdapter().setUserStage(userId, us);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * 保存居住地
	 * 
	 * @param host
	 * @param locationProvince
	 * @param locationCity
	 * @param locationCode
	 * @return true ok false fail 网络资料完全添加失败，认为失败
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:39:48
	 */
	public boolean saveLocation(final User host, final String locationProvince,
			final String locationCity, final int locationCode) {
		boolean rt = true;
		try {
			RegionInfo regionInfo = new RegionInfo();
			regionInfo.setCity_name(locationCity);
			regionInfo.setProvince_name(locationProvince);
			regionInfo.setRegion_id(locationCode);
			regionInfo.setUserid(host.getId());
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host,
					regionInfo);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}

		final boolean isWorkplaceUser = RegUtil.isWorkplaceEmail(host
				.getAccount());
		if (!isWorkplaceUser) {
			//2010-01-20 拆分UserBasic
			/*final UserBasic oceBasic = new UserBasic();
			oceBasic.setId(host.getId());
			oceBasic.setUniv(locationCode);
			UserAdapter.setBasic(oceBasic);*/
			try {
				UserStage us = SnsAdapterFactory.getUserStageAdapter().getUserStage(host.getId());
				us.setId(host.getId());
				us.setUniv(locationCode);
				SnsAdapterFactory.getUserStageAdapter().setUserStage(host.getId(), us);
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
		return rt;
	}

	/**
	 * 保存公司
	 * 
	 * @param host
	 * @param company
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:40:11
	 * @deprecated 使用 saveWorkplace
	 */
	public void saveCompany(final User host, final String company) {
		this.saveWorkplace(host, company, NetworkStatus.CURRENT_NETWORK);
	}

	/**
	 * 保存公司信息
	 * 
	 * @param host
	 * @param company
	 * @param status
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-10 - 上午10:42:59
	 */
	public void saveWorkplace(final User host, final String company,
			final int status) {
		if (host == null) {
			return;
		}
		final Workplace wps = RegUtil.getWorkplace(host.getAccount());
		if (wps != null) {
			WorkplaceInfo info = new WorkplaceInfo();
			info.setWorkplaceName(company);
			info.setUserId(host.getId());
			try {
				DatumInfoClient.getInstance().modifyWorkplaceInfo(host, info);
			} catch (Exception e1) {
				e1.printStackTrace();
			}
			try {
				DatumNetworkClient.getInstance().addOneWorkNetwork(host, info);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else if (company != null && !company.equalsIgnoreCase("")) {
			// 白领注册，输入了公司，但是上面的邮箱判断没被添加
			final WorkplaceInfo info = new WorkplaceInfo();
			info.setUserId(host.getId());
			info.setWorkplaceName(company);
			AdapterFactory.getNetworkAdapter().addWorkplaceInfo(info);
		}
	}
}
