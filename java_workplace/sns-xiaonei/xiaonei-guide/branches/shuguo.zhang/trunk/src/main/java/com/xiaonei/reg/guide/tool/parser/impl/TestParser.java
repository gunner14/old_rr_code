package com.xiaonei.reg.guide.tool.parser.impl;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Stack;

import com.xiaonei.reg.guide.tool.model.Datacolumn;
import com.xiaonei.reg.guide.tool.model.Datatable;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatacolumnItem;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatacolumnItemList;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatatableItem;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatatableItemList;
import com.xiaonei.reg.guide.tool.parser.composite.DatatableJointItem;
import com.xiaonei.reg.guide.tool.parser.composite.JointConditionItem;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatacolumnException;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatatableException;
import com.xiaonei.reg.guide.tool.parser.sqlmodel.ISqlObject;
import com.xiaonei.reg.guide.tool.parser.sqlmodel.SqlBracket;
import com.xiaonei.reg.guide.tool.parser.sqlmodel.SqlExpress;
import com.xiaonei.reg.guide.tool.parser.sqlmodel.SqlOperation;



//Token类的定义
class Token{
    String content;
    Type type;
    String getText(){
        return "[ "+content+" , "+type+" ]";
    }
}

//Token的类型
enum Type{
    ERROR,      //错误Token，即不希望出现的
    KEY,        //关键字
    OP,         //操作符
    BOUND,      //边界符
    SPECIAL,    //特殊符
    SEP,        //分隔符
    WORD        //标识符
}

//解析器主体类
public class TestParser {
    
    //定义关键字
    private static String[] keywords = {
        "SELECT","INSERT","UPDATE","DELETE",
        "INTO",
        "DISTINCT",
        "FROM",
        "WHERE",
        "LEFT",
        "JOIN",
        "ON",
        "INNER",
        "OUTER",
        "VALUES",
        "SET",
        "GROUP",
        "ORDER",
        "BY",
        "HAVING",
        "AS"
        
    };
    
    //操作符定义
    private static String[] operators = {
        ">",
        "<",
        "=",
        ">=",
        "<=",
        "<>",
        "!=",
        "==",
        "LIKE",
        "AND",
        "OR",
        "NOT"
    };
    
    //边界符
    private static char[] bounds = {
        '(',
        ')',
        '\'',
        '\"',
        '[',
        ']'
    };
    
    //分隔符
    private static char[] deviedesymbols = {
        ',',
        ' ',
        '\n'
    };
    
    //特殊符
    private static char[] specails = {
        '~'
    };
    
    //分离符
    private static char[] seporators = {
        ',',
        ' ',
        '\n',
        '>',
        '<',
        '=',
        '(',
        ')',
        '[',
        ']',
        '!',
        '?'
    };
    
    //查找并返回下一个Token
    private static Token nextToken() {

        StringBuilder sb = new StringBuilder();
        Type type = Type.WORD;

        if (isSeprator()) {
            if (isDS()) {
                type = Type.SEP;
                sb.append(sql.charAt(curPos++));
            } else if (isBounds()) {
                sb.append(sql.charAt(curPos++));
                type = Type.BOUND;
            } else if (isSpecail()) {
                sb.append(sql.charAt(curPos++));
                type = Type.SPECIAL;
            } else {
                type = Type.OP;
                sb.append(sql.charAt(curPos++));
                String s = sb.toString();
                s += sql.charAt(curPos);
                if (isOperator(s)) {
                    sb.append(sql.charAt(curPos++));
                }
                else if(isOperator(sb.toString())){
                    
                }
                else {
                    type = Type.ERROR;
                }
            }

        }

        else {
            while (!isSeprator() /*&& !isBounds()*/) {
                sb.append(sql.charAt(curPos++));
            }
            String ss = sb.toString();
            if (isKeyword(ss)) {
                type = Type.KEY;
            } else if (isOperator(ss)) {
                type = Type.OP;
            } else {
                type = Type.WORD;
            }
        }
        Token tk = new Token();
        tk.content = sb.toString();
        tk.type = type;
        // System.out.println("found token:"+tk.content);
        return tk;
    }

