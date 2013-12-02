package xce.tools;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.xce.buddyadapter.buddyfriends.BuddyFriendsCacheAdapter;

/**
 * BuddyApplyAnalyzer
 * 
 * 用途： 从指定文件中读取好友申请id序列对，查询他们的共同好友，输出到指定文件中
 * 
 * 用法： BuddyApplyAnalyzer [inputFilePath] [outputFilePath] inputFilePath指向的文件格式为："userId1 uesrId2\n"
 * outputFilePath指向的文件格式为："userId1 uesrId2 mutualCount\n"
 * 
 * 使用注意： 本程序调用BuddyFriendsCache查询用户共同好友。 如果用户好友不在缓存中，查询会返回0，可以通过连续调用此程序2次来改善缓存miss导致的结果误差
 * 
 * @author xiaofeng.liang@renren-inc.com
 * 
 */
public class BuddyApplyAnalyzer {
  public static void main(String[] args) {

    if (args.length != 2) {
      System.out.println("usage:");
      System.out.println("BuddyApplyAnalyzer [inputFilePath] [outputFilePath]");
      return;
    }

    String dataPath = args[0];
    String resultPath1 = args[1];

    try {
      int userId1;
      int userId2;
      String line = "";
      BufferedReader reader = new BufferedReader(new FileReader(new File(dataPath)));
      BufferedWriter writer;

      // 覆盖已有文件，再写入
      writer = new BufferedWriter(new FileWriter(resultPath1, false));
      writer.write("");
      writer = new BufferedWriter(new FileWriter(resultPath1, true));

      while ((line = reader.readLine()) != null) {
        String[] fields = line.split(" ");
        userId1 = Integer.valueOf(fields[0]);
        userId2 = Integer.valueOf(fields[1]);

        List<Integer> listRes = new ArrayList<Integer>();
        listRes = BuddyFriendsCacheAdapter.getInstance().getCommonFriends(userId1, userId2);
        writer.write(userId1 + " " + userId2 + " " + listRes.size() + " " + fields[2] + "\n");
      }

      reader.close();
      writer.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
    System.exit(0);
  }
}
