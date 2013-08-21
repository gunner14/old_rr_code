package com.xiaonei.platform.core.opt.jdbc.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

public class GoingDAO {
	private static GoingDAO ins = new GoingDAO();
	public static GoingDAO getInstance(){
		return ins ;
	}
	private static String insert="replace into going values(?,?,now())" ;
    public void adGouYin(final int from,final int to)throws SQLException{
    	
    	OpUpdate op = new OpUpdate(insert,DataAccessMgr.BIZ_GOING_INFO){

    			@Override
    			public void setParam(PreparedStatement ps) throws SQLException {
    				ps.setInt(1, from);
    				ps.setInt(2, to);
    				
    			}

    		};
    		MemCacheManager mem=MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide) ;
    		mem.set(MemCacheKeys.key_going+from+"_"+to,new Boolean(true)) ;
    		//System.out.println("---from:"+from+"|to:"+to+"|.true") ;
    		int returnValue = DataAccessMgr.getInstance().update(op);
    	
    }
    
	private static String deleteByGouYinerId="delete from going where fromid = ? and toid = ?" ;
    public void delGouYinByGyerId(final int gouyiner, final int beigouyiner)throws SQLException{    	
    	OpUpdate op = new OpUpdate(deleteByGouYinerId,DataAccessMgr.BIZ_GOING_INFO){

    			@Override
    			public void setParam(PreparedStatement ps) throws SQLException {
    				ps.setInt(1, gouyiner);
    				ps.setInt(2, beigouyiner);
    				
    			}

    		};
    		MemCacheManager mem=MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide) ;
    		mem.set(MemCacheKeys.key_going+gouyiner+"_"+beigouyiner,new Boolean(false));    
    		DataAccessMgr.getInstance().update(op);    	
    }    
   
    public boolean isGouYinGou(final int from,final int to)throws SQLException{
    	boolean isGoing = false ;
    	MemCacheManager mem=MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide) ;
    	Boolean bl = (Boolean)mem.get(MemCacheKeys.key_going+from+"_"+to) ;
    	//System.out.println("--------------mem.going:"+bl) ;
    	if(bl ==null){
	    	Connection con = null ;
	    	Statement st = null ;
	    	ResultSet rs = null ;
	    	try{
	    	    con = DataAccessMgr.getInstance().getConnection(DataAccessMgr.BIZ_GOING_INFO,true);
	    	    st =con.createStatement() ;
	    	    String sql = "select fromid from going where fromid="+from+" and toid="+to ;
	    	    rs = st.executeQuery(sql) ;
	    	    if(rs.next())
	    	    	isGoing= true ;
	    	    
	    	}finally{
	    		if(rs !=null)
	    		rs.close();
	    		if(st !=null)
	    		st.close();
	    		DataAccessMgr.getInstance().closeConnection(con) ;
	    	}
	    	
	    	mem.set(MemCacheKeys.key_going+from+"_"+to,new Boolean(isGoing)) ;
    	}else
    		isGoing=bl.booleanValue() ;
    	//System.out.println("---from:"+from+"|to:"+to+"|.true="+isGoing) ;
    	return isGoing ;
    	
    	
    }
}
