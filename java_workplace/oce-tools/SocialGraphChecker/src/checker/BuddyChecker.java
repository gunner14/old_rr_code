package checker;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import mop.hi.oce.domain.buddy.BuddyRelation;

import org.junit.Test;

public class BuddyChecker {
	@Test
	public void test() {
		System.out.println("test");

		BuddyChecker checker = new BuddyChecker();

//		int hisXiaoNeiId = 247049410;
//		int herXiaoNeiId = 268269080;
		
		int hisXiaoNeiId = 281264530;
		int herXiaoNeiId = 24496255;

		// checker.displayAll(hisXiaoNeiId);
		// checker.reloadAll(hisXiaoNeiId);

		// checker.displayBuddy(hisXiaoNeiId);
		// checker.reloadAndDisplayBuddy(hisXiaoNeiId);
		// checker.reloadBuddyCache(hisXiaoNeiId);
		checker.displayRelation(hisXiaoNeiId, herXiaoNeiId);

		// checker.displayApply(herXiaoNeiId);
		// checker.reloadAndDisplayApply(hisXiaoNeiId);
		// checker.reloadApply(hisXiaoNeiId);
		// checker.reloadApply(herXiaoNeiId);

		// checker.addApply(hisXiaoNeiId,herXiaoNeiId);
		// checker.acceptApply(hisXiaoNeiId,herXiaoNeiId);
		// checker.removeFriend(hisXiaoNeiId,herXiaoNeiId);
		// checker.addFriend(hisXiaoNeiId,herXiaoNeiId);

		// Map<Integer,int[]> haha = new HashMap<Integer,int[]>();
		// int[] a = {1,2,3};
		// haha.put(1, a);
		// Map<Integer,List<Integer> > hehe = new HashMap<Integer,List<Integer>
		// >();
		//
		// for( Entry<Integer, int[]> one : haha.entrySet()){
		// int key = one.getKey();
		// List<Integer> value = new ArrayList<Integer>();
		// for( int two : one.getValue() ){
		// value.add(two);
		// System.out.println(two);
		// }
		// hehe.put(key, value);
		// }
	}

	public void displayAll(int userId) {
		displayBuddy(userId);
		displayApply(userId);
	}

	public void reloadAll(int userId) {
		reloadBuddyCache(userId);
		reloadApply(userId);
	}

	public void displayBuddy(int userId) {
		List<BuddyRelation> BuddyCore_return = Adapter.getBuddyCoreFriendList(
				userId, 0, 1000);
		List<Integer> BuddyByIdCache_return = Adapter.getBuddyByIdCache(userId);
		List<Integer> BuddyByNameCache_return = Adapter
				.getBuddyByNameCache(userId);
		List<Integer> BuddyByOnlineTimeCache_return = Adapter
				.getBuddyByOnlineTimeCache(userId);
		List<Integer> BuddyByAddTimeCache_return = Adapter
				.getBuddyByAddTimeCache(userId);

		System.out.println("用户ID : " + userId);
		System.out.println("========好友========");
		System.out.println("	BuddyCore              : "
				+ BuddyCore_return.size());
		System.out.println("	BuddyByIdCache         : "
				+ BuddyByIdCache_return.size());
		System.out.println("	BuddyByNameCache       : "
				+ BuddyByNameCache_return.size());
		System.out.println("	BuddyByOnlineTimeCache : "
				+ BuddyByOnlineTimeCache_return.size());
		System.out.println("	BuddyByAddTimeCache    : "
				+ BuddyByAddTimeCache_return.size());
		System.out.println("");
	}

	public void reloadBuddyCache(int userId) {
		System.out.println("reloadBuddyCache : " + userId);
		Adapter.reloadBuddyCache(userId);
	}

	public void reloadAndDisplayBuddy(int userId) {
		reloadBuddyCache(userId);
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		displayBuddy(userId);
	}

