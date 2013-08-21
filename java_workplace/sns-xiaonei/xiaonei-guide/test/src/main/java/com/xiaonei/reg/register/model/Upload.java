package com.xiaonei.reg.register.model;

import java.io.Serializable;

public class Upload implements Serializable {
    private static final long serialVersionUID = -8362980031353656196L;
    private int id;
    private String size1;
    private String size2;
    private String size3;
    private String size4;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    /**
     * large
     *
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:28:52
     */
    public String getSize1() {
        return size1;
    }

    public void setSize1(String size1) {
        this.size1 = size1;
    }

    /**
     * main
     *
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:29:05
     */
    public String getSize2() {
        return size2;
    }

    public void setSize2(String size2) {
        this.size2 = size2;
    }

    /**
     * head
     *
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:29:27
     */
    public String getSize3() {
        return size3;
    }

    public void setSize3(String size3) {
        this.size3 = size3;
    }

    /**
     * tiny
     *
     * @return
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-15 - 上午10:29:32
     */
    public String getSize4() {
        return size4;
    }

    public void setSize4(String size4) {
		this.size4 = size4;
	}

	
}
