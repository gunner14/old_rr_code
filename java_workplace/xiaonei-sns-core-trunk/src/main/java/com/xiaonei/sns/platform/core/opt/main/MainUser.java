package com.xiaonei.sns.platform.core.opt.main;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

public class MainUser {

    public static final String[] prifxs = { "email", "id" };

    public static void main(String[] args) {
        if (args == null || args.length == 0) {
            return;
        }
        for (String arg : args) {
            if (arg.indexOf("-email=") == 0) {
                String email = arg.replaceAll("-email=", "");
                User user = SnsAdapterFactory.getUserAdapter().getByEmail(email);
                if (user == null) {
                    System.out.println("null");
                    System.exit(0);
                    return;
                } else {
                    System.out.println("[" + user.getId() + "," + user.getAccount() + "]");
                    System.exit(0);
                    return;
                }
            } else if (arg.indexOf("-id=") == 0) {
                String sId = arg.replaceAll("-id=", "");
                Integer id;
                try {
                    id = Integer.parseInt(sId);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                    System.exit(1);
                    return;
                }
                if (id == null) {
                    System.err.println("id is null");
                    System.exit(1);
                    return;
                }
                User user = SnsAdapterFactory.getUserAdapter().get(id);
                if (user == null) {
                    System.out.println("null");
                    System.exit(0);
                    return;
                } else {
                    System.out.println("[" + user.getId() + "," + user.getAccount() + "]");
                    System.exit(0);
                    return;
                }
            }
        }

        System.exit(0);
    }
}
