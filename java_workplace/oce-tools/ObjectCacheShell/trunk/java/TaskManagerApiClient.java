import xce.svcapi.TaskManagerAPIPrx;
import xce.svcapi.TaskManagerAPIPrxHelper;
import xce.util.channel.Channel;
import Ice.ObjectPrx;


public class TaskManagerApiClient {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try{
			for(int i=0;i<1;i++){
			ObjectPrx op=Channel.newOceChannel("").getCommunicator().stringToProxy("TMA@UserNetworkN9");
			TaskManagerAPIPrx tmap=TaskManagerAPIPrxHelper.uncheckedCast(op);
			System.out.println(tmap.size(-4));
			System.out.println(tmap.size(-5));
			}
		}catch(Exception e){
			e.printStackTrace();
		}
		System.exit(0);
	}

}

