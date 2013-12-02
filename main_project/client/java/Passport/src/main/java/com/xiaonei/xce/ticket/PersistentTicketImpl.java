package com.xiaonei.xce.ticket;

final public class PersistentTicketImpl extends TicketImpl {

    private static PersistentTicketImpl _instance = null;

    public PersistentTicketImpl() {
        super("PTicket");
    }

    /**
     * 推荐使用
     * 
     * @return 单例
     */
    public PersistentTicketImpl getInstance() {
        if (PersistentTicketImpl._instance == null) {
            synchronized (PersistentTicketImpl.class) {
                if (PersistentTicketImpl._instance == null) {
                    PersistentTicketImpl._instance = new PersistentTicketImpl();
                }
            }
        }
        return PersistentTicketImpl._instance;
    }
}
