package com.renren.sns.guide.model;

import java.util.ArrayList;
import java.util.List;

/**
 * @author: wei.xiang
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-12 上午11:13:10 类说明
 */
public class GuideSearchRelation implements GuideIRelation {

    private List<GuideIRelationFriend> list = new ArrayList<GuideIRelationFriend>();

    private String more = "";

    private String title = "";

    private int count = 0;

    private String type;

    private String typeName;

    private int typeId = 0;

    public String getMore() {
        return more;
    }

    public void setMore(String more) {
        this.more = more;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setTypeName(String typeName) {
        this.typeName = typeName;
    }

    @Override
    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public void add(GuideSearchRelationFriend friend) {
        list.add(friend);
    }

    public List<GuideIRelationFriend> getList() {
        return list;
    }

    public void setList(List<GuideIRelationFriend> list) {
        this.list = list;
    }

    public String getMoreLink() {
        return more.toString();
    }

    public void setMoreLink(String more) {
        this.more = more;
    }

    public String getTitle() {
        return title.toString();
    }

    public void setTitle(String title) {
        this.title = title;
    }

    @Override
    public String getType() {
        // TODO Auto-generated method stub
        return this.type;
    }

    @Override
    public String getButtonValue() {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String getTypeName() {
        // TODO Auto-generated method stub
        return this.typeName;
    }

    @Override
    public int getTypeId() {
        return typeId;
    }

    public void setTypeId(int typeId) {
        this.typeId = typeId;
    }

}
