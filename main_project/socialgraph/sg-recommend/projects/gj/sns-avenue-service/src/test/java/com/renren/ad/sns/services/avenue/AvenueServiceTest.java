package com.renren.ad.sns.services.avenue;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.AbstractJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;



import com.google.code.morphia.Datastore;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:beans/ranking-service.xml")
public class AvenueServiceTest extends AbstractJUnit4SpringContextTests{
	@Autowired
	private AvenueBuilder avenueBuilder;
	
	@Autowired
	protected Datastore ds;
	@Test
	public void test() throws Exception{
		avenueBuilder.getHammer().isTest=true;
		avenueBuilder.getHammer().testGoodsIds.add(1210141900495357231l);
		avenueBuilder.getHammer().testGoodsIds.add(1210151159072721277l);
		//avenueBuilder.getHammer().testGoodsIds.add(1208092128373069572L);
		avenueBuilder.createIndex();
		System.out.println("ok");
//		 long date = 1207160817060518840l ;
//         // most recently goods (1h)
//        double point = Math.log(Math.log(1+date*10000)+Math.log(1.0 + Math.log(1.0 + 0
//                          + 0* 0.05)));
//        System.out.println(point);
	}
	
//	@Test
//	public void test(){
//		AvenueSortParameter avenueSortParameter=ds.createQuery(AvenueSortParameter.class).get();
//		if(avenueSortParameter==null){
//			avenueSortParameter=new AvenueSortParameter();
//			ds.save(avenueSortParameter);
//		}else{
//			avenueSortParameter.setOpenCredibleForNotLikeSort(false);
//			ds.save(avenueSortParameter);
//		}
//	}
}
