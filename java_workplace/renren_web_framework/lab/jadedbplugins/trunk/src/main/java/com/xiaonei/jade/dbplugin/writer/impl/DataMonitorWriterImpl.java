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
package com.xiaonei.jade.dbplugin.writer.impl;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.jade.dbplugin.logger.LogUtil;
import com.xiaonei.jade.dbplugin.writer.DataMonitorWriter;

/**
 * DataMonitorWriteImpl <br>
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 2:20:35 PM
 */
public class DataMonitorWriterImpl implements DataMonitorWriter, Runnable {

    protected final Log logger = LogFactory.getLog(getClass());

    private String content;

    public DataMonitorWriterImpl(String content) {
        this.content = content;
    }

    public String getContent() {
        return content;
    }

    @Override
    public void run() {
        write(content);
    }

    @Override
    public void write(String content) {
    	//content = StringUtils.replaceChars(content, '\n', '$');
    	//content = StringUtils.replaceChars(content, '\r', '$');
	char[] chars = content.toCharArray();
	for(int i = 0; i < chars.length; i++){
		if(chars[i] == '\n'){
			chars[i] = '$';
		}else if(chars[i] == '\r'){
			chars[i] = '$';
		}
	}
        LogUtil.getJadeDBMonitorLogger().info(new String(chars));
    }

}
