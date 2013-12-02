package xce.tools;

import java.io.FileWriter;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;

import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.buddyadapter.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybynamecache.BuddyByNameCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybyonlinetimecache.BuddyByOnlineTimeCacheAdapter;

public class BuddyListCorrecterById {

  public static void printUsage() {
    System.out.println("Usage: BuddyListCorrecter <begin id> <end id> <output file>");
  }

  public static void main(String[] args) {

    if (args.length < 3) {
      printUsage();
      System.exit(0);
    }

    try {
      BuddyCoreAdapterImpl st = new BuddyCoreAdapterImpl();
      Integer begin = Integer.parseInt(args[0]);
      Integer end = Integer.parseInt(args[1]);
      FileWriter fw = new FileWriter(args[2]);
      Integer max = begin;
      Integer count = 0;
      System.out.println("begin: " + begin + " end:" + end);
      Connection conn = null;
      ResultSet rs = null;
      String write;
      while (max < end && max >= 0) {
        try {
          write = "";
          count = 0;
          conn = XceAdapter.getInstance().getReadConnection("user_passport");
          PreparedStatement stmt =
              conn.prepareStatement("SELECT id FROM user_passport WHERE id > " + max
                  + " LIMIT 10000");
          rs = stmt.executeQuery();
          Integer id = 1;
          while (rs.next()) {
            write = "";
            id = rs.getInt("id");
            max = Math.max(max, id);
            // write = count + " " + id + "\n";;
            count++;

            boolean reload = false;
            try {
              // List<BuddyRelation> brl = st.getFriendList(id, 0, -1);
              int byidsize = BuddyByIdCacheAdapter.getInstance().getFriendCount(id);
              int byonlinesize = BuddyByOnlineTimeCacheAdapter.getInstance().getFriendCount(id);
              int bynamesize = BuddyByNameCacheAdapter.getInstance().getFriendCount(id);
              int byaddtimesize = BuddyByAddTimeCacheAdapter.getInstance().getFriendCount(id);
              /*
               * if(byidsize != brl.size()){ //write = id + " byidsize != brl.size()\n"; int
               * checksize = brl.size(); for(int j=0; j<brl.size();j++){
               * if(statusBitSet.get(brl.get(j).toId)){ checksize--; } } if(checksize != byidsize){
               * write = id + " byidsize != checked coresize\n"; reload = true; } }
               */
              if (byidsize != byonlinesize) {
                write = id + " byidsize:" + byidsize + " byonlinesize:" + byonlinesize + "\n";
                reload = true;
              } else if (byidsize != bynamesize) {
                write = id + " byidsize:" + byidsize + " bynamesize:" + bynamesize + "\n";
                reload = true;
              } else if (byidsize != byaddtimesize) {
                write = id + " byidsize:" + byidsize + " byaddtimesize:" + byaddtimesize + "\n";
                reload = true;
              }

              if (reload) {
                // BuddyByIdCacheAdapter.getInstance().reload(id);
                reload = false;
              }

            } catch (Exception e) {
              // e.printStackTrace();
              write = "[ERROR]Exception id: " + id;
              continue;
            }
            if (write != "") {
              fw.write(write);
            }
          }
        } catch (Exception e) {

          e.printStackTrace();
        } finally {
          if (conn != null) {
            try {
              conn.close();
            } catch (Exception e) {
              e.printStackTrace();
            }
          }
        }
        System.out.println("Produced " + count + " userIds, start at " + max);
      }

      fw.close();
      System.out.println("done ");
      // stat.endFinally();

    } catch (Exception e) {
      e.printStackTrace();
    }
    System.exit(0);
  }
}