    //判断入参值是否操作
    private static boolean isOperator(String string) {
        for (String s : operators) {
            if (s.equalsIgnoreCase(string)) {
                return true;
            }
        }
        return false;
    }
    
    //判断入参值是否关键字
    private static boolean isKeyword(String string){
        for (String s : keywords) {
            if (s.equalsIgnoreCase(string)) {
                return true;
            }
        }
        return false;
    }

    //判断当前字符是否分离符
    private static boolean isSeprator(){
        for(char s : seporators){
            if(s==sql.charAt(curPos)){
                return true;
            }
        }
        return false;
    }
    
    //判断当前字符是否分隔符
    private static boolean isDS() {
        for(char s : deviedesymbols){
            if(s == (sql.charAt(curPos))){
                return true;
            }
        }
        return false;
    }
    
    //判断当前字符是否边界符
    private static boolean isBounds(){
        for(char s : bounds){
            if(s == (sql.charAt(curPos))){
                return true;
            }
        }
        return false;
    }
    
    //判断当前字符是否特殊符
    private static boolean isSpecail(){
        for(char s : specails){
            if(s ==  sql.charAt(curPos)){
                return true;
            }
        }
        return false;
    }
    
    public static ArrayList<ISqlObject> whereexps = new ArrayList<ISqlObject>();
    public static ArrayList<ISqlObject> onexps = new ArrayList<ISqlObject>();
    public static ArrayList<ISqlObject> havingexps = new ArrayList<ISqlObject>();
    public static ChoosedDatatableItemList tables = new ChoosedDatatableItemList(); 
    public static ChoosedDatacolumnItemList columns = new ChoosedDatacolumnItemList();
    public static ChoosedDatacolumnItemList ordercolumns = new ChoosedDatacolumnItemList();
    public static ChoosedDatacolumnItemList groupcolumns = new ChoosedDatacolumnItemList();
    public static ArrayList<ChoosedDatatableItem> onitems = new ArrayList<ChoosedDatatableItem>();
    public static ArrayList<String> values = new ArrayList<String>();
    public static boolean distinct = false;
    public static String sqltype = "SELECT";
    //public static String sql = " select s.STUDENTID myid,name from student s join score b on s.studentid== b.studentid and s.id == b.id where (myid>=?) OR (((myid <= ?))) ~";
    public static String sql ="select s.STUDENTID myid,name from(classinfo LEFT JOIN (student s LEFT JOIN score b ON s.STUDENTID=b.STUDENTID and s.STUDENTID == b.STUDENTID) ON classinfo.CLASSID = b.CLASSID) where (s.STUDENTID = b.STUDENTID) ~";

    private static ArrayList<Token> ontokens = new ArrayList<Token>();
    private static int curPos = 0;
    
