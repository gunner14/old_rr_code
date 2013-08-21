package com.xiaonei.xce.PreSearchMatch;


public class PreSearchMatchAdapterTest {

	public static void main(String[] args)  {
		// TODO Auto-generated method stub
//		System.setProperty("isTestSearchChannel", "true");
		try{
			String[] matchStr = null; 
			
			matchStr = PreSearchMatchAdapter.getInstance().searchPreMatch(202007480, "阿");
			if(matchStr != null) {
				System.out.println(matchStr.length);
				for(String u : matchStr) {
					System.out.println("len="+u);
				}
			} else {
				System.out.println("result is null");
			}
			
		} catch(Exception e) {
			System.out.println("catch:"+e);
		}		
		
		
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}

}