	public void displayRelation(int userIdA, int userIdB) {
		BuddyRelation BuddyCore_BuddyRelation_B2A = Adapter.getRelation(
				userIdA, userIdB);
		String relation_B2A = new String("???");
		if (BuddyCore_BuddyRelation_B2A.isApply()) {
			relation_B2A = "isApply";
		} else if (BuddyCore_BuddyRelation_B2A.isApplied()) {
			relation_B2A = "isApplied";
		} else if (BuddyCore_BuddyRelation_B2A.isBlock()) {
			relation_B2A = "isBlock";
		} else if (BuddyCore_BuddyRelation_B2A.isBlocked()) {
			relation_B2A = "isBlocked";
		} else if (BuddyCore_BuddyRelation_B2A.isFriend()) {
			relation_B2A = "isFriend";
		} else {
			relation_B2A = " no relationship";
		}
		System.out.println("------------------" + relation_B2A);
		MyBuddyApplyCacheDataWithTotal BuddyApplyCache_IDS_return_B2A = Adapter
				.getBuddyApplyCacheWithTotal(userIdA, 0, -1);
		String BuddyApplyCache_exist_B2A = new String("no  : B in A.BuddyApply");
		for (MyBuddyApplyCacheData one : BuddyApplyCache_IDS_return_B2A
				.getBuddyApplyCache()) {
			if (userIdB == one.getApplicant()) {
				BuddyApplyCache_exist_B2A = "yes : B in A.BuddyApply";
				break;
			}
		}

		List<Integer> BuddyByIdCache_return_B2A = Adapter
				.getBuddyByIdCache(userIdA);
		List<Integer> BuddyByNameCache_return_B2A = Adapter
				.getBuddyByNameCache(userIdA);
		List<Integer> BuddyByAddTimeCache_return_B2A = Adapter
				.getBuddyByAddTimeCache(userIdA);
		List<Integer> BuddyByOnlineTimeCache_return_B2A = Adapter
				.getBuddyByOnlineTimeCache(userIdA);

		String BuddyByIdCache_exist_B2A = new String(
				"no  : B in A.BuddyByIdCache");
		for (int one : BuddyByIdCache_return_B2A) {
			if (userIdB == one) {
				BuddyByIdCache_exist_B2A = "yes : B in A.BuddyByIdCache";
				break;
			}
		}
		String BuddyByNameCache_exist_B2A = new String(
				"no  : B in A.BuddyByNameCache");
		for (int one : BuddyByNameCache_return_B2A) {
			if (userIdB == one) {
				BuddyByNameCache_exist_B2A = "yes : B in A.BuddyByNameCache";
				break;
			}
		}
		String BuddyByOnlineTimeCache_exist_B2A = new String(
				"no  : B in A.BuddyByOnlineTimeCache");
		for (int one : BuddyByOnlineTimeCache_return_B2A) {
			if (userIdB == one) {
				BuddyByOnlineTimeCache_exist_B2A = "yes : B in A.BuddyByOnlineTimeCache";
				break;
			}
		}
		String BuddyByAddTimeCache_exist_B2A = new String(
				"no  : B in A.BuddyByAddTimeCache");
		for (int one : BuddyByAddTimeCache_return_B2A) {
			if (userIdB == one) {
				BuddyByAddTimeCache_exist_B2A = "yes : B in A.BuddyByAddTimeCache";
				break;
			}
		}

		BuddyRelation BuddyCore_BuddyRelation_A2B = Adapter.getRelation(
				userIdB, userIdA);
		String relation_A2B = new String("???");
		if (BuddyCore_BuddyRelation_A2B.isApply()) {
			relation_A2B = "isApply";
		} else if (BuddyCore_BuddyRelation_A2B.isApplied()) {
			relation_A2B = "isApplied";
		} else if (BuddyCore_BuddyRelation_A2B.isBlock()) {
			relation_A2B = "isBlock";
		} else if (BuddyCore_BuddyRelation_A2B.isBlocked()) {
			relation_A2B = "isBlocked";
		} else if (BuddyCore_BuddyRelation_A2B.isFriend()) {
			relation_A2B = "isFriend";
		} else {
			relation_A2B = "no relationship";
		}
		MyBuddyApplyCacheDataWithTotal BuddyApplyCache_IDS_return_A2B = Adapter
				.getBuddyApplyCacheWithTotal(userIdB, 0, -1);
		String BuddyApplyCache_exist_A2B = new String("no  : A in B.BuddyApply");
		for (MyBuddyApplyCacheData one : BuddyApplyCache_IDS_return_A2B
				.getBuddyApplyCache()) {
			if (userIdA == one.getApplicant()) {
				BuddyApplyCache_exist_A2B = "yes : A in B.BuddyApply";
				break;
			}
		}

		List<Integer> BuddyByIdCache_return_A2B = Adapter
				.getBuddyByIdCache(userIdB);
		List<Integer> BuddyByNameCache_return_A2B = Adapter
				.getBuddyByNameCache(userIdB);
		List<Integer> BuddyByAddTimeCache_return_A2B = Adapter
				.getBuddyByAddTimeCache(userIdB);
		List<Integer> BuddyByOnlineTimeCache_return_A2B = Adapter
				.getBuddyByOnlineTimeCache(userIdB);

		String BuddyByIdCache_exist_A2B = new String(
				"no  : A in B.BuddyByIdCache");
		for (int one : BuddyByIdCache_return_A2B) {
			if (userIdA == one) {
				BuddyByIdCache_exist_A2B = "yes : A in B.BuddyByIdCache";
				break;
			}
		}
		String BuddyByNameCache_exist_A2B = new String(
				"no  : A in B.BuddyByNameCache");
		for (int one : BuddyByNameCache_return_A2B) {
			if (userIdA == one) {
				BuddyByNameCache_exist_A2B = "yes : A in B.BuddyByNameCache";
				break;
			}
		}
		String BuddyByOnlineTimeCache_exist_A2B = new String(
				"no  : A in B.BuddyByOnlineTimeCache");
		for (int one : BuddyByOnlineTimeCache_return_A2B) {
			if (userIdA == one) {
				BuddyByOnlineTimeCache_exist_A2B = "yes : A in B.BuddyByOnlineTimeCache";
				break;
			}
		}
		String BuddyByAddTimeCache_exist_A2B = new String(
				"no  : A in B.BuddyByAddTimeCache");
		for (int one : BuddyByAddTimeCache_return_A2B) {
			if (userIdA == one) {
				BuddyByAddTimeCache_exist_A2B = "yes : A in B.BuddyByAddTimeCache";
				break;
			}
		}

		System.out.println("");
		System.out.println("A用户ID : " + userIdA);
		System.out.println("B用户ID : " + userIdB);
		System.out.println("========好友关系========");
		System.out.println("---> B to A :" + relation_B2A);
		System.out.println("	" + BuddyApplyCache_exist_B2A);
		System.out.println("	" + BuddyByIdCache_exist_B2A);
		System.out.println("	" + BuddyByNameCache_exist_B2A);
		System.out.println("	" + BuddyByOnlineTimeCache_exist_B2A);
		System.out.println("	" + BuddyByAddTimeCache_exist_B2A);
		System.out.println("---> A to B :" + relation_A2B);
		System.out.println("	" + BuddyApplyCache_exist_A2B);
		System.out.println("	" + BuddyByIdCache_exist_A2B);
		System.out.println("	" + BuddyByNameCache_exist_A2B);
		System.out.println("	" + BuddyByOnlineTimeCache_exist_A2B);
		System.out.println("	" + BuddyByAddTimeCache_exist_A2B);
		System.out.println("");
	}

