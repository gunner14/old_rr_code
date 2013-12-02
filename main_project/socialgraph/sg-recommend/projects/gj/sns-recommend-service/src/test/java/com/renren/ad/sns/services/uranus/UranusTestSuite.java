package com.renren.ad.sns.services.uranus;

import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

import com.renren.ad.sns.services.uranus.cf.CFCandidatePickerTest;
import com.renren.ad.sns.services.uranus.cf.CFRecommendationLoaderTest;
import com.renren.ad.sns.services.uranus.prg.RankerTest;

@RunWith(Suite.class)
@SuiteClasses( {
    CandidateListTest.class,
    CandidatesFilterUtilTest.class,
    CategoryCandidatePickerTest.class,
    DefaultCandidatesFilterTest.class,
    SimpleMetadataProviderTest.class,
    /*UranusServiceProxyTest.class,*/
    
    CFCandidatePickerTest.class,
    CFRecommendationLoaderTest.class, 
    
    RankerTest.class
    })
    
public class UranusTestSuite {
}
