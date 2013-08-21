package com.xiaonei.login.controllers.base;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.xce.passport.PassportAdapterFactory;

public class LocalTest {

    public static void main(final String[] args) {
        //        final String userName=args[0];
        //        final String password=args[1];
        //        final User user = new User();
        final Map<String, String> profiler = new HashMap<String, String>();
        profiler.put("User-Agent", "firefox");
        profiler.put("IP", "10.2.18.27");
        System.out.println(PassportAdapterFactory.getWebTicket().createTicket(230447552, profiler));
        //        System.out.println(PassportAdapterFactory.getWebLogin().id2Ticket(userId));
        //        final Account loginVirtual = PassportAdapterFactory.getWebLogin().loginVirtual(//
        //                Globals.defaultLogin,//
        //                "zhangkun1123@163.com",//
        //                "25f9e794323b453885f5181f1b624d0b",//
        //                "renren.com", //
        //                -1,//T
        //                true//
        //                );
        //        //        System.out.println(loginVirtual.getStatus());
        //        System.out.println(loginVirtual.getTicket());
        //        System.out.println(PassportAdapterFactory.getWebTicket().createTicket(loginVirtual.getId(),
        //                profiler));
        //        System.out.println(loginVirtual.toString());
        //
        //        //        System.out.println(PassportAdapterFactory.getWebTicket().verifyTicket(
        //        //                "3444d4354df7622c6c14d814e879eebe6", profiler));
        //        //        System.out.println(PassportAdapterFactory.getWebLogin().getAccount("nice2mietu"));
        //        //
        //        //        System.out.println(SnsAdapterFactory.getUserPassportAdapter().getUserPassport(2000044756));
        System.exit(0);
    }
}
