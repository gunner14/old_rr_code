package com.xiaonei.reg.register.logic.additional;

import java.io.File;
import java.io.FileInputStream;
import java.io.StringWriter;
import java.sql.SQLException;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.lang.StringUtils;
import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.msg.home.MessageHome;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.usercount.UserCountMgr;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.IntroducerLogDAO;
import com.xiaonei.reg.register.model.IntroducerLog;
import com.xiaonei.reg.usertrace.q.UTUtils;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

/**
 * 注册邀请时候的站内信
 *
 * @author wei.cheng
 */
public class RegMsgLogic {
    
    private static RegMsgLogic _instance = new RegMsgLogic();
    
    private static final boolean is_template_letter = true;
    private static final int log_type = 2;
    private static final int log_type_debug = 1;
    private static final int log_type_error = 3;
    /**
     * mail template conf path
     */
    private static final String REG_MAIL_CONF_PATH = "/opt/www/WEB-INF/conf/welcome_letter";
    //private static final String REG_MAIL_CONF_PATH = "E:/xiaonei/xiaonei_project/xiaonei_reg/trunk/src/main/webapp/WEB-INF/conf/welcome_letter";

    /**
     *mail template conf name
     */
    private static final String REG_MAIL_CONF_NAME = "templateConfig_message.xml";

    /**
     * velocity conf name
     */
    private static final String VELOCITY_CONF_NAME = "velocity.properties";

    /**
     * velocity engine
     */
    private static volatile VelocityEngine ve = null;

    /**
     * storage for mail body template files
     */
    public static volatile Map<String, Template> templateMap;

    public static RegMsgLogic getInstance() {
        return _instance;
    }

    private static final String DEFAULT_FLOW = "100";

