package xce.tools;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.BuddyRelation;

public class BuddyRemoveFriend{
	 
	public static void printUsage() {
                System.out.println("Usage: BuddyRemoveFriend <input file> <outputfile>");
        }
 
	public static void main(String[] args){

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
	
/*		BitSet statusBitSet = new BitSet();
		//boolean t = true;
		//t = StatusCacheAdapter.getInstance().isStatusTrue(745486616);
		//System.out.println("745486616 is " + t );
		boolean success = StatusCacheAdapter.getInstance().getAllStatus(statusBitSet);
	
		if(!success){ 
			write = "Failed to get status cache!!";
			return;
		}
*/
		int isFriend = 0;	
    		int total=0;
		String strtime;
		int i = 0;
		boolean finished = false;
    		while(br.ready()){

			read = br.readLine();
			String[] r = read.split(" ");
			try{
				first = Integer.parseInt(r[0]);
				second = Integer.parseInt(r[1]);  
				fw.write(first+" and " + second + "\n");
			}
			catch(Exception e){
				e.printStackTrace();
				fw.write("[ERROR]File Read Exeption. \n");
				continue;
			}
			try {
				//List<BuddyRelation> brl = st.getFriendList(first, 0, -1);
				BuddyRelation relation = new BuddyRelation(first,second,BuddyRelation.BuddyDesc.Friend);
				st.removeFriend(relation);
					
				Thread.sleep(100);	
				fw.write(first+ " and "+ second + " done \n");
			} catch (Exception e) {
        	                //e.printStackTrace();
				fw.write("[ERROR]Exception id:" + first + "\n");
				continue;
                	}
			//fw.write(write);
			//System.out.println( write +"/" +total);
    		}
    		
    		
    		br.close();
    		fr.close();
    		fw.close();
		System.out.println("done " );
    		//stat.endFinally();
    		
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    	System.exit(0);
	}
}
