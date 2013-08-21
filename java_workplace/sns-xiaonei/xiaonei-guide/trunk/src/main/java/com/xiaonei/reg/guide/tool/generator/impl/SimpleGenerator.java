package com.xiaonei.reg.guide.tool.generator.impl;

import java.util.Iterator;
import java.util.List;

import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.tool.generator.IGenerator;
import com.xiaonei.reg.guide.tool.model.Datacolumn;
import com.xiaonei.reg.guide.tool.model.Datatable;


/**
 * SimpleGenerator.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 创建文件
 */
public class SimpleGenerator implements IGenerator {

    @Override
    public String generate(Datatable datatable, int gentype) {
        switch(gentype){
            case IGenerator.TYPE_DAO:
                return genDao(datatable);
            case IGenerator.TYPE_LOGIC:
                return genLogic(datatable);
            case IGenerator.TYPE_MODEL:
                return genModel(datatable);
            case IGenerator.TYPE_JSP:
                return genJSP(datatable);
            case IGenerator.TYPE_CONTROLLER_EXTRACT_MODEL:
                return genInvToModel(datatable);
        }
        return "";
    }
    /*
     * 
     * 
     * <table width="100%">
        <tr class="td2">
        <td>
            ID
        </td>
        <td>
            channel(渠道)
        </td>
        <td>
            shop(站长)
        </td>
        <td>
            ad(广告)
        </td>
        <td>
            flow(流程)
        </td>
        <td>
            jumpurl(跳转)
        </td>
        <td>
            memo(备注)
        </td>
        <td>
            oper(操作)
        </td>
    </tr>
    
    <c:set var="listcount" value="${fn:length(jumplinklist)}"/>
    <c:forEach var="item" items="${jumplinklist}" begin="0" step="1" varStatus="i">
    <tr class="td${i.index mod 2 + 3}">
        <td>
            ${item.id }
        </td>
        <td>
            ${item.channel}
        </td>
        <td>
            ${item.shop}
        </td>
        <td>
            ${item.ad}
        </td>
        <td>
            ${item.flow}
        </td>
        <td>
            ${item.jumpurl}
        </td>
        <td>
            <% if (JumpUtils.isKXDomain(request)) {%>
            ${item.memo} <a target="_blank" href="http://wwv.kaixin.com/kx.do?ss=${item.channel}&rt=${item.flow}&g=test_abc_234">go</a>
            <%}else{%>
            ${item.memo} <a target="_blank" href="http://wwv.renren.com/xn.do?ss=${item.channel}&rt=${item.flow}&g=test_abc_234">go</a>
            <%}%>
        </td>
        <td>
            <span style="cursor:pointer;color:blue;" onclick="_submit('set',${item.id })">set</span>
            <span style="cursor:pointer;color:red;" onclick="_submit('del',${item.id })">del</span>
        </td>
    </tr>
    </c:forEach>
    
    <tr class="td${(listcount) mod 2 + 3}">
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td>
            <span style="cursor:pointer;color:blue;" onclick="_submit('add',0)">add</span>
        </td>
    </tr>
</table>

     */
    
    /*
     * <c:if test="${item.id == theId}">
        <tr class="td${i.index mod 2 + 3}">
        <td>
         <input type="hidden" name="id" value="${item.id}"/>
            ${item.id}
        <br></td>
        <td>
            <input type="text" name="channel" value="${item.channel}"  class="smallInput"/><br/>
        <br></td>
        <td>
            <input type="text" name="shop" value="${item.shop}"  class="smallInput"/><br/>
        <br></td>
        <td>
            <input type="text" name="ad" value="${item.ad}"  class="smallInput"/><br/>
        <br></td>
        <td>
            <input type="text" name="flow" value="${item.flow}"  class="smallInput"/><br/>
        <br></td>
        <td>
            <input type="text" name="jumpurl" value="${item.jumpurl}" class="smallInput" /><br/>
        <br></td>
        <td>
            <input type="text" name="memo" value="${item.memo}" class="smallInput" /><br/>
        <br></td>
        <td>
            <input type="submit" name="提交" value="提交"/><a href="/manage.do">cancel</a>
        <br></td>
    </tr>
            
    </c:if>
     * 
     * 
     * 
     * 
     * 
     */
    
