package xce.tools.objectcache;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class ObjectCacheShell {
	public static void main(String[] args) {
		if (args.length < 1) {
			printUsage();
			System.exit(0);
		}
		System.out.println("arglength:"+args.length);
		try {
			BlockingQueue<Long[]> queue = new LinkedBlockingQueue<Long[]>();
			Consumer consumer = null;
			if ("ReloadFriendDist".equalsIgnoreCase(args[0])) {
				Producer producer = new UserIdAllAction(queue, 10000);
				consumer = new ReloadOceAction(producer, "FriendDist",
						new int[] { 0 }, 10);
			} else if ("RebuildAllFriendDist".equalsIgnoreCase(args[0])) {
				Producer producer = new UserIdAllAction(queue, 10000);
				consumer = new InvokeFriendDistAction(producer);
			} else if ("RemoveFeedNewsSwap".equalsIgnoreCase(args[0])) {
				Producer producer = new UserIdAllAction(queue, 10000);
				consumer = new RemoveFeedAction(producer, "XFeedNews",
						new int[] { 1 }, 40);
			}else if ("RemoveFeedNewsRecent".equalsIgnoreCase(args[0])) {
						Producer producer = new UserIdTopNAction(queue, 1000000);
						consumer = new RemoveFeedAction(producer, "XFeedNews",
								new int[] { 0 }, 40);
			} else if ("PreloadUserCount".equalsIgnoreCase(args[0])){
				Producer producer = new UserIdAllAction(queue,10000);
				consumer = new PreloadOceAction(producer,"UserCount",
						new int[]{0},10,1,40);
			} else if ("PreloadUserCountR".equalsIgnoreCase(args[0])){
				Producer producer = new UserIdAllAction(queue,10000);
				consumer = new PreloadOceAction(producer,"UserCountR",
						new int[]{0},10,1,40);
			} else if ("PreloadUserConfigR".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer = new PreloadOceAction(producer,"UserConfigR",
						new int[]{0},100,-4,1);
			} else if ("PreloadUserConfig".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer = new PreloadOceAction(producer,"UserConfigN",
						new int[]{0},100,-4,50);
			} else if ("PreloadUserState".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"UserState",new int[]{0},10,-4,30);
			} else if ("PreloadUserStateR".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"UserStateR",new int[]{0},10,-4,30);
			} else if ("PreloadUserNetwork".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdHotAction(queue,10000,1000000);
				consumer=new PreloadOceAction(producer,"UserNetworkN",new int[]{0,1,2,3,4,5,6,7},10,-4,500);
			} else if ("PreloadBlogFootprintL".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,1000);
				consumer=new PreloadOceAction(producer,"BlogFootprintL",new int[]{0},1,-4,1);
			} else if ("PreloadFootprintN".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"FootprintN",new int[]{0,3},10,-4,10);
			} else if ("PreloadVisitCount".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"FootprintN",new int[]{3},10,-4,100);
			} else if ("PreloadRecentContact".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,1000);
				consumer=new PreloadOceAction(producer,"RecentContact",new int[]{0},10,-4,5);
			} else if ("PreloadBlogFootprint".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"BlogFootprint",new int[]{0},1,-4,100);
			} else if ("PreloadBuddyCore".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"BuddyCore",new int[]{0},10,-4,400);
			} else if ("PreloadNotifyRpl".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,1000);
				consumer=new PreloadOceAction(producer,"NotifyRpl",new int[]{0},10,1,30);
			} else if ("PreloadFriendDist".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"FriendDist",new int[]{0},10,-4,40);
			} else if ("PreloadFriendFinderReplica".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdHotAction(queue,10000,10000000);
				consumer=new PreloadSGAction(producer,"FriendFinderReplica",new int[]{0},10,-4,30);
			} else if ("PreloadOfferFriendsReplica".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdHotAction(queue,10000,10000000);
				consumer=new PreloadSGAction(producer,"OfferFriendsReplica",new int[]{0},10,-4,10);
			} else if ("PreloadFriendFinderByActiveUser".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdHotAction(queue,10000,10000000);
				consumer=new PreloadSGAction(producer,"FriendFinderByActiveUser",new int[]{2},10,-4,10);
			} else if ("PreloadUserBase".equalsIgnoreCase(args[0])){
				
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"UserBase",new int[]{4},100,-5,20);
			} else if ("PreloadUser".equalsIgnoreCase(args[0])){
				
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"UserN",new int[]{0,1},100,-4,40);
			} else if ("PreloadBuddyGroup".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"BuddyGroup",new int[]{0},10,-4,10);
			} else if ("PreloadBuddyApplyCache".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"BuddyApplyCache",new int[]{0},10,-4,100);
			} else if ("PreloadXFeedDispatcher".equalsIgnoreCase(args[0])){
				Producer producer=new XFeedIdAllAction(queue,10000);
				consumer=new PreloadXFeed(producer,10);
			} else if ("PreloadRecentlyOnlineBuddy".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"RecentlyOnlineBuddy",new int[]{0},10,4,100);
			} else if ("PreloadBuddyCacheByName".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"BuddyCacheByName",new int[]{0},10,-4,100);
			} else if ("PreloadXFeedMini".equalsIgnoreCase(args[0])){
				Producer producer=new UserIdAllAction(queue,10000);
				consumer=new PreloadOceAction(producer,"XFeedMini",new int[]{0},100,-4,40);
			}
			else {
				System.out.println("Unknown type " + args[0]);
			}
			consumer.waitFinished();
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.exit(0);
	}

	public static void printUsage() {
		System.out.println("Usage: Reload <SvcName>");

	}
}
