package com.renren.fmwidget.utils;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import com.renren.fmwidget.model.WeatherContent;
import com.renren.fmwidget.model.WeatherData;
import com.renren.fmwidget.model.WeatherData.Date;

/**
 * 天气查询. 推荐使用此 google service 查询<br/>
 * 
 */
public class WeatherUtils {

	/**
	 * 使用 google 查询天气<br/>
	 * 上海: http://www.google.com/ig/api?hl=zh_cn&weather=shanghai
	 * 
	 * @param city
	 *            城市拼音, 如 北京: beijing
	 * @return wuhan : 2010-02-27 天气： 晴, 温度： 8 － 20℃, 湿度： 88%, 风向： 东南、风速：4 米/秒
	 * @throws IOException 
	 * @throws ParserConfigurationException 
	 * @throws SAXException 
	 */
	public static String getweather(String city) throws IOException, SAXException, ParserConfigurationException {
		WeatherContent content = new WeatherContent();
		InputStream in = null;
		BufferedReader reader = null;
		try {
			String ur = "http://www.google.com/ig/api?hl=zh-cn&weather=";
			URL url = new URL(ur + city);
			in = url.openStream();

			reader = new BufferedReader(new InputStreamReader(
					in, "gb2312"));
			String sCurrentLine;
			StringBuilder sTotalString = new StringBuilder();
			while ((sCurrentLine = reader.readLine()) != null) {
				sTotalString.append(sCurrentLine);

			}

			// 读取流
			Document doc = DocumentBuilderFactory.newInstance()
					.newDocumentBuilder().parse(
							new ByteArrayInputStream(sTotalString.toString()
									.getBytes()));
			// 城市:
			content.setCity(city);
			List<WeatherData> datas = new ArrayList<WeatherData>(4);
			content.setData(datas);
			NodeList currNode = doc.getElementsByTagName("current_conditions");
			// System.out.println(currNode.getLength());
			String cur_weather = currNode.item(0).getChildNodes().item(0)
					.getAttributes().item(0).getNodeValue();
			String cur_high = currNode.item(0).getChildNodes().item(2)
					.getAttributes().item(0).getNodeValue();
			WeatherData cur_data = new WeatherData();
			cur_data.setDayTemperature(Integer.valueOf(cur_high));
			cur_data.setDayWeather(weatherMap.get(cur_weather));
			Date cur_date = new Date();
			cur_date.setDay_of_week("今天");
			cur_data.setDate(cur_date);
			datas.add(cur_data);
			NodeList nodelist = doc.getElementsByTagName("forecast_conditions");
			for (int i = 1; i < nodelist.getLength() + 1; i++) {
				Node item = nodelist.item(i);
				if (item == null) {
					continue;
				}
				NodeList nodes = item.getChildNodes();
				String day_of_week = nodes.item(0).getAttributes().item(0)
						.getNodeValue();
				;
				String slow = nodes.item(1).getAttributes().item(0)
						.getNodeValue();
				String shigh = nodes.item(2).getAttributes().item(0)
						.getNodeValue();
				String dayWeather = nodes.item(4).getAttributes().item(0)
						.getNodeValue();
				int low = Integer.valueOf(slow);
				int high = Integer.valueOf(shigh);

				WeatherData data = new WeatherData();
				data.setDayTemperature(high);
				data.setNightTemperature(low);
				data.setDayWeather(weatherMap.get(dayWeather));
				WeatherData.Date date = new WeatherData.Date();
				date.setDay_of_week(day_of_week);
				data.setDate(date);
				datas.add(data);

			}
		} finally {
			if(in!=null) in.close();
			if(reader!=null) reader.close();
		}

		return JSONCover.fromObject(content).toString();
	}

