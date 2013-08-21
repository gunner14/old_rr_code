package com.xiaonei.passport.ticket;

import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.xce.ticket.Ticket;
/**
 * 根据票的类型获取票的处理器
 * @author xiaodong.yuan
 *
 */
public interface ITicketAdapter {
	public Ticket getTikcetAdapter(TicketType type);
}
