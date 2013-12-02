package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;

import com.xiaonei.xce.usercount.UserCountAdapter;

public class BuddyApplyCheck {

  public static void printUsage() {
    System.out.println("Usage: BuddyApplyCheck <input file> <outputfile>");
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
        } catch (Exception e) {
          e.printStackTrace();
          fw.write("[ERROR]File Read Exeption.");
          continue;
        }
        try {
          // List<BuddyRelation> brl = st.getFriendList(first, 0, -1);
          int bcapplycount = -1;
          bcapplycount = st.getAppliedCount(first);
          int ucapplycount = -1;
          ucapplycount = UserCountAdapter.getInstance().get(first, 3);
          if (-1 == bcapplycount || -1 == ucapplycount) {
            fw.write("failid: " + first + " " + bcapplycount + " " + ucapplycount + "\n");
            continue;
          }
          if (ucapplycount > 101) {
            write = "repair id: " + first + " " + bcapplycount + " " + ucapplycount + "\n";
            fw.write(write);
            st.reload(first);
            bcapplycount = st.getAppliedCount(first);
            if (bcapplycount > 101) {
              ucapplycount = 101;
              UserCountAdapter.getInstance().set(first, 3, 101);
            } else {
              ucapplycount = bcapplycount;
              UserCountAdapter.getInstance().set(first, 3, bcapplycount);
            }
            fw.write("repaired id: " + first + " " + bcapplycount + " " + ucapplycount + "\n");
          } else if ((bcapplycount != ucapplycount)) {
            if (bcapplycount > 101 && ucapplycount == 101) {
              // donoting
            } else {
              write = "repair id: " + first + " " + bcapplycount + " " + ucapplycount + "\n";
              fw.write(write);
              st.reload(first);
              bcapplycount = st.getAppliedCount(first);
              if (bcapplycount > 101) {
                ucapplycount = 101;
                UserCountAdapter.getInstance().set(first, 3, 101);
              } else {
                ucapplycount = bcapplycount;
                UserCountAdapter.getInstance().set(first, 3, bcapplycount);
              }
              fw.write("repaired id: " + first + " " + bcapplycount + " " + ucapplycount + "\n");
            }
          }
          // fw.write("check id:" + first + " " + bcapplycount + " " + ucapplycount + "done. \n" );
        } catch (Exception e) {
          // e.printStackTrace();
          fw.write("[ERROR]Exception id:" + first);
          continue;
        }
        first = 0;
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