    //将Token转化为程序需要的数据模型
    private static void tokenToItem(ArrayList<Token> tokens) throws NoSuchDatatableException{
        String curKeyWord = "";
        int curID = 0;
        SqlBracket [] bracketstack = new SqlBracket [100];
        int curPnt = 0;
        for (int i = 0; i < tokens.toArray().length; i++) {
            
            Token token = tokens.get(i);
            if(token.type == Type.BOUND){
                SqlBracket b = new SqlBracket();
                b.content = token.content;
                if(token.content.equals("(") || token.content.equals("[")){
                    b.id = ++curID;
                    curPnt ++;
                    bracketstack[curPnt] = b;
                }
                else{
                    b.id = bracketstack[curPnt].id;
                    curPnt --;
                }
                
                if(curKeyWord.equalsIgnoreCase("WHERE")){
                    whereexps.add(b);
                }
                else{
                    onexps.add(b);
                }
//              else{
//                  MyLog.writeLine("Parser Test -- tokenToItem_SELECT() -- if(token.type == Type.BOUND)");
//              }
                //whereexps.add(b);
            }
            else if(token.type == Type.OP){
                if(curKeyWord.equalsIgnoreCase("WHERE")){
                    SqlOperation ope = new SqlOperation();
                    ope.setOperation(token.content);
                    whereexps.add(ope);
                }
                else/* if(curKeyWord.equalsIgnoreCase("FROM") || curKeyWord.equalsIgnoreCase("ON"))*/{
                    SqlOperation ope = new SqlOperation();
                    ope.setOperation(token.content);
                    onexps.add(ope);
                }
//              else{
//                  MyLog.writeLine("Parser Test -- tokenToItem_SELECT() -- if(token.type == Type.OP)");
//              }
            }
            else if(token.type ==  Type.WORD){
                if(curKeyWord.equalsIgnoreCase("SELECT") || curKeyWord.equalsIgnoreCase("GROUP") || curKeyWord.equalsIgnoreCase("ORDER")){
                    Datacolumn dcol = new Datacolumn();
                    dcol.setName(token.content);
                    ChoosedDatacolumnItem col = new ChoosedDatacolumnItem(dcol,"",null);
                    if(tokens.size()>=i+2 && tokens.get(i+1).type == Type.WORD){ //try to see next token...
                        col.setAliasname(tokens.get(i+1).content);
                        i++;
                    }
                    if(curKeyWord.equalsIgnoreCase("SELECT")){
                        columns.add(col);
                    }
                    if(curKeyWord.equalsIgnoreCase("GROUP")){
                        groupcolumns.add(col);
                    }
                    if(curKeyWord.equalsIgnoreCase("ORDER")){
                        ordercolumns.add(col);
                    }
                    
                }
                else if(curKeyWord.equalsIgnoreCase("WHERE") || curKeyWord.equalsIgnoreCase("ON") || curKeyWord.equalsIgnoreCase("HAVING") || curKeyWord.equalsIgnoreCase("SET")){
                    SqlExpress exp = new SqlExpress();
                    exp.setAttributename(token.content);
                    if(tokens.get(i+1).type == Type.OP){
                        i++;
                        exp.setOperation(tokens.get(i).content);
                        i++;
                        exp.setValue(tokens.get(i).content);
                    }
                    if(curKeyWord.equalsIgnoreCase("WHERE")){
                        whereexps.add(exp);
                    }
                    else if(curKeyWord.equalsIgnoreCase("ON") || curKeyWord.equalsIgnoreCase("SET")){
                        onexps.add(exp);
                    }
                    else if(curKeyWord.equalsIgnoreCase("HAVING")){
                        havingexps.add(exp);
                    }
                    else {
                        System.out.println("Parser Test -- tokenToItem_SELECT() -- if(curKeyWord.equalsIgnoreCase(\"WHERE\")");
                    }
                }
                else if(curKeyWord.equalsIgnoreCase("FROM") || curKeyWord.equalsIgnoreCase("INTO")||curKeyWord.equalsIgnoreCase("UPDATE")|| curKeyWord.equalsIgnoreCase("JOIN")|| curKeyWord.equalsIgnoreCase("INSERT")  ||curKeyWord.equalsIgnoreCase("DELETE")){
                    Datatable dtable = new Datatable();
                    dtable.setName(token.content);
                    //inshion
                    dtable = the_dt;
                    ChoosedDatatableItem dt = new ChoosedDatatableItem(dtable, "");
                    if(i+1<tokens.toArray().length){
                        if(tokens.get(i+1).type == Type.WORD){
                            dt.setAliasname(tokens.get(i+1).content);
                            i++;
                        }
                    }
                    dt.setContainer(tables);
                    tables.add(dt);
                    
                }
                else if(curKeyWord.equalsIgnoreCase("VALUES")){
                    values.add(token.content);
                }
            }
            else if(token.type == Type.SPECIAL){
                
            }
            else if(token.type == Type.KEY){
                if(token.content.equalsIgnoreCase("DISTINCT")){
                    distinct = true;
                }
                else if(!token.content.equalsIgnoreCase("BY")){
                    curKeyWord = token.content;
                }
            }
            
        }
        return;
    }
    
