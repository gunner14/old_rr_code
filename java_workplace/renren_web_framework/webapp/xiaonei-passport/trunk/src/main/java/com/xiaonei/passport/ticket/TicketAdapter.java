package com.xiaonei.passport.ticket;


import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.xce.ticket.PersistentTicketImpl;
import com.xiaonei.xce.ticket.TemporaryTicketImpl;
import com.xiaonei.xce.ticket.Ticket;

public class TicketAdapter implements ITicketAdapter{
	private static ITicketAdapter _instance=new TicketAdapter();
	private static Ticket ticket=new  TemporaryTicketImpl();
	private static Ticket passport=new PersistentTicketImpl();
	private TicketAdapter(){}
	public static ITicketAdapter getInstance(){
		return _instance;
	}
	@Override
	public Ticket getTikcetAdapter(TicketType type) {
		if(TicketType.TICKET==type){
			return ticket;
		}else{
			return passport;
		}
	}
	
}