    private RegMsgLogic() {
        if(is_template_letter){
            try {
                init();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * init RegMsgLogic
     * @throws Exception
     */
    public static void init() throws Exception{
        pr("init begin", log_type_debug);
        if (ve == null){
            ve = new VelocityEngine();
        }
        Properties pp = new Properties();
        FileInputStream fs = null;
        try {
            fs = new FileInputStream(REG_MAIL_CONF_PATH + File.separator + VELOCITY_CONF_NAME);
            pp.load(fs);
        } catch (RuntimeException e) {
            e.printStackTrace();
        }finally{
            if(fs!=null)
                fs.close();
        }
        ve.init(pp);
        if(templateMap == null){
        	templateMap = new ConcurrentHashMap<String, Template>();
        }
    	fillConfMap();
        pr("init end", log_type_debug);
    }
    
    /**
     * initialise hash map
     * if vm is not exist, then give a message
     * @throws Exception
     */
    private static void fillConfMap() throws Exception{
        SAXReader saxReader = new SAXReader();
        Document confDoc = saxReader.read(new File(REG_MAIL_CONF_PATH
                + File.separator + REG_MAIL_CONF_NAME));

        Element rootElement = confDoc.getRootElement();
        Element item = null;
        String emailType = null;
        String emailVM = null;

        for (Iterator<?> i = rootElement.elementIterator("category"); i
                .hasNext();){
            item = (Element) i.next();
            emailType = item.attributeValue("id");
            emailVM = item.attributeValue("value");
            //判断文件是否存在
            if(new File(REG_MAIL_CONF_PATH + File.separator + emailVM).exists()){
                Template t = ve.getTemplate(emailVM);
                templateMap.put(emailType, t);
                pr(REG_MAIL_CONF_PATH + File.separator + emailVM + " file not exist", log_type_debug);
            }else{
                pr(REG_MAIL_CONF_PATH + File.separator + emailVM + " file not exist", log_type_error);
            }
        }
    }
    
    /**
     * get template contents
     * @param mailType
     * @param paramsMap
     * @return
     * @throws Exception
     */
    private String getMailBody(String mailType, Map<String, String> paramsMap) throws Exception {
        Template t = templateMap.get(mailType);
        VelocityContext context = new VelocityContext();
        parseMailBody(paramsMap, context);
        StringWriter writer = new StringWriter();
        if(t != null){
            for(java.util.Map.Entry<String, Template> b : templateMap.entrySet()){
                pr("[weige msg letter]" + b.getKey() + b.getValue(), log_type);
            }
            t.merge(context, writer);
        }
        return writer.toString();
    }
    
    /**
     * parse mail body message
     * @param paramsMap
     * @param Context
     */
    private void parseMailBody(Map<String, String> paramsMap,
            VelocityContext Context){
        if (paramsMap == null)
            return;
        for(java.util.Map.Entry<String, String> b : paramsMap.entrySet()){
            Context.put(b.getKey(), b.getValue());
        }        
        return;
    }    
    
    private static void pr(String str, int type){
        RegLogger logger = RegLogger.getLoggerWithType(RegMsgLogic.class.getSimpleName(), UTUtils.authorEmail);
        switch (type){
            case 1:
                logger.debug(str);
                break;
            case 2:
                logger.info(str);
                break;
            case 3:
                logger.error(str);
                break;
            default:
                System.out.println(str);
                break;
        }
    }
    
    public static void sendInviteMessage(User user) throws Exception {
        UserCountMgr.getCount(user.getId(), UserCountMgr.IM_LEAVE_WORD);
        IntroducerLog intro = IntroducerLogDAO.getInstance().getByNewbie(user.getId());
        if (intro != null) {
            IntroducerLogDAO.getInstance().updateTime(intro.getId(), new Date());
            User introducer = UserAdapter.getUser(intro.getIntroducer());
            if (introducer != null) {
                StringBuffer buf = new StringBuffer();
                buf.append("<a href=\""+Globals.urlWww+"/profile.do?id=");
                buf.append(user.getId());
                buf.append("\">");
                buf.append(user.getName());
                buf.append("</a>");
                buf.append("已经通过你的邀请链接成功注册成为人人网用户了。快去打个招呼，把他加为好友吧。\n");
                MessageHome.getInstance().sendMessage(SnsAdapterFactory.getUserBasicAdapter().get(introducer.getId()), 
                        SnsAdapterFactory.getUserBasicAdapter().get(user.getId()), "【人人网通知】" + user.getName() + "已经成功注册", buf
                        .toString());
                ;
            }
        }
    }
    
    public void sendWelcomeMessageNew(User user) throws Exception {
        HashMap<String, String> paraMap = new HashMap<String, String>();
        paraMap.put("user_name", user.getName());
        paraMap.put("user_id", String.valueOf(user.getId()));
        String flow = DodoyoResource.getString("renren.reg.message.letter.tpl.type", "check-rules");
        if(StringUtils.isEmpty(flow)) flow = DEFAULT_FLOW;
        String body = getMailBody(flow, paraMap);
        pr(body, log_type_debug);
        
        int admin = 223158817;
        try {
            UserCountMgr.incCount(user.getId(), UserCountMgr.NEW_MESSAGE, 0);
        } catch (Exception e) {
            System.err.print("init msg count failure, time out");
        }
        MessageHome.getInstance().sendMessage(SnsAdapterFactory.getUserBasicAdapter().get(user.getId())
                , SnsAdapterFactory.getUserBasicAdapter().get(admin), "欢迎来到人人网，这篇是使用指南，提问必读", body
                , "人人网是真实的社交网络，加入它你可以轻松愉快的和朋友保持联系");
        
    }
    
    public void sendWelcomeMessage(User user) throws SQLException {
        StringBuffer buf = new StringBuffer();

        buf.append("欢迎来到人人网 ^_^\n");
        buf.append("人人网是一个真实网络，非常重视对用户隐私权的保护。在这里你可以创建个性空间，发表日志，上传照片，分享好友新鲜事。\n");
        buf.append("\n");
        buf.append("这篇是使用指南，请仔细阅读，谢谢！\n");
        buf.append("\n");
        buf.append("1，填写真实资料\n");
        buf.append("为了方便朋友找到你，请使用真实姓名、真实头像，填写真实信息。\n");
        buf.append("资料真实就会成为<a href=\""+Globals.urlWww+"/notselectuser.do?action=no\" target=\"_blank\">星级用户</a>。\n");
        buf.append("\n");
        buf.append("2，找老同学、朋友：\n");
        buf.append("你可以点击个人空间上的大学、院系、高中、家乡等链接搜索到同校、同系、同学、同乡。\n");
        buf.append("也可以按姓名、公司名称、大学、中学、小学信息、入学年份，email等条件进行精确搜索。\n");
        buf.append("\n");
        buf.append("3，隐私保护：\n");
        buf
                .append("为保护用户的隐私，人人网提供较为全面的隐私设置来保证你的信息安全。进右上角的“帐号”--“<a href=\""+Globals.urlWww+"/privacyhome.do\" target=\"_blank\">隐私设置</a>”设置你的信息开放度。\n");
        buf
                .append("如果不喜欢某人就把他加入“<a href=\""+Globals.urlMain+"/info/Help.do#help2_15\" target=\"_blank\">黑名单</a>”吧 ^_^。\n");
        buf.append("\n");
        buf.append("4，加好友：\n");
        buf.append("在对方页面上点击“加为好友”，等待对方确认，对方同意后系统会自动以对方名义给你发一封站内信。\n");
        buf.append("\n");
        buf
                .append("更多问题请查看<a href=\""+Globals.urlMain+"/info/Help.do\" target=\"_blank\">帮助</a>。\n");
        buf.append("\n");
        buf.append("人人网 "+Globals.urlWww+"\n");
        buf.append("\n");
        buf.append("——人人网是你一辈子跟朋友保持沟通的桥梁。\n");

        int admin = 223158817;
        try {
            UserCountMgr.incCount(user.getId(), UserCountMgr.NEW_MESSAGE, 0);
        } catch (Exception e) {
            System.err.print("init msg count failure, time out");
        }
        try {
			MessageHome.getInstance().sendMessage(SnsAdapterFactory.getUserBasicAdapter().get(user.getId())
			        , SnsAdapterFactory.getUserBasicAdapter().get(admin), "欢迎来到人人网，这篇是使用指南，提问必读", buf.toString());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

    public void sendWelcomeMessageHs(User user) throws SQLException {
        StringBuffer buf = new StringBuffer();

        buf.append("欢迎来到人人网 ^_^ \n");
        buf.append("人人网是一个真实的网络，在这里你可以找到同学朋友、创建个性主页、发日志、传照片、玩游戏和分享好玩的内容。\n");
        buf.append("\n");
        buf.append("这篇是使用指南，请仔细阅读，谢谢！\n");
        buf.append("\n");
        buf.append("1. 填写真实资料\n");
        buf
                .append("为了方便朋友找到你，请使用真实姓名、真实头像，填写真实信息。资料真实会成为<a href=\""+Globals.urlMain+"/info/Help.do#help1_15\" target=\"_blank\">星级用户</a>，能更好的使用我们提供的各项服务。\n");
        buf.append("\n");
        buf.append("2. 找老同学、朋友:\n");
        buf
                .append("你可以在个人首页右上角的搜索框输入姓名、学校等信息<a href=\""+Globals.urlMain+"/info/Help.do#help1_0\" target=\"_blank\">搜索</a>同学朋友，也可以通过点击个人主页上的学校、家乡等信息查找同校、同地区好友。\n");
        buf.append("\n");
        buf.append("3. 加好友:\n");
        buf.append("在对方页面上点击个人图像下方的“加为好友”按钮，对方确认后你们就可以成为好友了。\n");
        buf.append("\n");
        buf.append("4. 装扮主页:\n");
        buf
                .append("点击你的个人图像下面的的“<a href=\""+Globals.urlMain+"/commonPreviewTemplate.do\" target=\"_blank\">装扮主页</a>”，使用免费模板美化你的个人主页。\n");
        buf.append("\n");
        buf.append("5. 手机人人网\n");
        buf.append("你可以通过电脑和手机两种方式登录人人网，手机人人网的网址是"+Globals.urlMobile+"；手机登录人人网完全免费，只需要支付手机上网流量费用，建议使用手机包月套餐。\n");
        buf.append("\n");
        buf.append("6. 邀请好友:\n");
        buf
                .append("人人网是一个跟同学朋友保持联系一起玩的地方，喜欢就<a href=\""+Globals.urlFriend+"/Invite.do\" target=\"_blank\">邀请</a>他们一起加入吧！\n");
        buf.append("\n");
        buf.append("7. 隐私保护:\n");
        buf
                .append("为保护用户的隐私，人人网提供较为全面的隐私设置来保证你的信息安全。进右上角的“设置”--“<a href=\""+Globals.urlMain+"/privacyhome.do\" target=\"_blank\">隐私设置</a>”设置你的信息开放度。\n");
        buf.append("\n");
        buf.append("更多问题请查看帮助，或到客服中心给管理员留言，我们会尽快回复你。\n");
        buf.append("\n");
        buf.append("人人网 "+Globals.urlWww+"\n");
        buf.append("——人人网是你一辈子跟朋友保持沟通的桥梁。\n");

        int admin = 230229770;
        //User adm = UserAdapter.getUser(admin);
        try {
            UserCountMgr.incCount(user.getId(), UserCountMgr.NEW_MESSAGE, 0);
        } catch (Exception e) {
            System.err.print("init msg count failure, time out");
        }
        //SendMessageLogic.sendMessage(user.getId(), adm, "欢迎来到人人网，这篇是使用指南，提问必读", buf.toString());
        try {
			MessageHome.getInstance().sendMessage(SnsAdapterFactory.getUserBasicAdapter().get(user.getId())
			        , SnsAdapterFactory.getUserBasicAdapter().get(admin), "欢迎来到人人网，这篇是使用指南，提问必读", buf.toString());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
}
