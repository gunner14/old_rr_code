package com.xiaonei.reg.guide.tool.generator;

import com.xiaonei.reg.guide.tool.model.Datatable;


/**
 * IGenerator.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 新建文件
 */
public interface IGenerator {
    public String generate(Datatable datatable,int gentype);
    
    public static final int TYPE_MODEL = 1;
    public static final int TYPE_LOGIC = 2;
    public static final int TYPE_DAO = 3;
    
    public static final int TYPE_JSP = 4;
    public static final int TYPE_CONTROLLER_EXTRACT_MODEL = 5;
    
}
