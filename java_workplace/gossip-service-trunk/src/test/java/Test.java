import java.text.SimpleDateFormat;
import java.util.Date;

import com.xiaonei.antispam.client.constant.UserBehavior.Gossip;
import com.xiaonei.platform.component.gossip.home.GossipHome;




public class Test {
    public static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
    public static String getTody() {
        return sdf.format(new Date());
    }
    public static void main(String[] args) {
        System.out.println(getTody());
        
    }
}
