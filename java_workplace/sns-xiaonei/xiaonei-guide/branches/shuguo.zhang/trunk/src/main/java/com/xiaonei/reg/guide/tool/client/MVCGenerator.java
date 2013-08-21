package com.xiaonei.reg.guide.tool.client;

import java.io.IOException;
import java.util.List;

import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.tool.generator.IGenerator;
import com.xiaonei.reg.guide.tool.generator.impl.SimpleGenerator;
import com.xiaonei.reg.guide.tool.model.Datacolumn;
import com.xiaonei.reg.guide.tool.model.Datatable;
import com.xiaonei.reg.guide.tool.parser.IParser;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatacolumnItem;
import com.xiaonei.reg.guide.tool.parser.composite.ChoosedDatacolumnItemList;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatacolumnException;
import com.xiaonei.reg.guide.tool.parser.exeption.NoSuchDatatableException;
import com.xiaonei.reg.guide.tool.parser.impl.FileParser;
import com.xiaonei.reg.guide.tool.parser.impl.TestParser;


/**
 * MVCGenerator.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 新建文件
 */
public class MVCGenerator {

    private static String getType(String colname,Datatable table){
        String ret = "";
        List<Datacolumn> cols = table.getDatacolumns();
        for (Datacolumn c : cols) {
            if(c.getName().equals(colname)){
                ret = c.getType();
            }
                
        }
        return ret;
    }
    public static void main(String[] args) throws IOException {
        IParser parser = new FileParser("db.txt");
        Datatable tb = parser.parse();
        System.out.println("tb is:"+tb);
        IGenerator gen = new SimpleGenerator();
        String g = "";
        /*
        g = gen.generate(tb, IGenerator.TYPE_MODEL);
        System.out.println(IGenerator.TYPE_MODEL+"===============>>>");
        System.out.println(g);
        */
        
        g = gen.generate(tb, IGenerator.TYPE_DAO);
        System.out.println(IGenerator.TYPE_DAO+"===============>>>");
        System.out.println(g);
        /*
        g = gen.generate(tb, IGenerator.TYPE_LOGIC);
        System.out.println(IGenerator.TYPE_LOGIC+"===============>>>");
        System.out.println(g);
        
        g = gen.generate(tb, IGenerator.TYPE_JSP);
        System.out.println(IGenerator.TYPE_JSP+"===============>>>");
        System.out.println(g);
        
        //g = gen.generate(tb, IGenerator.TYPE_CONTROLLER_EXTRACT_MODEL);
        //System.out.println(IGenerator.TYPE_CONTROLLER_EXTRACT_MODEL+"===============>>> TYPE_CONTROLLER_EXTRACT_MODEL");
        System.out.println(g);
        */
        
        genSQL(tb);
    }
    private static void genSQL(Datatable tb) {
        TestParser.the_dt = tb;
        //TestParser.sql = "select id,ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark,insert_time,update_time from mlink_reg_to_game_map where ss_id=? and rt_id=? ~";
        //TestParser.sql = "select ss_id,rt_id,x_id,y_id,game_stat_id,code_id,to_url,remark from mlink_reg_to_game_map ";
        TestParser.sql = "UPDATE user set user_name = ?,remote_id = ?,coin_current = ?,coin_history = ?, coin_is_locked = ?,coin_locked_time = ?,coin_data = ?,head_url = ?,from_site = ? where remote_id = ? ~";
        try {
            System.out.println("Parsing sql");
            TestParser.doParser();
            ChoosedDatacolumnItemList cols = TestParser.columns;
            int i = 1;
            for (ChoosedDatacolumnItem col : cols.getItemlist()) {
                //System.out.println(col.getText()+":"+getType(col.getDatacolumn().getName(), tb));
                System.out.println("ps.set"+MyStringUtils.headToUpper(getType(col.getDatacolumn().getName(), tb))+"("+i+++",model.get"+MyStringUtils.headToUpper(col.getText())+"());");
            }
        } catch (NoSuchDatatableException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (NoSuchDatacolumnException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
