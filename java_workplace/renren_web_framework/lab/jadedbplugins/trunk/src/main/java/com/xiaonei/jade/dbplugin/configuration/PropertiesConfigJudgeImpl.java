/*
 * Copyright 2009-2010 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License i distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.xiaonei.jade.dbplugin.configuration;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Properties;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;

import com.xiaonei.jade.dbplugin.configuration.bean.ConfBean;
import com.xiaonei.jade.dbplugin.model.DataModel;

/**
 * PropertiesConfigJudgeImpl <br>
 * 
 * @author tai.wang@opi-corp.com Jul 14, 2010 - 3:58:45 PM
 */
public class PropertiesConfigJudgeImpl implements ConfigJudge, InitializingBean {

	protected Log logger = LogFactory.getLog(this.getClass());

	private List<ConfBean> conf = new ArrayList<ConfBean>();

	private Properties monitorProperties;

	public void setMonitorProperties(Properties monitorProperties) {
		this.monitorProperties = monitorProperties;
	}

	@Override
	public void afterPropertiesSet() throws Exception {
		refresh();
	}

	public void refresh() {
		if ((null == monitorProperties) || monitorProperties.isEmpty()) {
			return;
		}
		List<ConfBean> conf = new ArrayList<ConfBean>();
		for (Object key : monitorProperties.keySet()) {
			ConfBean confBean = new ConfBean();
			conf.add(confBean);
			confBean.setClazz((String) key);
			confBean.setCostHigh(Integer.MAX_VALUE);
			confBean.setCostLow(10);

			String value = StringUtils.trimToEmpty(monitorProperties
					.getProperty((String) key));
			value = StringUtils.remove(value, " ");
			value = StringUtils.remove(value, "\t");
			String[] items = StringUtils.splitByWholeSeparator(value, "),");
			for (String item : items) {
				String[] k_v = StringUtils.split(item, "(");
				if (!ArrayUtils.isEmpty(k_v) && (k_v.length == 2)) {
					if ("cost".equals(k_v[0])) {
						String val = StringUtils.removeEnd(k_v[1], ")");
						// split
						String[] costs = StringUtils.split(val, "-");
						if (!ArrayUtils.isEmpty(costs)) {
							String lowCost = StringUtils.removeEndIgnoreCase(
									costs[0], "ms");
							Integer tl = NumberUtils.createInteger(lowCost);
							if (null != tl) {
								confBean.setCostLow(tl);
							}
							if (costs.length == 2) {
								String highCost = StringUtils
										.removeEndIgnoreCase(costs[1], "ms");
								Integer tu = NumberUtils
										.createInteger(highCost);
								if (null != tu) {
									confBean.setCostHigh(tu);
								}
							}
						}
					}
					if ("time".equals(k_v[0])) {
						String timesStr = StringUtils.removeEnd(k_v[1], ")");
						// split
						String[] timesArr = StringUtils.split(timesStr, ",");
						if (!ArrayUtils.isEmpty(timesArr)) {
							short[][] cal = new short[timesArr.length][2];
							int index = 0;
							for (String timeUnitSrt : timesArr) {
								if (index == timesArr.length) {
									break;
								}
								timeUnitSrt = StringUtils
										.trimToEmpty(timeUnitSrt);
								String[] timeUnitArr = StringUtils.split(
										timeUnitSrt, "-");
								if (null != timeUnitArr
										&& timeUnitArr.length == 2) {
									short c1 = parseTime(timeUnitArr[0]);
									short c2 = parseTime(timeUnitArr[1]);
									cal[index][0] = c1;
									cal[index][1] = c2;
									index++;
								}
							}
							confBean.setCal(cal);
						}
					}
				}
			}
			this.conf = conf;
		}
	}

	private short parseTime(String string) {
		if(StringUtils.isEmpty(string)){
			return -1;
		}
		String[] aTime = StringUtils.split(string, ':');
		if(ArrayUtils.isEmpty(aTime) || aTime.length != 2){
			return -1;
		}
		int time;
		try {
			time = Integer.valueOf(aTime[0])*100;
		} catch (NumberFormatException e) {
			return -1;
		}
		try {
			time += Integer.valueOf(aTime[1]);
		} catch (NumberFormatException e) {
			return -1;
		}
		return (short)time;
	}

	@Override
	public boolean isAllowed(DataModel data) {
		if (logger.isDebugEnabled()) {
			logger.debug("config.getConf() size: " + conf.size());
		}
		for (ConfBean confBean : this.conf) {
			String className = confBean.getClazz();
			String className2 = data.getClassName() + "."
					+ data.getMethodName();

			short[][] cal = confBean.getCal();
			if (logger.isDebugEnabled()) {
				logger.debug("className: " + className + ", className2: "
						+ className2);
			}
			if (ArrayUtils.isEmpty(cal)) {
				return false;
			}
			for (int i = 0; i < cal.length; i++) {
				short[] calendars = cal[i];
				if (ArrayUtils.isEmpty(calendars)) {
					continue;
				}
				if (calendars[0] == -1 || calendars[1] == -1) {
					continue;
				}
				Date now = new Date();
				Calendar nowCal = Calendar.getInstance();
				nowCal.setTime(now);
				short shortNow = (short) (nowCal.get(Calendar.HOUR_OF_DAY) * 100 + nowCal
						.get(Calendar.MINUTE));
				if (shortNow >= calendars[0] && shortNow <= calendars[1]) {
					break;
				}
				if ((i + 1) == cal.length) {
					return false;
				}
			}
			if ("*".equals(className)) {
				if (! valueMatch(confBean, data)) {
					return false;
				}
			} else if (StringUtils.equals(className, className2)) {
				if (! valueMatch(confBean, data)) {
					return false;
				}
			} else if (StringUtils.endsWith(className, ".*")) {
				className = StringUtils.removeEnd(className, ".*");
				if (StringUtils.startsWith(className2, className)) {
					if (! valueMatch(confBean, data)) {
						return false;
					}
				}else{
					return false;
				}
			}

		}

		return true;
	}

	private boolean valueMatch(ConfBean conf, DataModel data) {
		if (logger.isDebugEnabled()) {
			logger.debug("data.getCostTime(): " + data.getCostTime());
			logger.debug("conf.getCostLow(): " + conf.getCostLow());
			logger.debug("conf.getCostHigh(): " + conf.getCostHigh());
			logger.debug("(data.getCostTime() >= conf.getCostLow())&&"
					+ " (data.getCostTime() <= conf.getCostHigh()):"
					+ ((data.getCostTime() >= conf.getCostLow()) && (data
							.getCostTime() <= conf.getCostHigh())));
		}
		if ((data.getCostTime() >= conf.getCostLow())
				&& (data.getCostTime() <= conf.getCostHigh())) {
			return true;
		}
		return false;
	}
	
}
