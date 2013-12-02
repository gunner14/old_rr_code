package xce.tripod.checker;

import java.util.HashMap;
import java.util.Map;

public class TestChecker extends AbstractChecker {
	public Map<Integer, String> mapCache = new HashMap<Integer, String>();
	public Map<Integer, String> mapDB = new HashMap<Integer, String>();

	/*
	 * (non-Javadoc)
	 * 
	 * @see mop.hi.oce.adapter.impl.Checker#compare(java.lang.Object,
	 * java.lang.Object)
	 */
	@Override
	protected boolean compare(Object a, Object b) {
		// TODO Auto-generated method stub
		String strA = (String) a;
		String strB = (String) b;
		System.out.println("compare " + a + " with " + b);
		return strA.equals(strB);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see mop.hi.oce.adapter.impl.Checker#reloadData(int)
	 */
	@Override
	protected boolean reloadData(int id) {
		// TODO Auto-generated method stub
		System.out.println("reload data id");
		return true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see mop.hi.oce.adapter.impl.Checker#getDataFromDB(int)
	 */
	@Override
	protected Object getDataFromDB(int id) {
		// TODO Auto-generated method stub
		return mapDB.get(id%3+1);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see mop.hi.oce.adapter.impl.Checker#getDataFromCache(int)
	 */
	@Override
	protected Object getDataFromCache(int id) {
		// TODO Auto-generated method stub
		return mapCache.get(id%3+1);
	}
	
	public static void main(String[] args) {
		TestChecker checker =new TestChecker();
		checker.mapCache.put(1, "a");
		checker.mapDB.put(1, "a");
		checker.mapCache.put(2, "b");
		checker.mapDB.put(2, "b");
		checker.mapCache.put(3, "c");
		checker.mapDB.put(3, "d");
		checker.startCheck("/home/meteor/tmp/idfile.dat",1000,3);
	}


}