    //重置输入输出
    public static void reset(){
        curPos = 0;
        whereexps = new ArrayList<ISqlObject>();
        onexps = new ArrayList<ISqlObject>();
        tables = new ChoosedDatatableItemList(); 
        columns = new ChoosedDatacolumnItemList();
        ordercolumns = new ChoosedDatacolumnItemList();
        groupcolumns = new ChoosedDatacolumnItemList();
        ontokens = new ArrayList<Token>();
        onitems = new ArrayList<ChoosedDatatableItem>();
        values = new ArrayList<String>();
        havingexps = new ArrayList<ISqlObject>();
    }
    
    //进行解析
    public static void doParser() throws NoSuchDatatableException, NoSuchDatacolumnException{
        //sql = sql.toUpperCase();
        ArrayList<Token> lists = new ArrayList<Token>();
        //ArrayList<ISqlObject> sqls = new ArrayList<ISqlObject>();
        int len = sql.length();
        //StringBuilder curTokenBuider = new StringBuilder();
        while(curPos < len){
            if(sql.charAt(curPos) == '~')break;
            //System.out.println(nextToken());
            //curTokenBuider.append(sql.charAt(curPos));
            Token t = nextToken();
            if(t.type == Type.KEY){
                if(t.content.equalsIgnoreCase("SELECT")){
                    sqltype = "SELECT";
                }
                else if(t.content.equalsIgnoreCase("INSERT")){
                    sqltype = "INSERT";
                }
                else if(t.content.equalsIgnoreCase("UPDATE")){
                    sqltype = "UPDATE";
                }
                else if(t.content.equalsIgnoreCase("DELETE")){
                    sqltype = "DELETE";
                }
            }
            if(t.content.equals(" ") || ( t.type== Type.KEY)&& t.content.equalsIgnoreCase("AS"))continue;
            lists.add(t);
            System.out.println(t.getText());
        }
        
//      for(Token s: lists){
//          System.out.println(s.getText());
//      }
        
//      createJointTree("(classinfo LEFT JOIN (student s LEFT JOIN score b ON s.STUDENTID=b.STUDENTID and s.STUDENTID == b.STUDENTID) ON classinfo.CLASSID = b.CLASSID)");
        System.out.println("== token to item ing...==");
        tokenToItem(lists);
        //System.out.println("== updateColumnTable ing...==");
        updateColumnTable();
        
        //getOnTokens(lists);
        //onitems.add(createJointTree(ontokens));
        
        
      System.out.println("===========TABLES============");
      for(ChoosedDatatableItem i : tables.getItemlist()){
          System.out.println(i.getDatatable().getName()+":"+i.getAliasname()+":"+i.getColumns());
      }
      System.out.println("===========ON================");
      for(ISqlObject s : onexps){
          System.out.println(s.getText());
      }
      System.out.println("===========SELECT============");
      for(ChoosedDatacolumnItem i : columns.getItemlist()){
          String tname = i.getDatatableItem()==null?"null":i.getDatatableItem().getDatatable().getName();
          System.out.println(i.getDatacolumn().getName()+"["+i.getDatacolumn().getType()+"]"+":"+i.getAliasname()+" -> "+tname);
      }
      System.out.println("===========WHERE============");
      for(ISqlObject s : whereexps){
          System.out.println(s.getText());
      }
      

      System.out.println("===========ON TOKENS============");
      for(Token t : ontokens){
          System.out.println(t.getText());
      }
      System.out.println("==================================");
        
    }
    
