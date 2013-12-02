package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.BitSet;
import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyRelation;

import com.xiaonei.xce.buddyadapter.buddybyaddtimecache.BuddyByAddTimeCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybyidcache.BuddyByIdCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybynamecache.BuddyByNameCacheAdapter;
import com.xiaonei.xce.buddyadapter.buddybyonlinetimecache.BuddyByOnlineTimeCacheAdapter;
import com.xiaonei.xce.statuscache.StatusCacheAdapter;

public class BuddyListCorrecter {

  public static void printUsage() {
    System.out.println("Usage: BuddyListCorrecter <input file> <outputfile>");
  }


  public static void main(String[] args) {

    if (args.length < 2) {
      printUsage();
      System.exit(0);
    }

    try {
      BuddyCoreAdapterImpl st = new BuddyCoreAdapterImpl();
      FileReader fr = new FileReader(args[0]);
      BufferedReader br = new BufferedReader(fr);
      FileWriter fw = new FileWriter(args[1]);
      int first = 0;
      int second = 0;
      String read;
      String write;

      BitSet statusBitSet = new BitSet();
      // boolean t = true;
      // t = StatusCacheAdapter.getInstance().isStatusTrue(745486616);
      // System.out.println("745486616 is " + t );
      boolean success = StatusCacheAdapter.getInstance().getAllStatus(statusBitSet);

      if (!success) {
        fw.write("Failed to get status cache!!");
        return;
      }

      int isFriend = 0;
      int total = 0;
      String strtime;
      int i = 0;
      boolean finished = false;
      while (br.ready()) {

        if (finished) {
          read = br.readLine();
          try {
            first = Integer.parseInt(read);
          } catch (Exception e) {
            e.printStackTrace();
            fw.write("[ERROR]File Read Exeption.");
            continue;
          }
          finished = false;
        }
        write = "";
        boolean reload = false;
        boolean isbyidcorrect = true;
        try {
          List<BuddyRelation> brl = st.getFriendList(first, 0, -1);
          int byidsize = BuddyByIdCacheAdapter.getInstance().getFriendCount(first);
          int byonlinesize = BuddyByOnlineTimeCacheAdapter.getInstance().getFriendCount(first);
          int bynamesize = BuddyByNameCacheAdapter.getInstance().getFriendCount(first);
          int byaddtimesize = BuddyByAddTimeCacheAdapter.getInstance().getFriendCount(first);
          int coresize = brl.size();
          if (byidsize != brl.size()) {
            for (int j = 0; j < brl.size(); j++) {
              if (statusBitSet.get(brl.get(j).toId)) {
                coresize--;
              }
            }
            if (coresize != byidsize) {
              isbyidcorrect = false;
              // write = first + " byidsize:" + byidsize + " validcoresize" + coresize + "\n";
              // fw.write(write);
              // reload = true;
            }
          }

          if (byidsize != byaddtimesize) {
            write = first + " byidsize:" + byidsize + " byaddtimesize:" + byaddtimesize + "\n";
            fw.write(write);
            reload = true;
          }
          if (byidsize != bynamesize) {
            write = first + " byidsize:" + byidsize + " bynamesize:" + bynamesize + "\n";
            fw.write(write);
            reload = true;
          }
          if (byidsize != byonlinesize) {
            write = first + " byidsize:" + byidsize + " byonlinesize:" + byonlinesize + "\n";
            fw.write(write);
            reload = true;
          }

          if (!isbyidcorrect) {
            if (!reload) {
              write = first + " byidsize:" + byidsize + " tempmastersize" + coresize + "\n";
              fw.write(write);
              reload = true;
            } else {
              write = first + " byidsize:" + byidsize + " validcoresize" + coresize + "\n";
              fw.write(write);
            }
          }


          if (reload) {
            BuddyByIdCacheAdapter.getInstance().reload(first);
            reload = false;
          }

        } catch (Exception e) {
          // e.printStackTrace();
          fw.write("[ERROR]Exception id:" + first);
          continue;
        }
        // fw.write(write);
        finished = true;
        // System.out.println( write +"/" +total);
      }


      br.close();
      fr.close();
      fw.close();
      System.out.println("done ");
      // stat.endFinally();

    } catch (Exception e) {
      e.printStackTrace();
    }
    System.exit(0);
  }
}
