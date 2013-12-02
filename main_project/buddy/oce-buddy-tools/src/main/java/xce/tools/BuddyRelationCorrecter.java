package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.BitSet;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyRelation;

import com.xiaonei.xce.buddyrelationcache.BuddyRelationCacheAdapter;

public class BuddyRelationCorrecter {

  public static void printUsage() {
    System.out.println("Usage: BuddyRelationCorrecter <input file> <outputfile>");
  }


  public static void main(String[] args) {

    if (args.length < 2) {
      printUsage();
      System.exit(0);
    }

    try {
      BuddyCoreAdapterImpl core = new BuddyCoreAdapterImpl();
      FileReader fr = new FileReader(args[0]);
      BufferedReader br = new BufferedReader(fr);
      FileWriter fw = new FileWriter(args[1]);
      int first = 0;
      int second = 0;
      String read;
      String write;

      BitSet statusBitSet = new BitSet();

      int isFriend = 0;
      int total = 0;
      String strtime;
      int i = 0;
      while (br.ready()) {

        read = br.readLine();
        String[] r = read.split(" ");
        try {
          first = Integer.parseInt(r[0]);
          second = Integer.parseInt(r[1]);
          // fw.write(first+" " + second + "\n");
        } catch (Exception e) {
          e.printStackTrace();
          fw.write("[ERROR]File Read Exeption. \n");
          continue;
        }

        write = "";
        boolean reload = false;
        try {
          BuddyRelation corerelation = core.getBuddyCoreRelation(first, second);
          BuddyRelation cacherelation =
              BuddyRelationCacheAdapter.getInstance().getRelation(first, second);

          if (corerelation.getDesc() != cacherelation.getDesc()) {
            write =
                first + " " + second + " corerelation:" + corerelation.getDesc()
                    + " cacherelation:" + cacherelation.getDesc() + "\n";
            fw.write(write);
            reload = true;
          }

          if (reload) {
            BuddyRelationCacheAdapter.getInstance().setRelation(corerelation);
            reload = false;
          }

        } catch (Exception e) {
          // e.printStackTrace();
          fw.write("[ERROR]Exception id:" + first + " " + second);
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
