package com.xiaonei.passport.configure.impl;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


import com.xiaonei.passport.configure.ILoginConfigure;
import com.xiaonei.passport.configure.ILoginConfigureDao;
import com.xiaonei.passport.model.LoginRegistration;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;

public class LoginConfigureDaoImpl implements ILoginConfigureDao {
	private final static Log logger = LogFactory.getLog(LoginConfigureDaoImpl.class);
	private static ILoginConfigureDao _instance=new LoginConfigureDaoImpl();
	private LoginConfigureDaoImpl(){
		
	}
	public static ILoginConfigureDao getInstance(){
		return _instance;
	}
	@Override
	public Map<String, List<String>> findLoginConfigure() {
		try {
			class Configure{
				int id;
				String key;
				String value;
				int order;
			}
			OpList op =new OpList(ILoginConfigureDao.selectLoginConfigureSQL, ILoginConfigureDao.DATA_SOURCE){
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				Configure configure=new Configure();
				configure.id=rs.getInt("login_registration_id");
				configure.key=rs.getString("configure_key");
				configure.value=rs.getString("configure_value");
				configure.order=rs.getInt("configure_order");
				return configure;
			}
			
		};
	
			List<Configure>	list= (List<Configure>)DataAccessMgr.getInstance().queryList(op);
			Collections.sort(list, new Comparator<Configure>() {

				@Override
				public int compare(Configure o1, Configure o2) {
					return o1.order-o2.order;
				}
			});
			Map<String, List<String>> result=new HashMap<String, List<String>>();
			for(Configure configure: list){
				Integer id=configure.id;
				String key=id+ILoginConfigure.SPLIT_SIGN+configure.key;
				List<String> value=null;
				if(!result.containsKey(key)){
					value=new ArrayList<String>();
					result.put(key, value);
				}
				value=result.get(key);
				value.add(configure.value);
			}
			return result;
		} catch (SQLException e) {
			e.printStackTrace();
			logger.error("findLoginConfigure execute  error", e);
			return new HashMap<String, List<String>>();
		}
	}
	@SuppressWarnings("unchecked")
	@Override
	public List<LoginRegistration> findLoginRegistration() {
		try {
			OpList op =new OpList(ILoginConfigureDao.selectLoginRegistrationSQL, ILoginConfigureDao.DATA_SOURCE){
			@Override
			public Object parse(ResultSet rs) throws SQLException {
				LoginRegistration lr=new LoginRegistration();
				lr.setId(rs.getInt("id"));
				lr.setName(rs.getString("name"));
				lr.setCreateTime(rs.getDate("create_time"));
				return lr;
			}
			
		};
	
			return (List<LoginRegistration>)DataAccessMgr.getInstance().queryList(op);
		} catch (SQLException e) {
			logger.error("findLoginRegistration execute  error", e);
			return new ArrayList<LoginRegistration>();
		}
	}
	
	public static void main(String[] args) {
		LoginConfigureDaoImpl daoImpl=new LoginConfigureDaoImpl();
		Map<String, List<String>> m = daoImpl.findLoginConfigure();
		System.out.println(m.entrySet().iterator().next().getKey());
	}
}
