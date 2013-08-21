package com.xiaonei.reg.guide.tool.model;

import java.util.List;



/**
 * Datatable.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 新建文件
 */
public class Datatable {
    private String name;
    private List<Datacolumn> datacolumns;
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public List<Datacolumn> getDatacolumns() {
        return datacolumns;
    }
    
    public void setDatacolumns(List<Datacolumn> datacolumns) {
        this.datacolumns = datacolumns;
    }

    /**
     * Constructs a <code>String</code> with all attributes
     * in name = value format.
     *
     * @return a <code>String</code> representation 
     * of this object.
     */
    public String toString()
    {
        final String TAB = " ";
        
        String retValue = "";
        
        retValue = "Datatable ( "
            + "name = " + this.name + TAB
            + "datacolumns = " + this.datacolumns + TAB
            + " )";
    
        return retValue;
    }
    
    
}
