package com.renren.datamining.friendsoffriends;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class FriendWithTime {
  public static FriendWithTime parse(String line) throws ParseException {
    // 1100,18577,2008-06-10 18:38:57.0
    int pos1 = line.indexOf(',');
    int pos2 = line.indexOf(',', pos1 + 1);
    int userId = Integer.valueOf(line.substring(0, pos1));
    int friendId = Integer.valueOf(line.substring(pos1 + 1, pos2));
    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    int time = (int) (format.parse(line.substring(pos2 + 1)).getTime() / 1000L);
    return new FriendWithTime(userId, friendId, time);
  }

  private int userId;
  private int friendId;
  private int time;

  public FriendWithTime() {
  }

  public FriendWithTime(int userId, int friendId, int time) {
    this.userId = userId;
    this.friendId = friendId;
    this.time = time;
  }

  public int getUserId() {
    return userId;
  }

  public void setUserId(int userId) {
    this.userId = userId;
  }

  public int getFriendId() {
    return friendId;
  }

  public void setFriendId(int friendId) {
    this.friendId = friendId;
  }

  public int getTime() {
    return time;
  }

  public void setTime(int time) {
    this.time = time;
  }

  public static void main(String[] args) throws ParseException {
    String txt = "1100,18577,2008-06-10 18:38:57.0";
    FriendWithTime o = parse(txt);
    System.out.println(o.userId);
    System.out.println(o.friendId);
    System.out.println(new Date(1000L * o.time));
  }
}