	public static String getCitys() throws IOException {
		InputStream in = null;
		InputStream zh_in = null;
		BufferedReader reader = null;
		BufferedReader zh_reader = null;
		try {
			String ur = "http://www.google.com/ig/cities?country=cn";
			URL url = new URL(ur);
			in = url.openStream();

			reader = new BufferedReader(new InputStreamReader(in, "gb2312"));
			String sCurrentLine;
			StringBuilder sTotalString = new StringBuilder();
			while ((sCurrentLine = reader.readLine()) != null) {
				sTotalString.append(sCurrentLine);

			}

			JSONObject jo = new JSONObject();
			jo = JSONObject.fromObject(sTotalString.toString());
			JSONArray ja = jo.getJSONArray("cities");
			Map<String, String> map = new HashMap<String, String>();
			for (Object oCity : ja) {
				JSONObject jocity = (JSONObject) oCity;
				map.put(",,," + jocity.getLong("lat") + ","
						+ jocity.getLong("lon"), jocity.get("name").toString().replaceAll(" ", ""));
			}

			String zh_ur = "http://www.google.com/ig/cities?hl=zh-cn&country=cn";
			URL zh_url = new URL(zh_ur);
			zh_in = zh_url.openStream();

			zh_reader = new BufferedReader(new InputStreamReader(zh_in,
					"gb2312"));
			String zh_sCurrentLine;
			StringBuilder zh_sTotalString = new StringBuilder();
			while ((zh_sCurrentLine = zh_reader.readLine()) != null) {
				zh_sTotalString.append(zh_sCurrentLine);

			}
			JSONObject zh_jo = new JSONObject();
			zh_jo = JSONObject.fromObject(zh_sTotalString.toString());
			JSONArray zh_ja = zh_jo.getJSONArray("cities");
			JSONArray jacitys = new JSONArray();
			for (Object oCity : zh_ja) {
				JSONObject jocity = (JSONObject) oCity;
				JSONObject newCity = new JSONObject();
				newCity.put("name", map.get(",,," + jocity.getLong("lat") + ","
						+ jocity.getLong("lon")).toString().replaceAll(" ", ""));
				newCity.put("zh_name", jocity.get("name").toString().replaceAll(" ", ""));
				jacitys.add(newCity);

			}
			return jacitys.toString();
		} finally {
			if (in != null)
				in.close();
			if (zh_in != null)
				zh_in.close();
			if (reader != null)
				reader.close();
			if (zh_reader != null)
				zh_reader.close();
		}
	}
	
	private static final Map<String, String> weatherMap = new HashMap<String, String>();
	static{
		weatherMap.put("以晴为主", "sun");
		weatherMap.put("可能有雨", "light_rain");
		weatherMap.put("可能降雪", "light_snow");
		weatherMap.put("多云", "clouds");
		weatherMap.put("小雨", "light_rain");
		weatherMap.put("小雪", "light_snow");
		weatherMap.put("局部多云", "black_cloud");
		weatherMap.put("晴", "sun");
		weatherMap.put("晴间多云", "black_cloud");
		weatherMap.put("烟雾", "black_cloud");
		weatherMap.put("阴", "black_cloud");
		weatherMap.put("雨", "moderate_rain");
		weatherMap.put("雾", "black_cloud");
		weatherMap.put("雾霾", "black_cloud");

	}

	public static void main(String[] args) throws IOException, SAXException, ParserConfigurationException {
		String[] cs = {"Baoding" ,"Beijing" ,"Changchun" ,"Changsha" ,"Chengdu" ,"Chongqing" ,"Dalian" ,"Fuyang" ,"Fuzhou" ,"Ganzhou" ,"Guangzhou" ,"Guiyang" ,"Haikou" ,"Handan" ,"Hangzhou" ,"Harbin" ,"Hefei" ,"Hengyang" ,"Heze" ,"Hohhot" ,"Huanggang" ,"Jinan" ,"Jining" ,"Kunming" ,"Lanzhou" ,"Lhasa" ,"Nanchang" ,"Nanchong" ,"Nanjing" ,"Nanning" ,"Nantong" ,"Nanyang" ,"Ningbo" ,"Qingdao" ,"Shanghai" ,"Shangqiu" ,"Shantou" ,"Shaoyang" ,"Shenyang" ,"Shenzhen" ,"Shijiazhuang" ,"Taiyuan" ,"Tangshan" ,"Tianjin" ,"Urumqi" ,"Weifang" ,"Wenzhou" ,"Wuhan" ,"XiAn" ,"Xiamen" ,"Xining" ,"Xinyang" ,"Xuzhou" ,"Yancheng" ,"Yibin" ,"Yinchuan" ,"Zhengzhou" ,"Zhoukou" ,"Zhumadian"};
		for (String string : cs) {
				getweather(string);
		}
	}

}