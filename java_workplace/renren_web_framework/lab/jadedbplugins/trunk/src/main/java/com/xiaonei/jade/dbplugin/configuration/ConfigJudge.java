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

import com.xiaonei.jade.dbplugin.model.DataModel;

/**
 * ConfigJudge <br>
 * 处理sql中的数据
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 12:10:27 PM
 */
public interface ConfigJudge {

    /**
     * isAllowed<br>
     * 判断DataModel{@link DataModel}是否符合配置
     * 
     * @param data
     * @return
     * 
     * @author tai.wang@opi-corp.com Jul 16, 2010 - 12:11:01 PM
     */
    public boolean isAllowed(DataModel data);
}
