package com.xiaonei.reg.guide.tool.model;


/**
 * Datacolumn.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-13 新建文件
 */
public class Datacolumn {
    private String name;
    private String type;
    private String nullable;
    private String keydefine;
    private String isdefault;
    private String extra;
    
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getType() {
        return type;
    }
    
    public void setType(String type) {
        this.type = type;
    }
    
    public String getNullable() {
        return nullable;
    }
    
    public void setNullable(String nullable) {
        this.nullable = nullable;
    }
    
    public String getKeydefine() {
        return keydefine;
    }
    
    public void setKeydefine(String keydefine) {
        this.keydefine = keydefine;
    }
    
    public String getIsdefault() {
        return isdefault;
    }
    
    public void setIsdefault(String isdefault) {
        this.isdefault = isdefault;
    }
    
    public String getExtra() {
        return extra;
    }
    
    public void setExtra(String extra) {
        this.extra = extra;
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
        
        retValue = "Datacolumn ( "
            + "name = " + this.name + TAB
            + "type = " + this.type + TAB
            + " )";
    
        return retValue;
    }
    
}
