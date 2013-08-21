/**
 *
 */
package com.xiaonei.reg.guide.logic;

import java.sql.SQLException;

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
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;
import com.xiaonei.sns.platform.core.opt.ice.model.UserStage;

/**
 * FillInfoLogic
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-6-14 - 下午05:43:24
 */
final public class FillInfoLogic {

	private static final FillInfoLogic _INSTANCE = new FillInfoLogic();

	public static FillInfoLogic getInstance() {
		return _INSTANCE;
	}

	private FillInfoLogic() {
		super();
	}

	/**
	 * 保存保存性别<br>
	 * User
	 * 
	 * @param host
	 * @param gender
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-4 - 下午08:17:35
	 */
	public void saveGender(User host, String gender) {
		//2010-01-20 拆分UserBasic
		/*UserBasic userBasic = new UserBasic();
		userBasic.setGender(gender);
		WUserBasicAdapter.getInstance().setUserBasic(host.getId(), userBasic);*/
		try {
			UserBorn ub = SnsAdapterFactory.getUserBornAdapter().getUserBorn(host.getId());
			ub.setGender(gender);
			SnsAdapterFactory.getUserBornAdapter().setUserBorn(host.getId(), ub);
		} catch (Exception e) {
			GuideLogger.printLog("");
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
	public void saveHometown(User host, String hometownProvince,
			String hometownCity, String hometownCityCode) {
		//2010-01-20 拆分UserBasic
		/*UserBasic ub = WUserBasicAdapter.getInstance().get(host.getId());
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
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
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
	public void saveHighSchool(User host, int highSchoolCode,
			int highSchoolEnrollYear, String highSchoolName, boolean isNet,
			int status) {
	    GuideLogger.printLog(" host:"+host.getId()+" highSchoolCode:"+highSchoolCode+" isNet:"+isNet);
		HighSchoolInfo highSchool = new HighSchoolInfo();
		highSchool.setHighSchoolName(highSchoolName);
		highSchool.setHighSchoolId(highSchoolCode);
		highSchool.setEnrollYear(highSchoolEnrollYear);
		highSchool.setUserId(host.getId());
		try {
			//inshion 下面这行灵异的代码是做什么的？我很忧愁啊……
		    //highSchool.setId(AdapterFactory.getNetworkAdapter().addHighSchoolInfo(highSchool).getId());
			DatumInfoClient.getInstance()
					.modifyHighSchoolInfo(host, highSchool);
		} catch (Exception e) {
		    GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
			e.printStackTrace();
		}

		if (isNet) {
			try {
				DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host,
						highSchool);
			} catch (Exception e) {
			    GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString());
				e.printStackTrace();
			}

			// add by erxi.qian 20090225
			setUserbasicUniv(host.getId(), highSchoolCode);
		}
	}

	public void saveHighSchool(User host, int highSchoolCode,
			int highSchoolEnrollYear, String highSchoolName, String Class1,
			String Class2, String Class3, boolean isNet, int status) {
		HighSchoolInfo highSchool = new HighSchoolInfo();
		highSchool.setHighSchoolName(highSchoolName);
		highSchool.setHighSchoolId(highSchoolCode);
		highSchool.setEnrollYear(highSchoolEnrollYear);
		highSchool.setHClass1(Class1);
		highSchool.setHClass2(Class2);
		highSchool.setHClass3(Class3);
		highSchool.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance()
					.modifyHighSchoolInfo(host, highSchool);
		} catch (Exception e) {
			e.printStackTrace();
		}

		if (isNet) {
			try {
				DatumNetworkClient.getInstance().addOneHighSchoolNetwork(host,
						highSchool);
			} catch (Exception e) {
				e.printStackTrace();
			}

			// add by erxi.qian 20090225
			setUserbasicUniv(host.getId(), highSchoolCode);
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
	public void saveTechSchool(User host, int techSchoolCode,
			int techSchoolEnrollYear, String techSchoolName, boolean isNet,
			int status) {
		CollegeInfo collegeInfo = new CollegeInfo();
		collegeInfo.setCollegeName(techSchoolName);
		collegeInfo.setCollegeId(techSchoolCode);
		collegeInfo.setEnrollYear(techSchoolEnrollYear);
		collegeInfo.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance().modifyCollegeInfo(host, collegeInfo);
		} catch (Exception e1) {
			e1.printStackTrace();
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
	public void saveJuniorSchool(User host, int juniorSchoolCode,
			int juniorSchoolEnrollYear, String juniorSchoolName, boolean isNet,
			int status) {
		JuniorHighSchoolInfo info = new JuniorHighSchoolInfo();
		info.setJunHighentarySchoolId(juniorSchoolCode);
		info.setJunHighentarySchoolName(juniorSchoolName);
		info.setJunHighentarySchoolYear(juniorSchoolEnrollYear);
		info.setUserId(host.getId());
		try {
			DatumInfoClient.getInstance()
					.modifyJuniorHighSchoolInfo(host, info);
		} catch (Exception e) {
			e.printStackTrace();
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
	public void saveElementarySchool(User host, int elementarySchoolEnrollYear,
			String elementarySchoolName) {
		ElementarySchoolInfo info = new ElementarySchoolInfo();
		info.setElementarySchoolId(0);
		info.setElementarySchoolName(elementarySchoolName);
		info.setElementarySchoolYear(elementarySchoolEnrollYear);
		info.setUserId(host.getId());
		try {
			AdapterFactory.getNetworkAdapter().addElementarySchoolInfo(
					host.getId(), info);
		} catch (Exception e) {
			e.printStackTrace();
		}
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
	public void saveUniv(User host, int univEnrollyear, int univId,
			String univName, String typeOfCourse, String department, int status) {
		UniversityInfo university = new UniversityInfo();
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
		}

		try {
			DatumNetworkClient.getInstance()
					.addOneUnivNetwork(host, university);
		} 
		catch (Ice.UnknownException e){
			GuideLogger.printLog(" host:"+host.getId()+" "+e.toString(),GuideLogger.ERROR);
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		// add by erxi.qian 20090225
		setUserbasicUniv(host.getId(), univId);
	}

	/**
	 * 设置user_basic表里面的univ字段为对应的networkid<br>
	 * 在初始化网络为.CURRENT_NETWORK时，应进行设置
	 * 
	 * @param userId
	 * @param univ
	 */
	private void setUserbasicUniv(int userId, int univ) {
		//2010-01-20 拆分UserBasic
		/*UserBasic ub = new UserBasic();
		ub.setId(userId);
		ub.setUniv(univ);
		UserAdapter.setBasic(ub);*/
		
		try {
			UserStage us = SnsAdapterFactory.getUserStageAdapter().getUserStage(userId);
			us.setId(userId);
			us.setUniv(univ);
			SnsAdapterFactory.getUserStageAdapter().setUserStage(userId, us);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+userId+" e:"+e.toString(),GuideLogger.ERROR);
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
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:39:48
	 */
	public void saveLocation(User host, String locationProvince,
			String locationCity, int locationCode) {
		try {
			RegionInfo regionInfo = new RegionInfo();
			regionInfo.setCity_name(locationCity);
			regionInfo.setProvince_name(locationProvince);
			regionInfo.setRegion_id(locationCode);
			regionInfo.setUserid(host.getId());
			DatumInfoClient.getInstance().modifyRegionInfoAndNet(host,
					regionInfo);
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		setUserbasicUniv(host.getId(), locationCode);
		RegLogger.getLoggerWithType(this.getClass()).info(
				"set city code " + host + " " + locationCode + " "
						+ locationCity + " " + locationProvince);
	}

	public void saveLocation(User host, String locationProvince,
			String locationCity, int locationCode, int net) {
		this.saveLocation(host, locationProvince, locationCity, locationCode);
	}

	/**
	 * 保存公司
	 * 
	 * @param host
	 * @param company
	 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午12:40:11
	 */
	public void saveCompany(User host, String company) {
		try {
			WorkplaceInfo workplace = new WorkplaceInfo();
			workplace.setUserId(host.getId());
			workplace.setWorkplaceName(company);
			AdapterFactory.getNetworkAdapter().addWorkplaceInfo(workplace);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+host.getId()+" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
	}

	/**
	 * 保存stage
	 * 
	 * @param stage
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:39:01
	 */
	public void saveStage(User user, int stage) {
		try {
			UserStage stg = SnsAdapterFactory.getUserStageAdapter().getUserStage(user.getId());
			//2010-01-20 拆分UserBasic
			//去掉UB的调用 
			//UserBasic ub = WUserBasicAdapter.getInstance().get(user.getId());
			/*if(ub != null){
				ub.setStage(stage);
				UserAdapter.setBasic(ub);
			}*/
			if(stg != null){
				stg.setStage(stage);
				SnsAdapterFactory.getUserStageAdapter().setUserStage(user.getId(), stg);
			}
			else{
				System.err.println("ub is null Exception User id: " + user.getId());
			}
		} catch (Exception e) {
			System.err.println("Exception User id: " + user.getId());
			e.printStackTrace();
		}
	}

}