    //分出On子句的Token
    @SuppressWarnings("unused")
    private static void getOnTokens(ArrayList<Token> alltokens) {
        for (Iterator<Token> iter = alltokens.iterator(); iter.hasNext();) {
            Token element = (Token) iter.next();
            if(element.content.equalsIgnoreCase("FROM") || element.content.equalsIgnoreCase("SET")){
                if(iter.hasNext()){
                    element = (Token) iter.next();
                    if(element.type==Type.SEP)continue;
                    while(!element.content.equalsIgnoreCase("WHERE")){
                        ontokens.add(element);
                        if(iter.hasNext()){
                            element = (Token) iter.next();
                        }
                        else{
                            break;
                        }
                    }
                }
            }
        }
    }

    //测试用入口
    public static void main(String[] args) throws NoSuchDatatableException, NoSuchDatacolumnException {
//      doParser();
        //sql = "Select * from (classinfo LEFT JOIN (student s LEFT JOIN score b ON s.STUDENTID = b.STUDENTID) ON classinfo.CLASSID = b.CLASSID)";
        sql = "select id,ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark,insert_time,update_time from mlink_reg_to_game_map where ss_id=? and rt_id=? ~";
        doParser();
        
        ArrayList<ChoosedDatacolumnItem> a = columns.getItemlist();
        for (ChoosedDatacolumnItem c : a) {
            System.out.println(c.getText()+" "+c.getDatacolumn().getType());
        }
        //createJointTree(sql);
//      sql = sql.toUpperCase();
//      ArrayList<Token> lists = new ArrayList<Token>();
//      //ArrayList<ISqlObject> sqls = new ArrayList<ISqlObject>();
//      int len = sql.length();
//      //StringBuilder curTokenBuider = new StringBuilder();
//      while(curPos < len){
//          if(sql.charAt(curPos) == '~')break;
//          //System.out.println(nextToken());
//          //curTokenBuider.append(sql.charAt(curPos));
//          Token t = nextToken();
//          if(t.content.equals(" "))continue;
//          lists.add(t);
//      }
//      
//      for(Token s: lists){
//          System.out.println(s.getText());
//      }
//      
//      tokenToItem_SELECT(lists);
//      updateColumnTable();
//      
//      System.out.println("=====================");
//      for(ChoosedDatatableItem i : tables.getItemlist()){
//          System.out.println(i.getDatatable().getName()+":"+i.getAliasname());
//      }
//      for(ChoosedDatacolumnItem i : columns.getItemlist()){
//          String tname = i.getDatatableItem()==null?"null":i.getDatatableItem().getDatatable().getName();
//          System.out.println(i.getDatacolumn().getName()+":"+i.getAliasname()+" -> "+tname);
//      }
//      for(ISqlObject s : whereexps){
//          System.out.println(s.getText());
//      }
        
    }

    public static Datatable the_dt = null; 
    
    //更新数据模型中列与表的引用关系
    private static void updateColumnTable() throws NoSuchDatatableException, NoSuchDatacolumnException {
        for(int k = 0; k<3; k++){
            ChoosedDatacolumnItemList curList = columns;
            if(k == 1){
                curList = ordercolumns;
            }
            if(k == 2){
                curList = groupcolumns;
            }
            for(ChoosedDatacolumnItem i : curList.getItemlist()){
                boolean found = false;
                for(ChoosedDatatableItem ti : tables.getItemlist()){
                    String colname = i.getDatacolumn().getName();
                    String tablealias = null;
                    String collast = null;
                    if(colname.indexOf('.')>=0){
                        tablealias = colname.substring(0,colname.indexOf('.'));
                        collast = colname.substring(colname.indexOf('.')+1, colname.length());
                        //System.out.println("tablealias is "+tablealias);
                    }
                    if(tablealias!=null){
                        if(tablealias.equalsIgnoreCase(ti.getAliasname()) || tablealias.equalsIgnoreCase(ti.getUsingName())){
                            for (ChoosedDatacolumnItem col : ti.getColumns()) {
                                if(col.getDatacolumn().getName().equalsIgnoreCase(collast)){
                                    i.setDatatable(ti);
                                    i.getDatacolumn().setName(colname.substring(colname.indexOf('.')+1));
                                    found = true;
                                }
                            }
                            if(found){
                                break;
                            }
                            else{
                                throw new NoSuchDatacolumnException(i.getFullName());
                            }
                        }
                    }
                    if(ti.getDatatable().getDatacolumns()==null){
                        System.out.println("TestParser::updateColumnTable() -- columns null so continues:"+ti.getDatatable().getName());
                        continue;
                    }
                    for(Datacolumn col: ti.getDatatable().getDatacolumns()){
                        if(colname.equalsIgnoreCase(col.getName())){
                            i.setDatatable(ti);
                            found = true;
                            break;
                        }
                    }
                }
                if(!found){
                    throw new NoSuchDatacolumnException(i.getFullName());
                }
            }
        }
    }
    
