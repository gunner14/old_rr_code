package com.xiaonei.passport.ticket;


import java.util.List;

import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.xce.ticket.TicketInfo.Type;
import com.xiaonei.xce.ticket.TicketInfo;


public interface ITicketService {
	long DEFAULT_TICKET_TIME=60*60*1000;
	long DEFAULT_AUTO_PASSPORT_TIME=30*60*60*1000;
	
	/**根据参数信息创建票
	 * @param info 生成票需要的相关信息，确保所有字段都被设置
	 * @param expiredTime 过期时间
	 * @return 生成的结果 */
	public String createTicket(long userId, long realId, String ip, int loginType, long expiredTime, TicketType ticketType);
	

	
	/** 验证票的有效性 */
	public long verifyTicket(String ticket, int loginType, TicketType ticketType);
	
	/** 验证票的有效性 */
	public long verifyTicket(String ticket, List<Integer> loginType, TicketType ticketType);
	
	/** 获取票对应的信息 */
	public TicketInfo getTicketInfo(String ticket, TicketType ticketType);
	
	/** 根据票销毁票 */
	public boolean destroyTicket(String ticket, TicketType ticketType);
	
	/** 根据id销毁票
	＊@param id 要销毁的用户id
	 * @param type 要销毁的票类型*/
	public boolean destroyTicketById(long id, int type, TicketType ticketType);
}
