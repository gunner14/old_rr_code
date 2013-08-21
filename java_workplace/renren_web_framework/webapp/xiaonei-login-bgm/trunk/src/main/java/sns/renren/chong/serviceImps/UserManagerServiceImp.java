package sns.renren.chong.serviceImps;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.UserBean;
import sns.renren.chong.dao.UsersDAO;
import sns.renren.chong.services.UserManagerService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class UserManagerServiceImp implements UserManagerService {
	@Autowired
	private UsersDAO userDAO;
	
	@Override
	public List<UserBean> getAllUsers(UserBean host) {
		List<UserBean> users = null;
		try {
			users = userDAO.getAllUsers();
		} catch (Exception e) {
			//e.printStackTrace();
		}
		if (users != null) {
			sortAllUsers(users, host);
		}
		return users;
	}
	
	@Override
	public List<UserBean> getMatchUsers(String pattern) {
		List<UserBean> users = null;
		try {
			users = userDAO.getMatchUsers("%" + pattern + "%");
		} catch (Exception e) {
			//e.printStackTrace();
		}
		if (users != null) {
			sortMatchUsers(users, pattern);
		}
		return users;
	}
	
	@Override
	public int deleteUser(String name) {
		int result = 0;
		try {
			result = userDAO.deleteRecord(name);
		} catch (Exception e) {
			//e.printStackTrace();
		}
		return result;
	}
	
	@SuppressWarnings("unchecked")
	private void sortAllUsers(List<UserBean> users, UserBean host) {
		ComparatorAllUser cmp = new ComparatorAllUser(host);
		Collections.sort(users, cmp);
	}
	
	@SuppressWarnings("unchecked")
	private void sortMatchUsers(List<UserBean> users, String pattern) {
		ComparatorMatchUser cmp = new ComparatorMatchUser(pattern);
		Collections.sort(users, cmp);
	}

	@SuppressWarnings("rawtypes")
	class ComparatorAllUser implements Comparator {
		
		private UserBean host = null;
		public ComparatorAllUser(UserBean host) {
			this.host = host;
		}
		
		/**
		 * @排序 host 第一；其次余下super；再其次normal;同一identity,按名字排列 
		 * */
		@Override
		public int compare(Object o1, Object o2) {
			UserBean user1 = (UserBean)o1;
			UserBean user2 = (UserBean)o2;
			
			if(user1.getName().equals(host.getName()))
				return -1;
			if(user2.getName().equals(host.getName()))
				return 1;
			if(user1.getIdentity().equals(user2.getIdentity())) {
				return user1.getName().compareTo(user2.getName());
			}
			if ("S".equals(user1.getIdentity()) || "s".equals(user1.getIdentity()))
				return -1;
			return 1;
		}
		
	} 

	@SuppressWarnings("rawtypes")
	class ComparatorMatchUser implements Comparator {
		private String pattern = null;
		public ComparatorMatchUser(String pattern) {
			this.pattern = pattern;
		}
		@Override
		public int compare(Object o1, Object o2) {
			UserBean user1 = (UserBean) o1;
			UserBean user2 = (UserBean) o2;
			int re1 = (user1.getName().indexOf(pattern) == -1 ? 0 : 1) +
					  (user1.getMail().indexOf(pattern) == -1 ? 0 : 1);
			int re2 = (user2.getName().indexOf(pattern) == -1 ? 0 : 1) +
					  (user2.getMail().indexOf(pattern) == -1 ? 0 : 1);
			
			if(re1 == re2) {
				return user1.getName().compareTo(user2.getName());
			}
			return re2 - re1;
		}
		
	}

}
