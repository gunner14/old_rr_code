<%@page contentType="text/html;charset=UTF-8" session="false"%>


<%@page import="com.xiaonei.platform.core.usercount.UserCountMgr"%>
<%@page import="java.util.Map"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Properties"%>
<%@page import="java.io.IOException"%>
<%@page import="com.xiaonei.platform.core.opt.OpiConstants"%>
<%@page import="java.util.List"%>
<%@page import="java.util.ArrayList"%>
<%@page import="java.util.Map.Entry"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheWithTotalN"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.model.SnsBuddyApplyCacheDataN"%>
<%@page import="com.xiaonei.platform.core.model.WUserCache"%>
<%@page import="com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory"%><div></div>
<h2>Request Info</h2>
<!-- sub_requestinfo -->

<%
//User host;
if(!"true".equals(request.getParameter("run"))){
}
else if(host!=null){
%>

<!-- {------------ --> 

<ul>
    <%
    %>
    <%
    class UserCount {
        int id;
        int count; 
        Integer order;
        String link;

        String word;

        String style;

        public UserCount(int id, int count, String link, String word, String style,Integer order) {
            this.id = id;
            this.count = count;
            this.link = link;
            this.word = word;
            this.style = style;
            this.order=order;
        }

        public int getId() {
            return id;
        }

        public int getCount() {
            return count;
        }
        
        public Integer getOrder() {
            return order;
        }
        
        public void setOrder(Integer order) {
            this.order = order;
        }

        public String getLink() {
            return link;
        }

        public String getWord() {
            return word;
        }

        public String getStyle() {
            return style;
        }

        @Override
        public String toString() {
            return "UserCount [count=" + count + ", id=" + id + ", link=" + link + ", style=" + style + ", word=" + word + ", order=" + order + "]";
        }

    }


    int userId = host.getId();
    Map<Integer, Integer> uc = UserCountMgr.getCountMap(userId);
    /*linkMap*/
    Map<Integer,String> linkMap = new HashMap<Integer,String>();
    try {
        final Properties p = new Properties();
        p.load(this.getClass().getClassLoader().getResourceAsStream("usercount.properties"));
        for (Object k : p.keySet()) {
            String key = (String) k;
            if (key.startsWith("link-")) {
                String value = p.getProperty(key);                    
                try {
                    linkMap.put(Integer.parseInt(key.substring("link-".length())), value);
                } catch (NumberFormatException e) {
                    e.printStackTrace();
                }
            }
        }
    } catch (IOException e) {
        e.printStackTrace();
    }
    for (Map.Entry<Integer,String> entry : linkMap.entrySet()) {
        entry.setValue(String.format(entry.getValue(),OpiConstants.domain.toString()));
    }
    /*linkMap*/
    /*wordMap*/
    Map<Integer, String> wordMap = new HashMap<Integer,String>();
    if (wordMap == null || wordMap.size() == 0) {    
        final Properties p = new Properties();
        p.load(this.getClass().getClassLoader().getResourceAsStream("usercount.properties"));
            for (Object k : p.keySet()) {
                String key = (String) k;
                if (key.startsWith("word-")) {
                    String value = p.getProperty(key);                    
                    try {
                        wordMap.put(Integer.parseInt(key.substring("word-".length())), value);
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
            }

      
    }

    /*wordMap*/
    /*styleMap*/
    Map<Integer, String> styleMap = new HashMap<Integer,String>();
    if (styleMap == null || styleMap.size() == 0) {
        try {
            final Properties p = new Properties();
            p.load(this.getClass().getClassLoader().getResourceAsStream("usercount.properties"));
            for (Object k : p.keySet()) {
                String key = (String) k;
                if (key.startsWith("style-")) {
                    String value = p.getProperty(key);                    
                    try {
                        styleMap.put(Integer.parseInt(key.substring("style-".length())), value);
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    /*styleMap*/
    /*orderMap*/
    Map<Integer, String> orderMap = new HashMap<Integer,String>();
    if (orderMap == null || orderMap.size() == 0) {
        try {
            final Properties p = new Properties();
            p.load(this.getClass().getClassLoader().getResourceAsStream("usercount.properties"));
            for (Object k : p.keySet()) {
                String key = (String) k;
                if (key.startsWith("order-")) {
                    String value = p.getProperty(key);                    
                    try {
                        orderMap.put(Integer.parseInt(key.substring("order-".length())), value);
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    /*orderMap*/
    if (uc != null) {
        List<UserCount> list = new ArrayList<UserCount>();
        for (Map.Entry<Integer, Integer> u : uc.entrySet()) {
            if (linkMap.get(u.getKey()) == null || wordMap.get(u.getKey()) == null || 
                    styleMap.get(u.getKey()) == null || orderMap.get(u.getKey())==null || u.getValue() == 0) 
                continue;
            try {
                UserCount ucount = new UserCount(u.getKey(), u.getValue(), linkMap.get(u.getKey()),wordMap.get(u.getKey()), styleMap.get(u.getKey()),Integer.parseInt(orderMap.get(u.getKey())));                    
                list.add(ucount); 
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }  
        if(list==null || list.size()==0){
            out.println("<li>目前无任何请求</li>");
        }
        else{
        	out.println("<li>");
        	for(int i=0;i<list.size();i++){
                out.println("Req_id:"+list.get(i).getId()+" -> "+list.get(i).getCount()+list.get(i).getWord()+" <br />");
            }
        	out.println("</li>");
        }
    }
    
    if(true){
	    SnsBuddyApplyCacheWithTotalN buddyApplyCacheWithTotal = (SnsBuddyApplyCacheWithTotalN) SnsAdapterFactory.getBuddyApplyCacheAdapterN().getBuddyApplyCacheWithTotal(userId, 0, -1);
	    if ((buddyApplyCacheWithTotal != null) && !buddyApplyCacheWithTotal.getBuddyApplyCacheN().isEmpty() && (buddyApplyCacheWithTotal.getTotalCount() != 0)) {
	        List<SnsBuddyApplyCacheDataN> lst = buddyApplyCacheWithTotal.getBuddyApplyCacheN();// 取得申请
	        List<Integer> idList1 = new ArrayList<Integer>();
	        for (SnsBuddyApplyCacheDataN apply : lst) {
	            idList1.add(apply.getApplicant());
	        }
	        Map<Integer, WUserCache> mapUserCache = SnsAdapterFactory.getUserCacheAdapter().getUserCacheMap(idList1);
	        out.println("<li>");
	        for   (int i=0;i<idList1.size();i++){    
	            WUserCache wuc=mapUserCache.get(idList1.get(i));
	            if(wuc!=null){
	                out.println("From:"+wuc.getId()+"["+wuc.getName()+"] <br />");   
	            }         
	        }  
	        out.println("</li>");
	    }
    }
    %>
    <table>
        <tr><td>
    
        </td></tr>
    <%
    %>
    </table>
</ul>
<!-- {------------ --> 
<%
}
%>