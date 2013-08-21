package xce.test;

import mop.hi.oce.adapter.impl.NetworkTripodAdapter;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

public class NetworkTripodAdapterTest {

    static NetworkTripodAdapter client = NetworkTripodAdapter.getInstance();

    @BeforeClass
    public static void setUpBeforeClass() throws Exception {

    }

    @AfterClass
    public static void tearDownAfterClass() throws Exception {
    }

    // **************** network********************************
    @Test
    public void testGetNetworkCountByStage() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkCountByStage(35178, -1));
	System.out.println("*********************************************");
	System.out.println(client.getNetworkCountByStage(35178, 90));
    }

    @Test
    public void testGetValidNetworkCountByStage() {
	System.out.println("*********************************************");
	System.out.println(client.getValidNetworkCountByStage(35178, 30));
	System.out.println("*********************************************");
	System.out.println(client.getValidNetworkCountByStage(35178, -1));
    }

    @Test
    public void testGetNetworkList() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkList(35178));
    }

    @Test
    public void testGetNetworkListByStage() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkListByStage(35178, 30));
	System.out.println("*********************************************");
	System.out.println(client.getNetworkListByStage(35178, -1));
    }

    @Test
    public void testGetNetworkListByStatusIntInt() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkListByStatus(35178, 0));
    }

    @Test
    public void testGetNetworkListByStageAndStatus() {
	System.out.println("*********************************************");
	int[] arr = { 0, -1 };
	System.out.println(client
		.getNetworkListByStageAndStatus(35178, 30, arr));
    }

    @Test
    public void testGetNetworkListByStatusIntIntArray() {
	System.out.println("*********************************************");
	int[] arr = { 0, -1 };
	System.out.println(client.getNetworkListByStatus(35178, arr));
    }

    @Test
    public void testGetNetwork() {
	System.out.println("*********************************************");
	System.out.println(client.getNetwork(35178, 20001001));
    }

    @Test
    public void testGetRegionalNetwork() {
	System.out.println("*********************************************");
	System.out.println(client.getRegionalNetwork(35178));
    }

    @Test
    public void testIsExistInNetwork() {
	System.out.println("*********************************************");
	System.out.println(client.isExistInNetwork(35178, 60001001));
	System.out.println("*********************************************");
	System.out.println(client.isExistInNetwork(35178, 60001002));
    }

    // ******************************highSchool*****************************************
    @Test
    public void testGetHighSchoolInfoCount() {
	System.out.println("*********************************************");
	System.out.println(client.getHighSchoolInfoCount(35178));
    }

    @Test
    public void testGetHighSchoolInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getHighSchoolInfoList(35178));
    }

    @Test
    public void testGetHighSchoolInfoBySchoolId() {
	System.out.println("*********************************************");
	System.out.println(client.getHighSchoolInfoBySchoolId(35178, 10055854));
    }

    // ********************************University***************************************
    @Test
    public void testGetUniversityInfoCount() {
	System.out.println("*********************************************");
	System.out.println(client.getUniversityInfoCount(35178));
    }

    @Test
    public void testGetUniversityInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getUniversityInfoList(35178));
    }

    @Test
    public void testGetUniversityInfoByUnivId() {
	System.out.println("*********************************************");
	System.out.println(client.getUniversityInfoByUnivId(35178, 11002));
    }

    // **********************Workplace***********************************
    @Test
    public void testGetWorkplaceInfoCount() {
	System.out.println("*********************************************");
	System.out.println(client.getWorkplaceInfoCount(35178));
    }

    @Test
    public void testGetWorkplaceInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getWorkplaceInfoList(35178));
    }

    @Test
    public void testGetWorkplaceInfoByWorkId() {
	System.out.println("*********************************************");
	System.out.println(client.getWorkplaceInfoByWorkId(35178, 0));
    }

    // ********************************College***************************************
    @Test
    public void testGetCollegeInfoCount() {
	System.out.println("*********************************************");
	System.out.println(client.getCollegeInfoCount(247322621));
    }

    @Test
    public void testGetCollegeInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getCollegeInfoList(247322621));
    }

    @Test
    public void testGetCollegeInfo() {
	System.out.println("*********************************************");
	System.out.println(client.getCollegeInfo(247322621, 1));
    }

    @Test
    public void testGetCollegeInfoByCollegeId() {
	System.out.println("*********************************************");
	System.out.println(client
		.getCollegeInfoByCollegeId(247322621, 80007152));
    }

    // *********************************ElementarySchool**************************************
    @Test
    public void testGetElementarySchoolInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getElementarySchoolInfoList(1115));
    }

    @Test
    public void testGetElementarySchoolInfoInt() {
	System.out.println("*********************************************");
	System.out.println(client.getElementarySchoolInfo(1115));
    }

    @Test
    public void testGetElementarySchoolInfoIntInt() {
	System.out.println("*********************************************");
	System.out.println(client.getElementarySchoolInfo(1115, 79));
    }

    @Test
    public void testGetElementarySchoolInfoBySchoolId() {
	System.out.println("*********************************************");
	System.out.println(client.getElementarySchoolInfoBySchoolId(1115, 0));
    }

    // *********************************JuniorHighSchool**************************************
    @Test
    public void testGetJuniorHighSchoolInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getJuniorHighSchoolInfoList(1011));
    }

    @Test
    public void testGetJuniorHighSchoolInfoInt() {
	System.out.println("*********************************************");
	System.out.println(client.getJuniorHighSchoolInfo(1011));
    }

    @Test
    public void testGetJuniorHighSchoolInfoIntInt() {
	System.out.println("*********************************************");
	System.out.println(client.getJuniorHighSchoolInfo(1011, 6));
    }

    @Test
    public void testGetJuniorHighSchoolInfoBySchoolId() {
	System.out.println("*********************************************");
	System.out.println(client.getJuniorHighSchoolInfoBySchoolId(1011,
		40136177));
    }

    // *********************************Region**************************************
    @Test
    public void testGetRegionInfoList() {
	System.out.println("*********************************************");
	System.out.println(client.getRegionInfoList(35178));
    }

    @Test
    public void testGetRegionInfo() {
	System.out.println("*********************************************");
	System.out.println(client.getRegionInfo(35178, 60001001));
    }

    // *********************************NetworkBig**************************************
    @Test
    public void testGetNetworkBigObject() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObject(35178).getNetworks());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObject(35178).getHighschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObject(35178).getUnivs());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObject(35178).getWorkspaces());
    }

    @Test
    public void testGetNetworkBigObjectN() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178).getNetworks());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178).getHighschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178).getUnivs());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178).getWorkspaces());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178)
		.getElementaryschool());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178)
		.getJuniorhighschool());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(35178).getCollages());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(1115)
		.getElementaryschool());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigObjectN(1011)
		.getJuniorhighschool());
	System.out.println("*********************************************");
	System.out
		.println(client.getNetworkBigObjectN(247322621).getCollages());

    }

    @Test
    public void testGetNetworkBigData() {
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getNetworks());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getHighschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getUnivs());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getWorkspaces());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178)
		.getElementaryschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178)
		.getJuniorhighschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getCollages());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(35178).getRegions());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(1115)
		.getElementaryschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(1011)
		.getJuniorhighschools());
	System.out.println("*********************************************");
	System.out.println(client.getNetworkBigData(247322621).getCollages());
    }

}
