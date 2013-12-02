package com.xiaonei.xce.test.footprint;

import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.xiaonei.xce.footprintunite.FootprintUniteAdapter;
import com.xiaonei.xce.footprintunite.HomeFootprintResult;
import com.xiaonei.xce.test.menucache.MenuCacheTest;

public class HomeFootprintTest {
  private FootprintUniteAdapter adapter_ = new FootprintUniteAdapter();
  private static final String SHOW_DETAIL_INFO = "showDetailInfo";
  private static final String USER_IDS_FILE = "userIdsFile";
  private static final String TEST_MOD = "testMod";
  private static final int SERVER_CLUSTER = 10;
  
  private int[][] testData_ = { { 0, 9, 9 }, { 0, 48, 48 }, { 9, 39, 39 } };

  public FootprintUniteAdapter getInstance() {
    if (null == adapter_) {
      adapter_ = new FootprintUniteAdapter();
    }
    return adapter_;
  }

  public void testGetFootprint(final List<Integer> userIds, final Set<Integer> testMods, boolean showDetailInfo) {
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
  public boolean calculateDiff(int expectedNum, int returnedNum) {
    if (expectedNum == 48 && expectedNum - returnedNum >= 5) {
      return false;
    } else if (expectedNum == 39 && expectedNum - returnedNum >= 3) {
      return false;
    } else if (expectedNum == 9 && expectedNum != returnedNum) {
      return false;
    }
    return true;
  }
  public boolean testOK(int userId, boolean showDetailInfo) {
    boolean result = true;
    for (int i = 0; i < testData_.length; ++i) {
      int [] data = testData_[i];
      HomeFootprintResult footprintResult = adapter_.getAll(userId, data[0], data[1]);
      if (footprintResult.getVisitCount() < 100) {
        return true;
      }
      if (!calculateDiff(data[2], footprintResult.getFootprintViews().size())) {
        result = false;
        if (showDetailInfo) {
          System.out.println("userId = " + userId + ", (begin, limit):(" + data[0] + ", "+ data[1] + "), returned Num:"
              + footprintResult.getFootprintViews().size());
        }
      }
    }
    if (!result && showDetailInfo) {
      System.out.println("--------------------------------------");
    }
    return result;
  }

  public void printUsage() {
    System.out.println();
    System.out.println("-------------------------------------------------------------------------");
    System.out.println("Usage: HomeFootprintTest userIdsFile= showDetailInfo=[true|false] [testMod= ... testMod=]");
    System.out.println("userIdsFile是指定的测试用户id的文件目录，一行一个id。testMod可以有多个，如果没有的话，默认测试所有id");
    System.out.println("-------------------------------------------------------------------------");
  }

  public static void main(String args[]) {
    HomeFootprintTest tester = new HomeFootprintTest();
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
      tester.printUsage();
      System.exit(0);
    }
   
    List<Integer> userIdList = MenuCacheTest.getUserIdsFromFile(userIdsFile);
    tester.testGetFootprint(userIdList, testMods, showDetailInfo);
    System.exit(0);

  }
}
