package com.xiaonei.reg.guide.tool.parser.impl;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.tool.model.Datacolumn;
import com.xiaonei.reg.guide.tool.model.Datatable;
import com.xiaonei.reg.guide.tool.parser.IParser;


/**
 * FileParser.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-14 新建文件
 */
public class FileParser implements IParser{
    @Override
    public Datatable parse() {
        System.out.println(input);
        System.out.println("<===============>");
        BufferedReader reader = new BufferedReader(new StringReader(input));
        Datatable current_table = null;
        List<Datacolumn> current_columns = null;
        try {
            String current_table_name = null;
            
            String line = reader.readLine();
            while(line!=null){
                if(current_table_name == null && !line.contains("|")){
                    current_table_name = line.split(";")[0];
                    current_table = new Datatable();
                    current_table.setName(current_table_name);
                    current_columns = new ArrayList<Datacolumn>();
                }
                else{
                    if(!line.startsWith("| Field ") && !line.startsWith("+--") && !line.startsWith("SQL") && !MyStringUtils.isNullOrEmpty(line)){
                        //进入一个column了
                        System.out.println("parsing:"+line);
                        Datacolumn current_column = new Datacolumn();
                        String[] metas = line.split("\\|");
                        current_column.setName(metas[1].trim());
                        current_column.setType(getType(metas[2].trim()));
                        current_columns.add(current_column);
                    }
                }
                System.out.println("parsed:"+line);
                line = reader.readLine();
            }
            current_table.setDatacolumns(current_columns);
            
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return current_table;
    }
    private String getType(String ori_type) {
        String ret = "String";
        if(MyStringUtils.contains(ori_type, "int")){
            ret = "int";
        }
        return ret;
    }
    /*
     * 0:
1: id            
2: int(11) unsigned 
3: NO   
4: PRI 
5: NULL              
6: auto_increment 
7: 
     */
    String input = "";
    public FileParser(String filename){
        StringBuilder sb = new StringBuilder();
        try {
            BufferedReader reader = new BufferedReader(new FileReader(new File(filename)));
            String line = reader.readLine();
            while(line!=null){
                sb.append(line+"\n");
                line = reader.readLine();
            }
            reader.close();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        this.input = sb.toString();
    }
    
    public static void main(String[] args) {
        String[] aa = "| id            | int(11) unsigned | NO   | PRI | NULL              | auto_increment | ".split("\\|");
        int i = 0;
        for (String string : aa) {
            System.out.println(i+++":"+string);
        }
        
    }
}
