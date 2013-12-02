package com.xiaonei.xce.test.menucache;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import xce.clusterstate.ClientInterface;
import xce.menu.CacheType;
import xce.menu.MenuCacheManagerPrx;
import xce.menu.MenuCacheManagerPrxHelper;
import xce.serverstate.ServerStateInfo;

public class MenuCacheTest {
  private static final String endpoints = "ControllerMenuCacheN";
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
  private ServerStateInfo[] getMenuCacheManagerAllOneWay(int userid) {
    int cluster = _csClient.getCluster();
    if (cluster <= 0) {
      ServerStateInfo[] states = new ServerStateInfo[0];
      return states;
    } else {
      int mod = userid % cluster;
      return _csClient.getAvailableObjProxies().get(mod);
    }
  }
  
  private static final String SHOW_DETAIL_INFO = "showDetailInfo";
  private static final String USER_IDS_FILE = "userIdsFile";
  private static final String TEST_MOD = "testMod";
  
  public static List<Integer> getUserIdsFromFile(String filePath) {
    List<Integer> userIds = new ArrayList<Integer>();
    File userIdsFile = new File(filePath);
    BufferedReader reader = null;
    try {
      reader = new BufferedReader(new FileReader(userIdsFile));
      String line = null;
      while ((line = reader.readLine()) != null) {
        try {
          int userId = Integer.valueOf(line);
          userIds.add(new Integer(userId));
        } catch (Exception ex) {
          ex.printStackTrace();
        }
      }
      reader.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }
    return userIds;
  }
  
  public void printUsage() {
    System.out.println();
    System.out.println("-------------------------------------------------------------------------");
    System.out.println("Usage: MenuCacheTest userIdsFile= showDetailInfo=[true|false] [testMod= ... testMod=]");
    System.out.println("userIdsFile是指定的测试用户id的文件目录，一行一个id。testMod可以有多个，如果没有的话，默认测试所有id");
    System.out.println("-------------------------------------------------------------------------");
  }
  
  private boolean isEqual(int userId, final Map<CacheType, int[]> userMenus1, final Map<CacheType, int[]> userMenus2,
      boolean showDetailInfo) {
    if (userMenus1 == null || userMenus2 == null) {
      if (showDetailInfo) {
        System.out.println("userId = " + userId + ", userMenus1, userMenus2中有一个为null");
      }
      return false;
    }
    CacheType[] cacheTypes = {CacheType.RECENTLY, CacheType.FAVORITE};
    for (CacheType cacheType : cacheTypes) {
      int[] menus1 = userMenus1.get(cacheType);
      int[] menus2 = userMenus2.get(cacheType);
      if (menus1 == null && menus2 != null || menus1 != null && menus2 == null) {
        return false;
      }
      if (menus1 != null && menus2 != null) {
        if (menus1.length != menus2.length) {
          if (showDetailInfo) {
            System.out.println("userId = " + userId + ", cacheType = " + cacheType + " 对应的menus长度不一致");
            showUserMenusMap(userMenus1);
            showUserMenusMap(userMenus2);
          }
          return false;
        }
        for (int i = 0; i < menus1.length; ++i) {
          if (menus1[i] != menus2[i]) {
            if (showDetailInfo) {
              System.out.println("userId = " + userId + ", cacheType = " + cacheType + " 对应的menus内容不一致");
              showUserMenusMap(userMenus1);
              showUserMenusMap(userMenus2);
            }
            return false;
          }
        }
      }
    }
    return true;
  }
  
  private void showUserMenusMap(final Map<CacheType, int[]> userMenusMap) {
    for (Map.Entry<CacheType, int[]> entry : userMenusMap.entrySet()) {
      System.out.print(entry.getKey() + " :");
      for (int menu : entry.getValue()) {
        System.out.print(" " + menu);
      }
      System.out.println();
    }
  }
  
  private boolean testOK(int userId, boolean showDetailInfo) {
    ServerStateInfo[] states = getMenuCacheManagerAllOneWay(userId);
    Map<CacheType, int[]> initUserMenus = null;
    for (int i = 0; i < states.length; ++i) {
      if (states[i].prx != null) {      
        try {           
          MenuCacheManagerPrx prx = MenuCacheManagerPrxHelper.uncheckedCast(
              states[i].prx.ice_timeout(300));
          Map<CacheType, int[]> userMenus = prx.getAll(userId);
          if (initUserMenus == null) {
            initUserMenus = userMenus;
          } else {
            //比较userMenus和initUserMenus是否相等
            if (!isEqual(userId, initUserMenus, userMenus, showDetailInfo)) {
              return false;
            }
          }
        } catch (Exception ex) {
          if (showDetailInfo) {
            System.out.println("处理user = " + userId + "时发生了异常！");
          }
          System.out.println(ex);
          return false;
        }
      }
    }

    return true;
  }
  
  public void testMenuCache(final List<Integer> userIds, final Set<Integer> testMods, boolean showDetailInfo) {
    int testUserIdsTotal = 0, errorNum = 0;
    for (Integer userId : userIds) {
      if (testMods.isEmpty() || testMods.contains(userId % SERVER_CLUSTER)) {
        testUserIdsTotal++;
        if (!testOK(userId, showDetailInfo)) {
          errorNum++;
        }
      }
    }
    System.out.println("总共测试了 " + testUserIdsTotal + " 个用户，其中 " + errorNum + " 个错误，请查看");
  }
  
  public static void main(String args[]) {
    MenuCacheTest menuCacheTester = new MenuCacheTest();
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
      menuCacheTester.printUsage();
      System.exit(0);
    }
    List<Integer> userIds = getUserIdsFromFile(userIdsFile);
    menuCacheTester.testMenuCache(userIds, testMods, showDetailInfo);
    System.exit(0);
  }
}
