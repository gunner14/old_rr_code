package com.xiaonei.platform.component.gossip.biz;

import java.math.BigInteger;
import java.util.Date;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

//import com.renren.message.base.util.tool.UserUtil;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.talk.MessageHandlerAdapter;
import com.xiaonei.xce.notify.NotifyAdapter;
import com.xiaonei.xce.notify.NotifyBody;

@Service
public class GossipNotifyBiz {

	private Log logger = LogFactory.getLog(this.getClass());

	public void sendNotiy(User host, User guest, User cc, int type, int source, String whisper) {
		if (logger.isDebugEnabled()) {
			logger.debug(String.format("host.getId():%d,guest.getId():%d,cc.getId():%d,type:%d,source:%d,whisper:%s", host.getId(),
					guest.getId(), cc.getId(), type, source, whisper));
		}
		try {
			BigInteger sessionId = getSessionFixKey(host.getId(), guest.getId());
			String sessiontype="1";
			NotifyBody nb = new NotifyBody();
			nb.setFromId(host.getId());
			nb.setOwner(guest.getId());
			nb.setSchemaId(type);
			// nb.setSource(gossip.getId());
			nb.setSource(guest.getId());
			nb.setTime(new Date().getTime());
			if (host.getId() != guest.getId()) {
				nb.addToid(guest.getId());
			}
			if (cc != null) {
				nb.addToid(cc.getId());
			}
			nb.setType(type);
			nb.setValue("from_name", host.getName());
			nb.setValue("whisper", whisper);
			nb.setValue("sessionID",sessionId.toString());
			nb.setValue("sessionType", sessiontype);
			NotifyAdapter.getInstance().dispatch(nb);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void sendWebpager(User host, User guest, User cc, Gossip gv) {

		try {
			int to = 0;
			if (host.getId() == guest.getId()) {
				if (cc.getId() != 0) {
					to = cc.getId();
				}
			} else {
				to = guest.getId();
			}
			// if(host.getId() == guest.getId()){
			// return;
			// }
			//			
			String message = gv.getBody().getFilterOriginalBody();
			int index = message.indexOf(":");
			if (message.startsWith("回复") && index != -1 && (index + 1 <= message.length())) {
				message = message.substring(message.indexOf(":") + 1).trim();
			}
			// System.out.println("to"+to+" msg:"+message);
			MessageHandlerAdapter.instance().sendMessage(host.getId(), to, message);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	
	/**
	 * 留言板跳转私信  获取私信对应的sessionId
	 * @param userId  发送者
	 * @param userId2  接受者
	 * @return
	 */
	public BigInteger getSessionFixKey(int userId ,int userId2) {
		
		long[] sortedNumbers = new long[2];
		sortedNumbers[0] = userId >= userId2 ? userId2 : userId;
		sortedNumbers[1] = userId == sortedNumbers[0] ? userId2 : userId;
		sortedNumbers[0] = sortedNumbers[0]<<32;
		sortedNumbers[1] = sortedNumbers[0]|sortedNumbers[1];
		return new BigInteger(String.valueOf(sortedNumbers[1]));
	}

	public static void main(String[] args) {
		String message = "回复xxx:";
		int index = message.indexOf(":");
		if (message.startsWith("回复") && index != -1 && (index + 1 <= message.length())) {
			message = message.substring(message.indexOf(":") + 1).trim();
		}
		System.out.println(" msg:" + message);
	}

}
