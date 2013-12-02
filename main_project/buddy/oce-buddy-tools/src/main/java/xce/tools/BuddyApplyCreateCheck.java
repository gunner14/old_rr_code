package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;

import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheAdapterN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheDataN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheFactoryN;
import com.xiaonei.xce.buddyadapter.buddyapplycache.BuddyApplyCacheWithTotalN;

public class BuddyApplyCreateCheck {

  public static void printUsage() {
    System.out.println("Usage: BuddyApplyCreateCheck <input file> <outputfile>");
  }

  public static void main(String[] args) {

    if (args.length < 2) {
      printUsage();
      System.exit(0);
    }

    try {
      BuddyCoreAdapterImpl st = new BuddyCoreAdapterImpl();
      BuddyApplyCacheAdapterN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN> _cacheAdapter =
          new BuddyApplyCacheAdapterN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN>(
              new MyBuddyApplyCacheFactoryN());
      FileReader fr = new FileReader(args[0]);
      BufferedReader br = new BufferedReader(fr);
      FileWriter fw = new FileWriter(args[1]);
      int id = 0;
      String read;
      String write;

      /*
       * BitSet statusBitSet = new BitSet(); //boolean t = true; //t =
       * StatusCacheAdapter.getInstance().isStatusTrue(745486616);
       * //System.out.println("745486616 is " + t ); boolean success =
       * StatusCacheAdapter.getInstance().getAllStatus(statusBitSet);
       * 
       * if(!success){ write = "Failed to get status cache!!"; return; }
       */
      while (br.ready()) {

        read = br.readLine();
        try {
          id = Integer.parseInt(read);
        } catch (Exception e) {
          e.printStackTrace();
          fw.write("[ERROR]File Read Exeption.");
          continue;
        }
        try {
          // List<BuddyRelation> brl = st.getFriendList(first, 0, -1);
          int bcapplycount = 0;
          bcapplycount = st.getAppliedCount(id);
          int acapplycount = 0;
          List<MyBuddyApplyCacheDataN> _cacheData2 = _cacheAdapter.getBuddyApplyCache(id, 0, 100);
          acapplycount = _cacheData2.size();
          if (bcapplycount > 10) {
            write =
                "repair id: " + id + " " + bcapplycount + " " + acapplycount + " nothing to do \n";
            fw.write(write);
          } else if ((bcapplycount != acapplycount)) {
            write = "repair id: " + id + " " + bcapplycount + " " + acapplycount + "\n";
            fw.write(write);
            _cacheAdapter.reload(id);
            fw.write("repaired id: " + id + " " + bcapplycount + " " + acapplycount + "\n");
          }
        } catch (Exception e) {
          // e.printStackTrace();
          fw.write("[ERROR]Exception id:" + id);
          continue;
        }
        id = 0;
        // fw.write(write);
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

  private static class MyBuddyApplyCacheDataN extends BuddyApplyCacheDataN {

  }

  private static class MyBuddyApplyCacheWithTotalN
      extends BuddyApplyCacheWithTotalN<MyBuddyApplyCacheDataN> {

  }

  private static class MyBuddyApplyCacheFactoryN
      implements
        BuddyApplyCacheFactoryN<MyBuddyApplyCacheDataN, MyBuddyApplyCacheWithTotalN> {

    public MyBuddyApplyCacheDataN createBuddyApplyCacheDataN() {
      // TODO Auto-generated method stub
      return new MyBuddyApplyCacheDataN();
    }

    public MyBuddyApplyCacheWithTotalN createBuddyApplyCacheWithTotalN() {
      // TODO Auto-generated method stub
      return new MyBuddyApplyCacheWithTotalN();
    }

  }
}
