package test;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.userbaseview.UserBaseCommonView;
import com.xiaonei.xce.userbaseview.UserBaseCommonViewAdapter;
import com.xiaonei.xce.userbaseview.UserBaseCommonViewFactory;
import com.xiaonei.xce.username.UserNameFactory;
import com.xiaonei.xce.username.UserNameInfo;
import com.xiaonei.xce.userpassport.UserPassportFactory;
import com.xiaonei.xce.userpassport.UserPassportInfo;
import com.xiaonei.xce.userright.UserRightFactory;
import com.xiaonei.xce.userright.UserRightInfo;
import com.xiaonei.xce.userstate.UserStateFactory;
import com.xiaonei.xce.userstate.UserStateInfo;
import com.xiaonei.xce.userurl.UserUrlFactory;
import com.xiaonei.xce.userurl.UserUrlInfo;

public class TestClient {
	private static UserBaseCommonViewAdapter<MyUserCommonView, MyUserPassportInfo, MyUserRightInfo, MyUserNameInfo, MyUserStateInfo, MyUserUrlInfo> ubcva = new UserBaseCommonViewAdapter<MyUserCommonView, MyUserPassportInfo, MyUserRightInfo, MyUserNameInfo, MyUserStateInfo, MyUserUrlInfo>(
			new MyUserCommonViewFactory(), new MyUserPassportFactory(),
			new MyUserRightFactory(), new MyUserNameFactory(),
			new MyUserStateFactory(), new MyUserUrlFactory());

	private static class MyUserPassportInfo extends UserPassportInfo {

	}

	private static class MyUserRightInfo extends UserRightInfo {

	}

	private static class MyUserNameInfo extends UserNameInfo {

	}

	private static class MyUserStateInfo extends UserStateInfo {

	}

	private static class MyUserUrlInfo extends UserUrlInfo {

	}

	private static class MyUserCommonView
			extends
			UserBaseCommonView<MyUserPassportInfo, MyUserRightInfo, MyUserNameInfo, MyUserStateInfo, MyUserUrlInfo> {

	}

	private static class MyUserCommonViewFactory implements
			UserBaseCommonViewFactory<MyUserCommonView> {

		public MyUserCommonView create() {
			// TODO Auto-generated method stub
			return new MyUserCommonView();
		}

	}

	private static class MyUserPassportFactory implements
			UserPassportFactory<MyUserPassportInfo> {

		public MyUserPassportInfo create() {
			// TODO Auto-generated method stub
			return new MyUserPassportInfo();
		}

	}

	private static class MyUserRightFactory implements
			UserRightFactory<MyUserRightInfo> {

		public MyUserRightInfo create() {
			// TODO Auto-generated method stub
			return new MyUserRightInfo();
		}

	}

	private static class MyUserNameFactory implements
			UserNameFactory<MyUserNameInfo> {

		public MyUserNameInfo create() {
			// TODO Auto-generated method stub
			return new MyUserNameInfo();
		}

	}

	private static class MyUserStateFactory implements
			UserStateFactory<MyUserStateInfo> {
		public MyUserStateInfo create() {
			return new MyUserStateInfo();
		}
	}

	private static class MyUserUrlFactory implements
			UserUrlFactory<MyUserUrlInfo> {
		public MyUserUrlInfo create() {
			return new MyUserUrlInfo();
		}
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		int last = 0;
		while (true) {
			String sql = "select id from user_passport where id>=" + last
					+ " limit 3000";
			try {
				Connection c = XceAdapter.getInstance().getReadConnection(
						"user_passport");
				try {
					Statement s = c.createStatement();
					try {
						ResultSet rs = s.executeQuery(sql);
						System.out.println(sql);
						try {
							int max = last;
							while (rs.next()) {
								try{
									int id = rs.getInt("id");
									if (max < id) {
										max = id;
									}
									check(id);
								}catch(Exception e){
									e.printStackTrace();
								}
							}
							if (max <= last) {
								break;
							} else {
								last = max + 1;
							}
						} catch (Exception e) {
							e.printStackTrace();
						} finally {
							rs.close();
						}
					} catch (Exception e) {
						e.printStackTrace();
					} finally {
						s.close();
					}
				} catch (Exception e) {
					e.printStackTrace();
				} finally {
					c.close();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		System.exit(0);
	}

	private static void check(int id) {
		try {
			MyUserCommonView view = ubcva.getUserBaseView(id);
			if (view.getUserNameInfo().getId() == 0) {
				System.out.println(id+" get 0");
			}
		} catch (Exception e) {
			System.out.println(e.toString() + " id: " + id);
		}
	}
}