	public void displayApply(int userId) {
		int BuddyCore_AppliedCount = Adapter.getBuddyCoreAppliedCount(userId);
		int UserCount_RequestCount = Adapter.getRequestCount(userId);
		MyBuddyApplyCacheDataWithTotal BuddyApplyCache_IDS_return = Adapter
				.getBuddyApplyCacheWithTotal(userId, 0, -1);

		int BuddyApplyCache_TotalCount = BuddyApplyCache_IDS_return
				.getTotalCount();

		String BuddyApplyCache_IDS = new String();
		for (MyBuddyApplyCacheData one : BuddyApplyCache_IDS_return
				.getBuddyApplyCache()) {
			BuddyApplyCache_IDS = BuddyApplyCache_IDS + one.getApplicant()
					+ ", ";
		}

		System.out.println("");
		System.out.println("用户ID : " + userId);
		System.out.println("========好友申请========");
		System.out.println("	BuddyCore.AppliedCount(校内通) : "
				+ BuddyCore_AppliedCount);
		System.out
				.println("	UserCount(首页数/申请页右侧数) : " + UserCount_RequestCount);
		System.out.println("	BuddyApplyCache.TotalCount(申请页左侧数) : "
				+ BuddyApplyCache_TotalCount);
		System.out.println("");
	}