    //构造数据模型中的连接树
    @SuppressWarnings("null")
    public static ChoosedDatatableItem createJointTree(ArrayList<Token> tokenset) throws NoSuchDatatableException{
        Stack<Token> s = new Stack<Token>();
        Token lastJoin = null;
        Token lastOn = null;
        for (Token token : tokenset) {
            if(token.content.equalsIgnoreCase("JOIN")){
                s.push(token);
            }
            if(token.content.equalsIgnoreCase("ON")){
                lastOn = token;
                if(s.size()>0){
                    s.pop();
                }
            }
            if(s.size() == 1){
                lastJoin = s.peek();
            }
            
        }
        
        DatatableJointItem ret = new DatatableJointItem(null,null);
        if(lastJoin == null){
            for (Iterator<Token> iter = tokenset.iterator(); iter.hasNext();) {
                Token token = (Token) iter.next();
                if(token.type == Type.WORD){
                    ChoosedDatatableItem dt = null;
                    String tname = token.content;
                    String talia = null;
                    
                    if(iter.hasNext()){
                        token = (Token) iter.next();
                        if(token.type == Type.WORD){
                            //lt.setAliasname(token.content);
                            talia = token.content;
                        }
                    }
                    
                    for (ChoosedDatatableItem t : tables.getItemlist()) {
                        if(t.getDatatable().getName().equals(tname)){
                            if(talia!=null){
                                if(t.getAliasname().equalsIgnoreCase(talia)){
                                    dt = t;
                                }
                                else{
                                    continue;
                                }
                            }
                            else{
                                dt = t;
                            }
                        }
                    } 
                    //ChoosedDatatableItem lt = new ChoosedDatatableItem(dt, "");
                    return dt;
                }
            }
        }
        else{
            ArrayList<Token> list1 = new ArrayList<Token>();
            ArrayList<Token> list2 = new ArrayList<Token>();
            Token lasttoken = null;
            for (Iterator<Token> iter = tokenset.iterator(); iter.hasNext();) {
                Token token = (Token) iter.next();
                while(token!=lastJoin){
                    list1.add(token);
                    if(!iter.hasNext())break;
                    lasttoken = token;
                    token = (Token) iter.next();
                }
                if(lasttoken.type == Type.KEY){
                    ret.setJointype(lasttoken.content+" JOIN");
                }
                while(iter.hasNext()){
                    token = (Token) iter.next();
                    if(token == lastOn) break;
                    list2.add(token);
                }
                ArrayList<JointConditionItem> conds = new ArrayList<JointConditionItem>();
                while(iter.hasNext()){
                    
                    while(iter.hasNext()){
                        JointConditionItem conditem = new JointConditionItem();
                        Token ct = (Token) iter.next();
                        if(ct.type != Type.WORD) continue;
                        conditem.colname1 = ct.content;
                        conditem.operation = ((Token) iter.next()).content;
                        conditem.colname2 = ((Token) iter.next()).content;
                        conds.add(conditem);
                    }
                    
                }
                ret.setConditions(conds);
                break;
            }
            ret.setTable1(createJointTree(list1));
            ret.setTable2(createJointTree(list2));
        }
        
        
        
        return ret;
    }
    

}
