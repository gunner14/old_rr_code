package com.xiaonei.xce.log;

import java.util.Date;
import java.util.Random;

import com.xiaonei.xce.activetrackfacade.ActiveTrackFacadeAdapter;

import xce.util.tools.IPAddress;

public class ActiveTrack {
  private static final XceLoggerReplica logger = XceLoggerReplicaAdapter.getLogger();
  private static final String[][] accessCategories = new String[10][];
  private static final String[][] actionCategories = new String[10][];
  private static final String[][] combinedCategories = new String[10][];

  private static boolean isKaixin = IPAddress.isKaixin();
  private static boolean isXiaonei = IPAddress.isXiaonei();
  static {
    try {
      ReleaseVersion.reportToXceLogger();
    } catch (Throwable e) {
      e.printStackTrace();
    }

    String topLevelCategory;
    if (isXiaonei) {
      topLevelCategory = "ActiveTrackXN";
    } else if (isKaixin) {
      topLevelCategory = "ActiveTrackKX";
    } else {
      topLevelCategory = "ActiveTrack";
    }
    Oce.getLogger().info("Active Track System uses top level category: " + topLevelCategory);
    for (int i = 0; i < 10; ++i) {
      accessCategories[i] = new String[] { topLevelCategory, "access", "bucket_" + i };
      actionCategories[i] = new String[] { topLevelCategory, "action", "bucket_" + i };
      combinedCategories[i] = new String[] { topLevelCategory, "combined", "bucket_" + i };
    }
  }

  private static Random random = new Random(System.currentTimeMillis());
  public static String generateAnonIdent(){
    return Long.toString(System.currentTimeMillis(), 36) + 
        Long.toString(random.nextInt(), 36);
  }

  /** @deprecated use {access} instead; */
  @Deprecated
  public static void access(int userId, String ip, String url) {
    access(userId, ip, url, null);
  }

  /** @deprecated use {access} instead; */
  @Deprecated
  public static void access(int userId, String ip, String url, String decision) {
    access(null, userId, ip, url, decision);
  }

  /**
   * @param decision
   *          由业务逻辑决定的标志。
   * */
  public static void access(String anonymIdent, int userId, String ip, String url, String decision) {
    StringBuffer message = new StringBuffer();
    message.append("A"); // version
    message.append(Long.toString(System.currentTimeMillis(), 36)).append(" ");
    message.append(anonymIdent).append(" ");
    message.append(userId).append(" ");
    message.append(ip).append(" ");
    message.append(url);
    if (decision != null) {
      message.append(" ");
      message.append(decision);
    }
    logger.addLog(accessCategories[Math.abs(userId) % 10], message.toString());
    ActiveTrackFacadeAdapter.getInstance().access(userId, ip, url);
  }

  public static void action(int userId, String[] paths, String action, String object) {
    action(null, userId, paths, action, object);
  }

  /**
   * 记录一个行为。 例如： ActiveTrack.action(202911262, new String[]{"FEEDNEWS",
   * "PROFILE"}, "ADDFRIEND", "200353733"); 上述调用记录了一次加好友操作
   * 
   * @param userId
   * @param paths
   * @param action
   * @param object
   */
  public static void action(String anonymIdent, int userId, String[] paths, String action, String object) {
    StringBuffer message = new StringBuffer();
    message.append("A");
    message.append(Long.toString(System.currentTimeMillis(), 36)).append(" ");
    message.append(anonymIdent).append(" ");
    message.append(userId).append(" ");
    if (paths != null) {
      for (String path : paths) {
        message.append(path).append(" ");
      }
    }
    message.append(action).append(" ");
    message.append(object);
    logger.addLog(actionCategories[Math.abs(userId) % 10], message.toString());
    ActiveTrackFacadeAdapter.getInstance().action(userId, paths, action, object);
  }
  
  /**
   * 以CombinedLog的格式记录一个用户访问
   *  
   * @param userId
   *          用户id
   * @param log
   *          用该行为的各项信息组织一个CombinedLog结构，以此打远程log。CombinedLog的所有内容必须齐全。
   */
  public static void combined(int userid, CombinedLog log) {
    logger.addLog(combinedCategories[Math.abs(userid) % 10], log.toString());
  }
  
  public static void main(String[] args) {

    for (int i = 0; i < 1000; ++i) {
      ActiveTrack.combined(222525090, new CombinedLog("10.2.17.249", "-", 222525090, new Date().getTime(),
          "http://page.renren.com", 200, 60, "http://www.google.com.tw/search?q=" + i,
          "Mozilla/4.08 [en] (Win98; I ;\"Nav\")"));
    }
    System.out.println(generateAnonIdent());
    System.out.println(Long.toBinaryString(Long.MAX_VALUE));
    System.out.println(Long.toString(System.currentTimeMillis(), 36));
    System.exit(0);
  }
}
