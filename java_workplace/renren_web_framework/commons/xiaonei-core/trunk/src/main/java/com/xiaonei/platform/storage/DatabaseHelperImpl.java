package com.xiaonei.platform.storage;

import java.sql.Connection;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.xce.XceAdapter;
 
public class DatabaseHelperImpl extends DatabaseHelper {

	public DatabaseHelperImpl() {
		
	}

	public Connection getConnection() {
		try {
			String qirong_test_upload = System.getProperty("QIRONG_TEST_UPLOAD");
			
			if (qirong_test_upload != null && !"".equals(qirong_test_upload) && !"null".equals(qirong_test_upload))
				return DataAccessMgr.getInstance().getConnection(qirong_test_upload,true);//"storage_node",
						
			return DataAccessMgr.getInstance().getConnection(XceAdapter.DB_UPLOAD, true);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {

		}
		return null ;
	}
	public void closeConnection(Connection con){
		try{
			DataAccessMgr.getInstance().closeConnection(con) ;
		}catch(Exception e){
			e.printStackTrace(System.err) ;
		}
	}
}
