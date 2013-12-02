package com.xiaonei.xce.test.friendsrecentphotos;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import xce.clusterstate.ClientInterface;
import xce.friendsphotos.FriendsRecentPhotosManagerPrx;
import xce.friendsphotos.FriendsRecentPhotosManagerPrxHelper;
import xce.friendsphotos.PhotoResult;
import xce.friendsphotos.UserPhotos;
import xce.serverstate.ServerStateInfo;

import com.xiaonei.xce.test.menucache.MenuCacheTest;

/***
 * 这个类主要是测试服务重启后数据有没有问题，而不是测试服务详细逻辑的正确性
 * @author shangmin
 *
 */
public class FriendsRecentPhotosTest {
  private static final String SHOW_DETAIL_INFO = "showDetailInfo";
  private static final String USER_IDS_FILE = "userIdsFile";
  private static final String TEST_MOD = "testMod";

  private static final String endpoints = "ControllerFriendsRecentPhotosManager";
  private static final int SERVER_CLUSTER = 10;
  private static int interval = 120;

  private static ClientInterface _csClient = null;

  static {
    try{
      _csClient  = new ClientInterface(endpoints,
          interval);
    }catch(Throwable e){
      e.printStackTrace();
    }
  }
  
  // 获取所有适应userid的服务的prx
  private ServerStateInfo[] getAllFriendsRecentPhotosManagerPrx(int userid) {
    int cluster = _csClient.getCluster();
    if (cluster <= 0) {
      ServerStateInfo[] states = new ServerStateInfo[0];
      return states;
    } else {
      int mod = userid % cluster;
      return _csClient.getAvailableObjProxies().get(mod);
    }
  }
  
  private List<FriendsRecentPhotosManagerPrx> getAllPrxs(int userId) {
    ServerStateInfo[] stateInfos = getAllFriendsRecentPhotosManagerPrx(userId % _csClient.getCluster());
    List<FriendsRecentPhotosManagerPrx> prxs = new ArrayList<FriendsRecentPhotosManagerPrx>();
    for (ServerStateInfo stateInfo : stateInfos) {
      try {
        FriendsRecentPhotosManagerPrx prx = FriendsRecentPhotosManagerPrxHelper.uncheckedCast(
            stateInfo.prx.ice_timeout(300));
        prxs.add(prx);
      } catch (Exception ex) {
        ex.printStackTrace();
      }
    }
    return prxs;
  }
  
  private int getTotalPhotos(final PhotoResult result) {
    int total = 0;
    for (UserPhotos userPhotos : result.userPhotos) {
      total += userPhotos.photos.length;
    }
    return total;
  }
  
  private boolean testOk(int userId, boolean showDetailInfo) {
    List<FriendsRecentPhotosManagerPrx> prxs = getAllPrxs(userId);
    int begin = 0, limit = 100;
    for (FriendsRecentPhotosManagerPrx prx : prxs) {
      PhotoResult result = prx.getFriendsRecentPhotos(userId, begin, limit);
      int total = getTotalPhotos(result);
      if (total != limit) {
        if (showDetailInfo) {
          System.out.println("prx = " + prx + ", userId = " + userId + ", begin = " + begin
              + ", limit = " + limit + ", result.size = " + total);
        }
        return false;
      }
    }
    return true;
  }
  
  public void testFriendsRecentPhotos(final List<Integer> userIds, final Set<Integer> testMods, boolean showDetailInfo) {
    int totalTestNum = 0, errorNum = 0;
    for (Integer userId : userIds) {
      if (testMods.isEmpty() || _csClient.getCluster() == 1 || testMods.contains(userId)) {
        totalTestNum++;
        if (!testOk(userId, showDetailInfo)) {
          errorNum++;
        }
      }
    }
    System.out.println("总共测试了 " + totalTestNum + " 个用户，其中 " + errorNum + " 个错误！");
  }
  
  public void printUsage() {
    System.out.println();
    System.out.println("-------------------------------------------------------------------------");
    System.out.println("Usage: FriendsRecentPhotosTest userIdsFile= showDetailInfo=[true|false] [testMod= ... testMod=]");
    System.out.println("userIdsFile是指定的测试用户id的文件目录，一行一个id。testMod可以有多个，如果没有的话，默认测试所有id");
    System.out.println("-------------------------------------------------------------------------");
  }

  public static void main(String[] args) {
    FriendsRecentPhotosTest friendsRecentPhotosTester = new FriendsRecentPhotosTest();
    String userIdsFile = null;
    Set<Integer> testMods = new HashSet<Integer>();
    boolean showDetailInfo = false;
    for (String arg : args) {
      String[] argsArr = arg.split("=");
      if (argsArr.length == 2) {
        if (argsArr[0].compareTo(TEST_MOD) == 0) {
          try {
            int testMod = Integer.valueOf(argsArr[1]);
            testMods.add(new Integer(testMod));
          } catch (Exception ex) {
          }
        } else if (argsArr[0].compareTo(SHOW_DETAIL_INFO) == 0) {
          try {
            showDetailInfo = Boolean.valueOf(argsArr[1]);
          } catch (Exception ex) {
          }
        } else if (argsArr[0].compareTo(USER_IDS_FILE) == 0) {
          userIdsFile = argsArr[1];
        }
      }
    }

    if (userIdsFile == null) {
      friendsRecentPhotosTester.printUsage();
      System.exit(0);
    }
    List<Integer> userIds = MenuCacheTest.getUserIdsFromFile(userIdsFile);
    friendsRecentPhotosTester.testFriendsRecentPhotos(userIds, testMods, showDetailInfo);
    System.exit(0);

  }
}
