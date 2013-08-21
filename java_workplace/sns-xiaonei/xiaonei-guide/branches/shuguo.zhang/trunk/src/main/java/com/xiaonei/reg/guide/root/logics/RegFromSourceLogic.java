package com.xiaonei.reg.guide.root.logics;

import java.sql.SQLException;

import com.xiaonei.reg.guide.root.daos.RegFromSourceDAO;

public class RegFromSourceLogic {
	private static RegFromSourceLogic instance = new RegFromSourceLogic();

	public static RegFromSourceLogic getInstance() {
		return instance;
	}

	private RegFromSourceLogic() {
		super();
	}
	
	 public String getFromsource(final String id) {
		 String ret = null;
		 try {
			ret = RegFromSourceDAO.getInstance().getFromsourceById(id);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		 return ret;
	 }
	    
}
