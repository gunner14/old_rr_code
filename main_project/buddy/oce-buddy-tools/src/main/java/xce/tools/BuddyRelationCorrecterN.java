package xce.tools;

//package xce.tools;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.CheckedBuddyRelation;

import com.xiaonei.xce.buddyrelationcache.BuddyRelationCacheAdapter;
import com.xiaonei.xce.xcealert.XceAlertAdapter;

public class BuddyRelationCorrecterN{
	 
	public static class FromTo {
		public int from_;
		public int to_;
		public FromTo(int from, int to) {
			from_ = from;
			to_ = to;
		}
		public FromTo(FromTo rhp) {
			from_ = rhp.from_;
			to_ = rhp.to_;
		}
	}
	public static void printUsage() {
		System.out.println("Usage: BuddyRelationCorrecterN <input file> <outputfile>");
	}
	
	public static List<FromTo> check(List<FromTo> toBeCheckIds, FileWriter fw, int try_num) {
		List<FromTo> result = new ArrayList<FromTo>();
		BuddyCoreAdapterImpl core = new BuddyCoreAdapterImpl();
    		for(int i = 0; i < toBeCheckIds.size(); ++i) {
			String write = "";
			try {
	        	CheckedBuddyRelation corerelation = core.getBuddyCoreCheckedRelation(toBeCheckIds.get(i).from_, 
	        										toBeCheckIds.get(i).to_);
	        	CheckedBuddyRelation cacherelation = BuddyRelationCacheAdapter.getInstance().getCheckedRelation(toBeCheckIds.get(i).from_, 
						toBeCheckIds.get(i).to_);
	            if(!corerelation.equals(cacherelation) || !BuddyRelationCacheAdapter.getInstance().checkRelation(toBeCheckIds.get(i).from_, toBeCheckIds.get(i).to_)){
	            	if(corerelation.getSuccess() && cacherelation.getSuccess()) {
		            	write = "CoreRelation: " + corerelation.toString() + " CacheRelation: " + cacherelation.toString() + "\n";
				fw.write(write);
	            		BuddyRelationCacheAdapter.getInstance().setRelation(corerelation.getRelation());
	            	} else {
	            		if(try_num == 3) {
	            			String msg = "";
	            			if(!corerelation.getSuccess()) {
	            				msg = corerelation.toString();
	            			} else {
	            				msg = cacherelation.toString();
	            			}
	            			List<String> subjects = new ArrayList<String>();
	            			subjects.add("GetCheckedRelationWarning");
	            			XceAlertAdapter.getInstance().notifyAlertN(subjects, msg, 3);
	            		} else {
	            			result.add(new FromTo(toBeCheckIds.get(i)));
	            		}
	            	}
	            }
			} catch (Exception e) {
				e.printStackTrace();
				try {
					fw.write("[ERROR]Exception id:" + toBeCheckIds.get(i).from_ + " " + toBeCheckIds.get(i).to_);
				} catch (IOException e1) {
					e1.printStackTrace();
				}
				continue;
			}
    	}
		return result;
	}
	public static void main(String[] args){

		if (args.length < 2) {
			printUsage();
			//System.exit(0);
		}
		try {
			FileReader fr = new FileReader(args[0]);
			BufferedReader br = new BufferedReader(fr);
			FileWriter fw = new FileWriter(args[1]);
			int first = 0;
			int second = 0;
			String read;
	    		
			BitSet statusBitSet = new BitSet();
			int isFriend = 0;	
			int total=0;
			String strtime;
			int i = 0;
			
			List<FromTo> toBeCheck = new ArrayList<FromTo>();
			while(br.ready()){
				read = br.readLine();
	    		String[] r = read.split(" ");
	    		try {
	    			first = Integer.parseInt(r[0]);
	    			second = Integer.parseInt(r[1]);
	    			toBeCheck.add(new FromTo(first, second));
					//fw.write(first+" " + second + "\n");
	    		} catch(Exception e) {
	    			e.printStackTrace();
	    			fw.write("[ERROR]File Read Exeption. \n");
	    			continue;
	    		}
			}
			List<FromTo> result = check(toBeCheck, fw, 1);
			List<FromTo> result1 = new ArrayList<FromTo>();
			if(result.size() > 0) {
				Thread.sleep(3 * 60 * 1000);
				result1 = check(result, fw, 2);
			}
			if(result1.size() > 0) {
				Thread.sleep(6 * 60 * 1000);
				check(result1, fw, 2);
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

