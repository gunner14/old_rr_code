package com.xiaonei.xce.blt;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;

public class BulletinManager {
  public static void main(String args[]) {
    if (args.length != 2) {
      System.out.println("输入包含一个文件路径(每行一个userId)，一个bulletinId, 程序的作用是将文件中所有用户id" +
      		"对bulletinId关闭");
      System.exit(0);
    }
    int bulletinId = Integer.valueOf(args[1]);
    BulletinRecord bulletin = BulletinAdapter.getInstance().get(bulletinId);
    System.out.println(bulletin.getBulletinId() + " " + bulletin.getTitle() + " "
        + bulletin.getTimeScope()[0].beginTime + " " + bulletin.getTimeScope()[0].endTime);
    //System.exit(0);
    File userIdsFile = new File(args[0]);
    try {
      BufferedReader reader = new BufferedReader(new FileReader(userIdsFile));
      String line = null;
      while ((line = reader.readLine()) != null) {
        try {
          int userId = Integer.valueOf(line.trim());
          //if (userId % 10 == 9) {            
            System.out.println(line + " closed for " + bulletinId);
            BulletinAdapter.getInstance().close(userId, bulletinId);
            Thread.sleep(5);
          //}
        } catch (Exception e) {
          e.printStackTrace();
        }
      }
      reader.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    }
    System.exit(0);
  }
}