    private String genJSP_Set(Datatable datatable){
        MyStringBuilder sb = new MyStringBuilder();
        sb.appendLine("<c:forEach var=\"item\" items=\"${modellist}\" begin=\"0\" step=\"1\" varStatus=\"i\">");
        sb.appendLine("<c:if test=\"${item.id == theId}\">");
        sb.appendLine(" <tr class=\"td${i.index mod 2 + 3}\">");
        sb.appendLine("   <td>");
        sb.appendLine("     <input type=\"hidden\" name=\"id\" value=\"${item.id}\"/>");
        sb.appendLine("     ${item.id}");
        sb.appendLine("     <br>");
        sb.appendLine("   </td>");
        for (Datacolumn col  : datatable.getDatacolumns()) {
            if(MyStringUtils.equalsIgnoreCase("id", col.getName())){
                continue;
            }
            sb.appendLine(" <td>");
            sb.appendLine("     <input type=\"text\" name=\""+col.getName()+"\" value=\"${item."+col.getName()+"}\"  class=\"smallInput\"/><br/>");
            sb.appendLine(" </td>");
        }
        sb.appendLine(" <td>");
        sb.appendLine("     <input type=\"submit\" name=\"提交\" value=\"提交\"/><a href=\"/admin/jumpmanage\">cancel</a>");
        sb.appendLine(" </td>");
        sb.appendLine(" </tr>");
        sb.appendLine("</c:if>");
        
        
        sb.appendLine("<c:if test=\"${item.id != theId}\">");
        sb.appendLine("  <tr class=\"td${i.index mod 2 + 3}\">");
        for (Datacolumn col  : datatable.getDatacolumns()) {
            sb.appendLine(" <td>");
            sb.appendLine("     ${item."+col.getName()+"}");
            sb.appendLine(" </td>");
        }
        sb.appendLine("  </tr>");
        sb.appendLine("</c:if>");
        sb.appendLine("</c:forEach>");
        return sb.toString();
    }
    private String genJSP(Datatable datatable) {
        MyStringBuilder sb = new MyStringBuilder();
        sb.appendLine("<table width=\"100%\">");
        sb.appendLine(" <tr class=\"td2\">");
        for (Datacolumn col  : datatable.getDatacolumns()) {
            sb.appendLine(" <td>");
            sb.appendLine("     "+col.getName());
            sb.appendLine(" </td>");
        }
        
        sb.appendLine(" <c:set var=\"listcount\" value=\"${fn:length(modellist)}\"/>");
        sb.appendLine(" <c:forEach var=\"item\" items=\"${modellist}\" begin=\"0\" step=\"1\" varStatus=\"i\">");
        sb.appendLine(" <tr class=\"td${i.index mod 2 + 3}\">");
        
        for (Datacolumn col  : datatable.getDatacolumns()) {
            sb.appendLine("  <td>");
            sb.appendLine("     ${item."+col.getName()+"}");
            sb.appendLine("  </td>");
        }
        sb.appendLine("  <td>");
        sb.appendLine("  <span style=\"cursor:pointer;color:blue;\" onclick=\"_submit('set',${item.id })\">set</span>");
        sb.appendLine("  <span style=\"cursor:pointer;color:red;\" onclick=\"_submit('del',${item.id })\">del</span>");
        sb.appendLine("  </td>");
        sb.appendLine(" </tr>");
        
        sb.appendLine(" </c:forEach>");
        sb.appendLine(" <tr class=\"td${(listcount) mod 2 + 3}\">");
        for (Datacolumn col  : datatable.getDatacolumns()) {
            sb.appendLine(" <td><!--"+col.getName()+"--></td>");
        }
        sb.appendLine(" <td><span style=\"cursor:pointer;color:blue;\" onclick=\"_submit('add',0)\">add</span></td>");
        sb.appendLine(" </tr>");
        sb.appendLine("</table>");
        return sb.toString()+"=================>>>> set():\n"+genJSP_Set(datatable);
    }

