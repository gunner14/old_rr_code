import com.xiaonei.xce.userbasic.UserBasicAdapter;
import com.xiaonei.xce.userbasic.UserBasicFactory;
import com.xiaonei.xce.userbasic.UserBasicInfo;


public class UserBasicClient {
	private static class MyUserBasicInfo extends UserBasicInfo{

		/**
		 * 
		 */
		private static final long serialVersionUID = 1L;
		
	}
	private static class MyUserBasicFactory implements UserBasicFactory<MyUserBasicInfo>{

		@Override
		public MyUserBasicInfo create() {
			// TODO Auto-generated method stub
			return new MyUserBasicInfo();
		}
		
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try{
			UserBasicAdapter<MyUserBasicInfo> uba=new UserBasicAdapter<MyUserBasicInfo>(new MyUserBasicFactory());
			for(int i=0;i<100;i++){
				try{
					MyUserBasicInfo mubi=uba.getUserBasic(306716900+i);
					System.out.println(mubi.getName());
				}catch(Exception e){
					e.printStackTrace();
				}
			}
		}catch(Exception e){
			e.printStackTrace();
		}
		System.exit(0);
	}

}

