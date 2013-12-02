package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyApply;
import mop.hi.oce.domain.buddy.BuddyRelation;

public class BuddyDenyApply {

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

      /*
       * BitSet statusBitSet = new BitSet(); //boolean t = true; //t =
       * StatusCacheAdapter.getInstance().isStatusTrue(745486616);
       * //System.out.println("745486616 is " + t ); boolean success =
       * StatusCacheAdapter.getInstance().getAllStatus(statusBitSet);
       * 
       * if(!success){ write = "Failed to get status cache!!"; return; }
       */
      int isFriend = 0;
      int total = 0;
      String strtime;
      int i = 0;
      boolean finished = false;
      while (br.ready()) {

        read = br.readLine();
        try {
          first = Integer.parseInt(read);
          fw.write(first + "\n");
        } catch (Exception e) {
          e.printStackTrace();
          fw.write("[ERROR]File Read Exeption.");
          continue;
        }
        try {
          // List<BuddyRelation> brl = st.getFriendList(first, 0, -1);

          List<BuddyRelation> _cacheData2 = st.getApplySeq(first, 0, -1);
          fw.write(first + " " + _cacheData2.size());
          for (int j = 0; j < _cacheData2.size(); j++) {
            BuddyApply ba = new BuddyApply(first, _cacheData2.get(j).toId);
            // st.denyApply(ba);
            fw.write(first + " " + _cacheData2.get(j).toId + "\n");
          }

          // Thread.sleep(100);
          fw.write(first + " done \n");
        } catch (Exception e) {
          // e.printStackTrace();
          fw.write("[ERROR]Exception id:" + first);
          continue;
        }
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
}