    private String genModel(Datatable datatable) {
        StringBuilder ret = new StringBuilder();
        
        ret.append("public class "+genModelName(datatable.getName())+"{ \n");
        List<Datacolumn> cols = datatable.getDatacolumns();
        for (Datacolumn col : cols) {
            ret.append("private ");
            ret.append(col.getType());
            ret.append(" ");
            ret.append(col.getName());
            ret.append(";\n");
        }
        ret.append("}");
        return ret.toString();
    }

    private String genLogic(Datatable datatable) {
        StringBuilder ret = new StringBuilder();
        ret.append("public class "+genLogicName(datatable.getName())+"{ \n");
        ret.append("    public static "+genLogicName(datatable.getName())+" getInstance() { return instance; } \n");
        ret.append("    private "+genLogicName(datatable.getName())+"() { super(); } \n");
        ret.append("    private static "+genLogicName(datatable.getName())+" instance = new "+genLogicName(datatable.getName())+"(); \n");
        ret.append("} \n");
        return ret.toString();
    }

    private String genLogicName(String tname) {
        StringBuilder ret = new StringBuilder();
        String[] words = tname.split("_");
        for (String w : words) {
            ret.append(MyStringUtils.headToUpper(w));
        }
        ret.append("Logic");
        return ret.toString();
    }

    private String genDao(Datatable datatable) {
        StringBuilder ret = new StringBuilder();
        ret.append("public class "+genDaoName(datatable.getName())+"{ \n");
        ret.append("    public static "+genDaoName(datatable.getName())+" getInstance() { return instance; } \n");
        ret.append("    private "+genDaoName(datatable.getName())+"() { super(); } \n");
        ret.append("    private static "+genDaoName(datatable.getName())+" instance = new "+genDaoName(datatable.getName())+"(); \n");
        ret.append("    private static final String TABLE_NAME = \""+datatable.getName()+"\"; \n");
        //gen parse method
        ret.append("    protected "+genModelName(datatable.getName())+" parseToModel(ResultSet rs) throws SQLException { \n");
        ret.append("        "+genModelName(datatable.getName())+" model = new "+genModelName(datatable.getName())+"(); \n");
        for (Iterator<Datacolumn> iterator = datatable.getDatacolumns().iterator(); iterator.hasNext();) {
            Datacolumn curcol = (Datacolumn) iterator.next();
            ret.append("        "+getSettor(curcol));
        }
        ret.append("    return model; \n");
        ret.append("    } \n");
        ret.append("} \n");
        return ret.toString();
    }

    private Object getSettor(Datacolumn curcol) {
        StringBuilder ret = new StringBuilder();
        ret.append("model.set");
        ret.append(MyStringUtils.headToUpper(curcol.getName()));
        ret.append("(rs.get"+MyStringUtils.headToUpper(curcol.getType())+"(");
        ret.append("\""+curcol.getName()+"\")); \n");
        return ret.toString();
    }

    private String genDaoName(String tname) {
        StringBuilder ret = new StringBuilder();
        String[] words = tname.split("_");
        for (String w : words) {
            ret.append(MyStringUtils.headToUpper(w));
        }
        ret.append("DAO");
        return ret.toString();
    }

    private String genModelName(String tname){
        StringBuilder ret = new StringBuilder();
        String[] words = tname.split("_");
        for (String w : words) {
            ret.append(MyStringUtils.headToUpper(w));
        }
        ret.append("Model");
        return ret.toString();
    }

    private String genInvToModel(Datatable datatable){
        
        StringBuilder ret = new StringBuilder();
        
        List<Datacolumn> cols = datatable.getDatacolumns();
        for (Datacolumn col : cols) {
            if(MyStringUtils.equalsIgnoreCase(col.getType(), "int")){
                ret.append("model.set"+MyStringUtils.headToUpper(col.getName())+"(MyStringUtils.stringToInt(inv.getParameter(\""+(col.getName())+"\")));"+"\n");
            }
            else{
                ret.append("model.set"+MyStringUtils.headToUpper(col.getName())+"(inv.getParameter(\""+(col.getName())+"\"));"+"\n");
            }
        }
        return ret.toString();
    }
}

class MyStringBuilder{
    private StringBuilder sb = new StringBuilder();
    public void appendLine(String s ){
        sb.append(s+"\n");
    }
    public String toString(){
        return sb.toString();
    }
}
