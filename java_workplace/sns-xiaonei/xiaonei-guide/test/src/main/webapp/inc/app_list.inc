<%@ page contentType="text/html;charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn"%>
<%@page import="java.util.HashMap"%>
<%@page import="java.util.Map"%>
<c:set var="sideNavUrl" value="${item.sideNavUrl}"/>
<c:set var="apps" value="${applicationScope.urlApps}"/>
<c:set var="origin1" value="&origin=104"/>
<c:set var="origin2" value="?origin=104"/>
<c:set var="interrogation" value="?"/>
<c:if test="${fn:startsWith(sideNavUrl,apps)}">
	<c:if test="${fn:contains(sideNavUrl,interrogation)}"><c:set var="sideNavUrl" value="${sideNavUrl}${origin1}"></c:set></c:if>
	<c:if test="${!fn:contains(sideNavUrl,interrogation)}"><c:set var="sideNavUrl" value="${sideNavUrl}${origin2}"></c:set></c:if>
</c:if>
<%
	Map appPicItems = new HashMap(100);
	{
		appPicItems.put(23163,0);
		appPicItems.put(3,32);
		appPicItems.put(26352,64);
		appPicItems.put(2,96);
		appPicItems.put(30496,128);
		appPicItems.put(24997,160);
		appPicItems.put(34185,192);
		appPicItems.put(44097,224);
		appPicItems.put(30226,256);
		appPicItems.put(29507,288);
		appPicItems.put(30598,320);
		appPicItems.put(33114,352);
		appPicItems.put(1008315,384);
		appPicItems.put(30452,416);
		appPicItems.put(25049,448);
		appPicItems.put(21461,480);
		appPicItems.put(33195,512);
		appPicItems.put(16070,544);
		appPicItems.put(28677,576);
		appPicItems.put(26609,608);
		appPicItems.put(4,640);
		appPicItems.put(19974,672);
		appPicItems.put(1000035,704);
		appPicItems.put(1000025,736);
		appPicItems.put(29127,768);
		appPicItems.put(50072,800);
		appPicItems.put(24214,832);
		appPicItems.put(8,864);
		appPicItems.put(29669,896);
		appPicItems.put(31239,928);
		appPicItems.put(12012,960);
		appPicItems.put(29831,992);
		appPicItems.put(48170,1024);
		appPicItems.put(35,1056);
		appPicItems.put(20706,1088);
		appPicItems.put(44018,1120);
		appPicItems.put(68338,1152);
		appPicItems.put(29375,1184);
		appPicItems.put(19,1216);
		appPicItems.put(65783,1248);
		appPicItems.put(49758,1280);
		appPicItems.put(17940,1312);
		appPicItems.put(30739,1344);
		appPicItems.put(30865,1376);
		appPicItems.put(32133,1408);
		appPicItems.put(30015,1440);
		appPicItems.put(52753,1472);
		appPicItems.put(30822,1504);
		appPicItems.put(1000040,1536);
		appPicItems.put(1018587,1568);
		appPicItems.put(29520,1600);
		appPicItems.put(45106,1632);
		appPicItems.put(13496,1664);
		appPicItems.put(30744,1696);
		appPicItems.put(57947,1728);
		appPicItems.put(31715,1760);
		appPicItems.put(27727,1792);
		appPicItems.put(31373,1824);
		appPicItems.put(24985,1856);
		appPicItems.put(28556,1888);
		appPicItems.put(34052,1920);
		appPicItems.put(35191,1952);
		appPicItems.put(37922,1984);
		appPicItems.put(17254,2016);
		appPicItems.put(29454,2048);
		appPicItems.put(22929,2080);
		appPicItems.put(29492,2112);
		appPicItems.put(23881,2144);
		appPicItems.put(31192,2176);
		appPicItems.put(48069,2208);
		appPicItems.put(20805,2240);
		appPicItems.put(24379,2272);
		appPicItems.put(66999,2304);
		appPicItems.put(13462,2336);
		appPicItems.put(23784,2368);
		appPicItems.put(23918,2400);
		appPicItems.put(25366,2432);
		appPicItems.put(14293,2464);
		appPicItems.put(30789,2496);
		appPicItems.put(29903,2528);
		appPicItems.put(75,2560);
		appPicItems.put(9,2592);
		appPicItems.put(15,2624);
		appPicItems.put(14,2656);
		appPicItems.put(13,2688);
		appPicItems.put(18,2720);
		appPicItems.put(16534,2752);
		appPicItems.put(17397,2784);
		appPicItems.put(7,2816);
		appPicItems.put(30,2848);
		appPicItems.put(10,2880);
		appPicItems.put(17,2912);
		appPicItems.put(11,2944);
		appPicItems.put(45,2976);
		appPicItems.put(14870,3008);
		appPicItems.put(14514,3040);
		appPicItems.put(48,3072);
		appPicItems.put(11,3104);
		appPicItems.put(22986,3136);
		appPicItems.put(17954,3168);
	}
 
	request.setAttribute("appPicItems",appPicItems);
%>

<li<c:if test="${item.appId == requestScope.userLeftSideMenus.latestMenu.appId}"> class="last-app"</c:if><c:if test="${item.appId == 22356||item.appId == 21106}"> class="appmenuitemmoto"</c:if>>
<a href="${sideNavUrl}" style="<c:choose><c:when test="${!empty appPicItems[item.appId]}">background-image:url(${applicationScope.urlSmallStatic}/imgpro/icons/new-app100.png); background-position:10px ${-appPicItems[item.appId]+5}px;</c:when><c:otherwise>background-image: url(${item.appIcon});</c:otherwise></c:choose>"<c:if test="${!empty item.urlTarget}"> target="${item.urlTarget}"</c:if>>${item.appName}</a>
</li>

