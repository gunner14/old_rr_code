package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyRelation;

import com.xiaonei.xce.XceAdapter;
import com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter;

public class CheckCoreAndId {

  static class FileParser {

    public static List<String> paser(String rfile) {
      try {
        HashSet<Long> hashSet = new HashSet<Long>();
        List<String> returnValue = new ArrayList<String>();
        FileReader fr = new FileReader(rfile);
        BufferedReader br = new BufferedReader(fr);

        String read;

        while (br.ready()) {
          read = br.readLine();
          String[] firstAndsecond = read.split("\t");
          for (int i = 0; i < firstAndsecond.length; i++) {
            returnValue.add(firstAndsecond[i]);
          }

        }

        br.close();
        fr.close();
        return returnValue;

      } catch (IOException e) {
        e.printStackTrace();
      }
      return null;

    }
  }


  static class InValidStateFromCache {

    public static int check(int host, List<Integer> listFriend) {
      int valid = 0;
      for (int i = 0; i < listFriend.size(); i++) {
        valid = BuddyByIdCacheAdapter.getInstance().filter(host, listFriend);
      }
      return listFriend.size() - valid;
    }
  }

  static class InValidState {

    public static int check(List<Integer> listFriend) {

      if (listFriend.size() == 0) return 0;

      StringBuffer idstring = new StringBuffer();

      for (int i = 0; i < listFriend.size(); i++) {
        if (i == 0) {
          idstring.append(listFriend.get(i).toString());
        } else {
          idstring.append("," + listFriend.get(i).toString());
        }

      }
      int count = 0;


      try {
        Connection c = XceAdapter.getInstance().getReadConnection("user_passport_status");
        try {
          Statement s = c.createStatement();
          try {
            String sql =
                "SELECT count(id)  FROM user_passport WHERE status<>0 and status<>3  and  id IN ("
                    + idstring.toString() + ")";

            ResultSet result = s.executeQuery(sql);
            try {
              String info = null;
              while (result.next()) {
                count = result.getInt("count(id)");

              }
            } catch (Exception e) {
              System.out.println(e);
            } finally {
              result.close();
            }
          } catch (Exception e) {
            System.out.println(e);
          } finally {
            s.close();
          }
        } catch (Exception e) {
          System.out.println(e);
        } finally {
          c.close();
        }
      } catch (Exception e) {
        System.out.println(e);
      }
      return count;
    }

  }


  static class CheckData {
    static final BuddyCoreAdapterImpl adapter = new BuddyCoreAdapterImpl();

    static public void check(int userId) {
      try {
        int countFromIdCache = BuddyByIdCacheAdapter.getInstance().getFriendCount(userId);
        List<BuddyRelation> list = adapter.getFriendList(userId, 0, -1);
        int invalid = 0;

        List<Integer> listFriend = new ArrayList<Integer>();

        for (int i = 0; i < list.size(); i++) {
          int friend = 0;
          BuddyRelation data = list.get(i);
          {
            friend = data.toId;

          }
          listFriend.add(friend);

        }

        // invalid = InValidState.check(listFriend) ;
        invalid = InValidStateFromCache.check(userId, listFriend);

        if ((list.size() - invalid) != countFromIdCache) {
          System.out.println("userId : " + userId + " in Core : " + (list.size() - invalid)
              + " in IdCache :" + countFromIdCache);

        }

      } catch (Exception e) {
        e.printStackTrace();
      }


    }

    static public void check(int userId, FileWriter fw) {
      try {
        int countFromIdCache = BuddyByIdCacheAdapter.getInstance().getFriendCount(userId);
        List<BuddyRelation> list = adapter.getFriendList(userId, 0, -1);
        int invalid = 0;

        List<Integer> listFriend = new ArrayList<Integer>();

        for (int i = 0; i < list.size(); i++) {
          int friend = 0;
          BuddyRelation data = list.get(i);
          {
            friend = data.toId;

          }
          listFriend.add(friend);

        }

        invalid = InValidState.check(listFriend);

        if ((list.size() - invalid) != countFromIdCache) {
          fw.write(userId + " validcoresize : " + (list.size() - invalid) + " byidsize :"
              + countFromIdCache + "\n");

        }

      } catch (Exception e) {
        e.printStackTrace();
      }


    }
    /*
     * static public void check(List<Integer> userIds) { Map<Integer,Integer> mapFromIdCache =
     * BuddyByIdCacheAdapter.getInstance().getFriendCounts(userIds);
     * 
     * }
     */

  };

  /*
   * public static void main(String[] args) {
   * 
   * if (args.length < 1) { System.out.println("please input id files "); System.exit(0); }
   * 
   * String file = new String(); file = args[0];
   * 
   * try { List<String> list= FileParser.paser(file);
   * 
   * for(int i=0; i< list.size() ; i++) { CheckData.check(Integer.parseInt(list.get(i))); }
   * 
   * }catch(Exception e) { e.printStackTrace(); }
   * 
   * 
   * 
   * System.exit(0); }
   */
}
