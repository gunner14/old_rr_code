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
package com.xiaonei.jade.dbplugin.configuration.bean;


/**
 * ConfBean <br>
 * 配置信息的一个元
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 12:13:16 PM
 */
public class ConfBean {

    /** 类名 */
    private String clazz;

    /** 耗时最小阀值 */
    private int costLow;

    /** 耗时最大阀值 */
    private int costHigh;
    
    /** 12:34 -> 1234 */
    private short[][]  times;

    @Override
    public String toString() {
        return "ConfBean [clazz=" + clazz + ", costHigh=" + costHigh + ", costLow=" + costLow + "]";
    }

    public String getClazz() {
        return clazz;
    }

    public void setClazz(String clazz) {
        this.clazz = clazz;
    }

    public int getCostLow() {
        return costLow;
    }

    public void setCostLow(int costLow) {
        this.costLow = costLow;
    }

    public int getCostHigh() {
        return costHigh;
    }

    public void setCostHigh(int costHigh) {
        this.costHigh = costHigh;
    }

	/**
	 * @param cal the cal to set
	 */
	public void setCal(short[][] cal) {
		this.times = cal;
		
	}

	/**
	 * @return the cal
	 */
	public short[][] getCal() {
		return times;
	}

}