	public void reloadApply(int userId) {
		System.out.println("reloadApply " + userId);
		Adapter.reloadBuddyApplyCache(userId);
	}

	public void reloadAndDisplayApply(int userId) {
		reloadApply(userId);
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		displayApply(userId);
	}

	private void addFriend(int userId, int otherId) {
		Adapter.addFriend(userId, otherId);
	}

	private void removeFriend(int userId, int otherId) {
		Adapter.removeFriend(userId, otherId);
	}

	private void addApply(int userId, int otherId) {
		Adapter.addApply(userId, otherId);
	}

	private void acceptApply(int userId, int otherId) {
		Adapter.acceptApply(userId, otherId);
	}

	private void workFromFile(String inFileName,String outFileName) {
		System.out.println("==> now reading ... ");
		File infile;
		File outfile;
		try {
			infile = new File(inFileName);
			outfile = new File(outFileName);
		} catch (Exception ex) {
			ex.printStackTrace();
			infile = null;
			outfile = null;
		}

		BufferedReader reader = null;
		BufferedWriter writer = null;
		try {
			reader = new BufferedReader(new FileReader(infile));
			writer = new BufferedWriter(new FileWriter(outfile));
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
			return;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		String line = null;
		try {
			line = reader.readLine();
		} catch (IOException e1) {
			e1.printStackTrace();
			return;
		}

		while (line != null) {
			try {
				String[] fields = line.split("\t");
				line = reader.readLine();
				int hostId = Integer.valueOf(fields[0]);
				int guestId = Integer.valueOf(fields[1]);
				String ret = varifyCommonFriend(hostId, guestId);
				System.out.println(ret);
				writer.write(ret);
				writer.newLine();
				writer.flush();
			} catch (NumberFormatException e) {
				e.printStackTrace();
				return;
				// continue;
			} catch (Exception e) {
				e.printStackTrace();
				return;
				// continue;
			}
		}

		try {
			reader.close();
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private String varifyCommonFriend(int hostId, int guestId) {
		
		String res = null;
		
		List<Integer> level1 = Adapter.getBuddyByIdCache(hostId);
		
		Map<Integer, List<Integer>> level2 = Adapter.getBuddyByIdCaches(level1);
		
		Set<Integer> level3 = calCommonFriendCount(level2);
		
		for( Integer one : level3 ){
			if( one == guestId ){
				res = hostId+" "+guestId+" 1 "+level1.size()+" "+level3.size();
				return res;
			}
		}
		res = hostId+" "+guestId+" 0 "+level1.size()+" "+level3.size();
		return res;
		
	}

	private Set<Integer> calCommonFriendCount(Map<Integer, List<Integer>> level2) {
		Set<Integer> res = new HashSet<Integer>();
		for( Map.Entry<Integer, List<Integer> > one : level2.entrySet() ){
			for( int two : one.getValue() ){
				res.add(two);
			}
		}
		return res;
	}

	public static void main(String args[]) {
		// Properties prop = System.getProperties();
		// prop.put("socksProxyHost", "127.0.0.1");
		// prop.put("socksProxyPort", "3001");

		// if (args.length < 1) {
		// System.exit(0);
		// }
		//
		// int userId;
		// try {
		// userId = Integer.valueOf(args[0]);
		// } catch (Exception e) {
		// e.printStackTrace();
		// return;
		// }
//		System.out.println("test");
//
//		String inFileName = "/home/yexiang841/ids.txt";
//		String outFileName = "/home/yexiang841/res.txt";
//
//		BuddyChecker checker = new BuddyChecker();
//		checker.workFromFile(inFileName,outFileName);
//
//		System.exit(0);
	}
}