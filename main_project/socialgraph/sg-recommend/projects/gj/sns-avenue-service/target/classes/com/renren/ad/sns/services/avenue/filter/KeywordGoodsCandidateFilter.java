package com.renren.ad.sns.services.avenue.filter;

import java.util.ArrayList;
import java.util.List;

import com.google.code.morphia.Datastore;
import com.renren.ad.sns.data.AvenueFilterKeywords;
import com.renren.ad.sns.data.Goods;
import com.renren.ad.sns.services.avenue.DataHammer;

public class KeywordGoodsCandidateFilter implements GoodsCandidateFilter{
	 
	    private List<String> keyword4Name=new ArrayList<String>();
	    private List<String> keyword4Desc=new ArrayList<String>();
	    private Datastore datastore;
	    @Override
	    public List<Goods> filter(List<Goods> raw, DataHammer dataHammer) {
	        List<Goods> result = new ArrayList<Goods>();
	        loadSpamKeyword();
	        int count=0;
	        if(raw != null && raw.size() > 0){
	            for(Goods goods: raw){
	                if(matchSpamKeyword(goods.getName(), keyword4Name))continue;
	                if(matchSpamKeyword(goods.getDesc(), keyword4Desc))continue;
	                count++;
	                result.add(goods);
	            }
	        }
	        System.out.println("keyword filter goods count is "+(raw.size()-count));
	        return result;
	    }
	    private void loadSpamKeyword(){
	    	List<AvenueFilterKeywords> spamKeywords=datastore.find(AvenueFilterKeywords.class).asList();
	    	setSpamKeywords(spamKeywords);
	    }
	    
		private  void setSpamKeywords(List<AvenueFilterKeywords> keywords){
	    	if(keywords==null||keywords.size()==0){
	    		return;
	    	}
	    	for(AvenueFilterKeywords avenueFilterKeyword: keywords){
	    		if(avenueFilterKeyword.getType().intValue()==AvenueFilterKeywords.SPAM_KEYWORD_TYPE_FOR_NAME){
	    			keyword4Name.add(avenueFilterKeyword.getKeyword());
	    		}
	    		if(avenueFilterKeyword.getType().intValue()==AvenueFilterKeywords.SPAM_KEYWORD_TYPE_FOR_DESC){
	    			keyword4Desc.add(avenueFilterKeyword.getKeyword());
	    		}
	    	}
	    }
	    /**
	     * 判断某个字符串是否匹配spam关键字
	     * @param name
	     * @param keywords
	     * @return
	     */
		private boolean matchSpamKeyword(String name, List<String> keywords) {
			if(name==null||keywords==null)return true;
			for(String keyword:keywords){
				if(name.indexOf(keyword)!=-1){
					return  true;
				}
			}
			return false;
		}
		public void setDatastore(Datastore datastore) {
			this.datastore = datastore;
		}
	
}
