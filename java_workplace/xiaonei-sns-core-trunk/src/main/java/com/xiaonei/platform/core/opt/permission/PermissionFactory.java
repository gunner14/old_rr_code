package com.xiaonei.platform.core.opt.permission;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.dom4j.Document;
import org.dom4j.Element;

/**
 * 统一权限模型工厂类，完成权限的初始化以及如何获取权限对象。
 * 
 * @author yunlong.bai@opi-corp.com
 * @version 0.1
 * 
 */
public class PermissionFactory {

    private Map<String, Permission> permissions = new HashMap<String, Permission>();

    public static String stragegy_path = "/com/xiaonei/platform/core/opt/permission/source/stragegy.xml";

    private Map<String, Message> msg = new HashMap<String, Message>();

    private static final PermissionFactory instance = new PermissionFactory();

    public static PermissionFactory getInstance() {

        return instance;

    }

    public PermissionFactory() {
        this.init(PermissionFactory.stragegy_path);

    }

    @SuppressWarnings("unchecked")
    public void init(String path) {
        InputStream inputstream = null;
        try {
            //System.out.println("-----------------path:"+path) ;
            inputstream = PermissionFactory.class.getResourceAsStream(path);

            // path ="e:/trunk"+"/WEB-INF/conf/strategy/stragegy.xml" ;
            Document doc = MyXml.read(inputstream);
            Element root = doc.getRootElement();
            List<Element> modules = root.elements("module");

            if (modules == null || modules.size() == 0) return;
            for (Element el : modules) {
                this.initPermission(el);
            }

            List<Element> stategys = root.elements("strategy");

            stategys = stategys.get(0).elements();
            if (stategys == null) return;

            for (Element el : stategys) {
                this.initMessage(el);
            }
        } catch (Exception e) {
            e.printStackTrace(System.out);
        } finally {
            if (inputstream != null) {
                try {
                    inputstream.close();
                } catch (Exception e) {}
            }
        }

    }

    private void initMessage(Element msg) throws Exception {

        String className = msg.attributeValue("key");
        String value = msg.attributeValue("value");
        String isRedirect = msg.attributeValue("isRedirect");
        Message m = new Message(className, value);
        if (isRedirect != null) {
            m.setRedirect(new Boolean(isRedirect).booleanValue());
            m.setRedirectUrl(msg.attributeValue("redirectUrl"));
        }
        //System.out.println(m);
        this.msg.put(className, m);

    }

    public Message getMessageByKey(String className) {

        Message msg = this.msg.get(className);

        return msg;
    }

    @SuppressWarnings("unchecked")
    private void initPermission(Element module) throws Exception {

        String className = module.attributeValue("className");
        //		System.out.println("---------className:"+className) ;
        Permission per = (Permission) Class.forName(className).newInstance();
        List<Element> actions = module.elements();
        if (actions != null) {
            for (int i = 0; i < actions.size(); i++) {
                String action = actions.get(i).attributeValue("action");
                List<Strategy> strategys = this.getStrategy(actions.get(i));

                if (strategys != null) per.addStrages(action, strategys);
            }
        }
        this.addModule(className, per);

    }

    @SuppressWarnings("unchecked")
    private List<Strategy> getStrategy(Element action) throws Exception {
        if (action == null) return null;
        List<Element> ls = action.elements();
        if (ls == null) return null;
        List<Strategy> strategys = new ArrayList<Strategy>();
        for (int i = 0; i < ls.size(); i++) {
            String className = ls.get(i).getTextTrim();
            Strategy st = (Strategy) Class.forName(className).newInstance();
            st.setClassName(className);

            strategys.add(st);

        }
        return strategys;

    }

    private void addModule(String className, Permission permission) {
        this.permissions.put(className, permission);
    }

    /**
     * 通过 module 名称获取权限对象以便于做权限检查
     * 
     * @param module 定义于
     *        com.xiaonei.platform.core.opt.permission.define.Module
     * @return Permisson
     */
    public Permission getPermissionByModule(String module) {

        return permissions.get(module);
    }

    public static void main(String args[]) throws Exception {
        //PermissionFactory pfc = PermissionFactory.getInstance();
        // pfc.init("e:/trunk"+"/WEB-INF/conf/strategy/stragegy.xml") ;
        // int id,int userId,int stage,int net,int auth,int status,int control
        // Status guester = new Status(1,2,20,5,1,0,3);
        // Status source = new Status(1,3,10,5,1,0,3);
        // System.out.println(pfc.getPermissionByModule(Module.MODULE_SPACE).hasSelect(guester,
        // source)) ;

    }

}
