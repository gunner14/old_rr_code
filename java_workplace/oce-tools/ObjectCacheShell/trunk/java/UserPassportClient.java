import java.util.Scanner;


public class UserPassportClient {
	private static class MyUserPassportInfo extends UserPassportInfo{
		
	}
	private static class MyUserPassportFactory implements UserPassportFactory<MyUserPassportInfo>{

		@Override
		public MyUserPassportInfo create() {
			return new MyUserPassportInfo();
		}
		
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try{
			UserPassportAdapter<MyUserPassportInfo> upa=new UserPassportAdapter<MyUserPassportInfo>(new MyUserPassportFactory());
			Scanner sc = new Scanner(System.in);
			while(sc.hasNextInt()){
				int id=sc.nextInt();
				MyUserPassportInfo r=upa.getUserPassport(id);
				if(r.getDomain().equals("kaixin.com")){
					System.out.println(id);
				}
			}
			
			
		}catch(Exception e){
			e.printStackTrace();
		}
		System.exit(0);
	}

}

