/**
 *
 */
package com.xiaonei.reg.guide.logic.fillinfo;

import com.xiaonei.reg.guide.dao.ChinaAreaDAO;
import com.xiaonei.reg.guide.pojo.ProCity;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

/**
 * HighSchoolCity.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-11-6 上午11:40:48
 */
public class HighSchoolCity {

    static class SingletonHolder {
        static HighSchoolCity instance = new HighSchoolCity();
    }

    public static HighSchoolCity getInstance() {
        return SingletonHolder.instance;
    }

    private Map<String, String> shortProvinceMap;

    private HighSchoolCity() {
        shortProvinceMap = new HashMap<String, String>();
        shortProvinceMap.put("北京市", "北京市");
        shortProvinceMap.put("天津市", "天津市");
        shortProvinceMap.put("河北省", "河北省");
        shortProvinceMap.put("山西省", "山西省");
        shortProvinceMap.put("内蒙古自治区", "内蒙古");
        shortProvinceMap.put("辽宁省", "辽宁省");
        shortProvinceMap.put("吉林省", "吉林省");
        shortProvinceMap.put("黑龙江省", "黑龙江省");
        shortProvinceMap.put("上海市", "上海市");
        shortProvinceMap.put("江苏省", "江苏省");
        shortProvinceMap.put("浙江省", "浙江省");
        shortProvinceMap.put("安徽省", "安徽省");
        shortProvinceMap.put("福建省", "福建省");
        shortProvinceMap.put("江西省", "江西省");
        shortProvinceMap.put("山东省", "山东省");
        shortProvinceMap.put("河南省", "河南省");
        shortProvinceMap.put("湖北省", "湖北省");
        shortProvinceMap.put("湖南省", "湖南省");
        shortProvinceMap.put("广东省", "广东省");
        shortProvinceMap.put("广西壮族自治区", "广西");
        shortProvinceMap.put("海南省", "河南省");
        shortProvinceMap.put("重庆市", "重庆市");
        shortProvinceMap.put("四川省", "四川省");
        shortProvinceMap.put("贵州省", "贵州省");
        shortProvinceMap.put("云南省", "云南省");
        shortProvinceMap.put("西藏自治区", "西藏");
        shortProvinceMap.put("陕西省", "陕西省");
        shortProvinceMap.put("甘肃省", "甘肃省");
        shortProvinceMap.put("青海省", "青海省");
        shortProvinceMap.put("宁夏回族自治区", "宁夏");
        shortProvinceMap.put("新疆维吾尔自治区", "新疆");
        shortProvinceMap.put("台湾省", "台湾省");
        shortProvinceMap.put("香港特别行政区", "香港");
        shortProvinceMap.put("澳门特别行政区", "澳门");
    }

    /**
     * 获得高中所在省市
     *
     * @param hsCode
     * @return
     * @author (wang-tai)tai.wang@opi-corp.com 2008-11-6 上午11:55:41
     */
    public ProCity getAreaByHighSchoolId(String hsCode) {
        try {
            // 获得省
            String pro = ChinaAreaDAO.getInstance().getProvinceName(hsCode);
            pro = shortProvinceMap.get(pro);
            // 获得市
            String city = ChinaAreaDAO.getInstance().getCityName(hsCode);
            city = "市辖区".equals(city) ? ChinaAreaDAO.getInstance().getDistrictName(hsCode) : city;

            // 构造
            ProCity pc = new ProCity(pro, city);
            return pc;
        } catch (SQLException e) {
            e.printStackTrace();
            return null;
		}
	}
}
