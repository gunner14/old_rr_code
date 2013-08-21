package xce.tools.objectcache;

import java.util.ArrayList;
import java.util.Vector;

import xce.svcapi.TaskManagerAPIPrx;
import xce.svcapi.TaskManagerAPIPrxHelper;
import xce.util.channel.Channel;
import MyUtil.ObjectCachePrx;
import MyUtil.ObjectCachePrxHelper;

public class PreloadOceAction extends Consumer {
	private final String endpoints;
	private final int[] categories;
	private final int cluster;

	private final Ice.Communicator ic;
	private final ObjectCachePrx prx[];
	private final TaskManagerAPIPrx tprx[];

	public PreloadOceAction(Producer producer, String endpoints,int[] categories, int cluster,int level,int maxthreads) {
		super(producer);
		this.endpoints = endpoints;
		this.categories = categories;
		ic = Channel.newOceChannel(endpoints).getCommunicator();
		if (cluster == 0) {
			this.cluster = 1;
			prx = new ObjectCachePrxHelper[1];			
			prx[0] = ObjectCachePrxHelper.uncheckedCast(ic.stringToProxy("SC@"
					+ this.endpoints));
			tprx=new TaskManagerAPIPrxHelper[1];
			tprx[0]=TaskManagerAPIPrxHelper.uncheckedCast(ic.stringToProxy("TMA@"+this.endpoints));
			tprx[0].config(level, 1, maxthreads, 128*1024);
		} else {
			this.cluster = cluster;			
			prx = new ObjectCachePrxHelper[cluster];			
			for (int i = 0; i < cluster; ++i) {
				prx[i] = ObjectCachePrxHelper.uncheckedCast(ic
						.stringToProxy("SC@" + this.endpoints + i));
			}
			tprx=new TaskManagerAPIPrxHelper[cluster];
			for(int i=0;i<cluster;++i){
				tprx[i]=TaskManagerAPIPrxHelper.uncheckedCast(ic.stringToProxy("TMA@"+this.endpoints+i));
				tprx[i].config(level, 1, maxthreads, 128*1024);
			}
		}
	}

	@Override
	protected void consume(Long[] batch) {
		System.out.println("OceChannel Preload " + batch.length + " objects");
		Vector<ArrayList<Long>> seperate = new Vector<ArrayList<Long>>();
		for (int i = 0; i < cluster; ++i) {
			seperate.add(new ArrayList<Long>());
		}
		for (Long one : batch) {
			seperate.get((int) (one % cluster)).add(one);
		}

		for (int i = 0; i < cluster; ++i) {

			/*if  (i!= 5 && i != 3 && i!=7 && i!=9) {
				continue; 
			}*/

			ArrayList<Long> sepBatch = seperate.get(i);
			long[] sepLongBatch = new long[sepBatch.size()];
			for (int pos = 0; pos < sepBatch.size(); ++pos) {
				sepLongBatch[pos] = sepBatch.get(pos);
			}
			for (int category : categories) {
				prx[i].preloadObjects(category, sepLongBatch);
			}
		}
	}
}
