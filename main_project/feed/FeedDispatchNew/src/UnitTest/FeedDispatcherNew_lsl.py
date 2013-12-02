#! /usr/bin/env python

import sys, traceback, time
sys.path.append('/data/xce/IcePy-3.3.0/python')
import Ice

Ice.loadSlice("-I../UtilSlice/slice -I../OceSlice/slice --all ../UtilSlice/slice/Util.ice ../OceSlice/slice/RFeed.ice")

from xce.feed import *
indexManager = None
ic = Ice.initialize(["--Ice.Default.Locator=XceFeed/Locator:default -h 10.3.17.77 -p 14300:default", "--Ice.Trace.Network=1"])
#ic = Ice.initialize(["--Ice.Default.Locator=XceFeed/Locator:default -h 10.3.17.77 -p 14300:default", "--Ice.Trace.Network=1"])
prx = FeedDispatchNewPrx.checkedCast(ic.stringToProxy("FDNW@FeedDispatcherNew0"))
print prx
try:

  seed = FeedSeed()
  seed.feed = 1 
  seed.miniMerge = 1209101040256883736
  seed.newsMerge = 1209101040256883736
  seed.source = 1209101040256883736
  seed.psource = 1209101040256883736
  seed.actor = 260773925 
  seed.type = 1342251918
  seed.ptype = 1342251918
  seed.time = int(time.time())
  seed.baseWeight = 18
  xml502= """<f version="1" tpl="cs"><album><id>1208221342567009933</id><name>桔小熙的分享</name></album><cat><catLink>http://tag</catLink><catName>notag</catName></cat><from><gjimg>http://hdn.xnimg.cn/photos/hdn221/20091207/0855/h_tiny_QuX4_7b290005ff432f75.jpg</gjimg><id>304934608</id><name>桔小熙</name><tinyimg>http://hdn.xnimg.cn/photos/hdn221/20091207/0855/h_tiny_QuX4_7b290005ff432f75.jpg</tinyimg></from><goods><content>正品耐克运动鞋 反毛皮透气SB板鞋 复古休闲鞋 2012新款男鞋</content><img>http://fmn.rrimg.com/fmn056/gouwu/20120910/1040/main2_qYZA_41c700008ddd125e.jpg</img><name>正品耐克运动鞋 反毛皮透气SB板鞋 复古休闲鞋 2012新款男鞋</name><price>245</price><url>http://j.renren.com/plaza/1209101040256883736</url></goods><sourceId>1209101040256883736</sourceId><taobaoId>1209101040256883736</taobaoId><time>1347244825724</time></f>"""
  xmlReplace = """ <f v="1.0" type="blog_publish">
     <source></source>
     <time>1258547249776</time>
     <type>601</type>
     <from>
           <id>298671393</id>
           <name>成明</name>
           <tinyimg>hdn221/20091028/1945/tiny_NJjB_33246b019118.jpg</tinyimg> 
     </from>
     <blog>
         <id>430502249</id> 
         <title>&lt;span&gt;dssssssssssssssssssss&lt;/span&gt;</title>
         <describe>&lt;span&gt;ssss...&lt;/span&gt;</describe>
         <url>http://blog.renren.com/GetEntry.do?id=430502249&amp;owner=298671393</url>
         <video>
                <exist></exist>
                        <image></image>
                </video>
                <photo>
                        <exist></exist>
                        <image></image>
                </photo>
                <music>
                        <exist></exist>
                </music>
     </blog>
</f>                                     """
  seed.xml=xml502;
  seed.extraProps = dict()
  seed.extraProps["base_weight_id"] = "17"
  seed.extraProps["expr"] = "u(260773925)"
  seed.extraProps["publish_mini"] = "1"
  seed.extraProps["publish_news"] = "1"
  seed.extraProps["send_config"] = "1"
  seed.extraProps["update_time"] = "1"

  reply = FeedReply()
  reply.oldId = 37717933;
  reply.oldReply = """<f>
<reply>
<id>37717933</id>
<type>0</type>
<time>2012-09-04 09:53</time>
<body>&quot;1&quot;</body>
<im>
<body></body>
</im>
<from>
<id>236239032</id>
<name>何其旺</name>
<tinyimg>http://hdn.xnimg.cn/photos/hdn321/20120826/1035/tiny_Ld3i_635c00015b7b1375.jpg</tinyimg>
</from>
</reply>
</f> """
  reply.newId = 38072232
  reply.newReply = """ 
 <f>
<reply>
<id>38072231</id>
<type>0</type>
<time>2012-09-10 12:00</time>
<body>&quot;lishilong看完对料理界都要绝望了。lishilong&quot;</body>
<im>
<body></body>
</im>
<from>
<id>278080818</id>
<name>❀李娜❀cat’</name>
<tinyimg>http://hdn.xnimg.cn/photos/hdn421/20120615/2130/h_tiny_0Fd7_2ae2000002e21376.jpg</tinyimg>
</from>
</reply>
</f>

  """
  reply.count = 2;
#  prx.DispatchEDM(seed)
#  prx.Dispatch(seed)
#  prx.DispatchWithReply(seed,reply)
#  prx.DispatchAgain(1209101040256883736,9102,260773925,448456754)
#  print  prx.GetFeedId(1209101040256883736,9102,260773925)
#  config = dict();
#  prx.AddFeedReplyWithConfig(1209101040256883736,9102,260773925,reply,config);
#  prx.AddFeedReply(1209101040256883736,9102,260773925,reply)
#  prx.AddFeedReplyId(9102,1L,reply)
#  prx.RemoveFeedById(1L);
#  prx.RemoveFeedBySource(1209101040256883736,9102);
#  prx.RemoveFeedByParent(1209101040256883736,1342251918);
#  prx.RemoveFeed(1209101040256883736,9102,260773925)
#  prx.RemoveFeedReply(1209101040256883736,9102,260773925,1,FeedReply())
#  prx.ReplaceXML(1209101040256883736,9102,260773925,xmlReplace)  
#  prx.AddBuddyNotify(448456754,260773925)
#  prx.JoinPageNotify(600002616,448456754)
#  prx.AddMiniSiteFeed(671000125,260773925)
#  guides = [448456754]
#  prx.AddMiniSiteFeedToGuides(3674946092068602691,3711,671308189,guides)

except:
    traceback.print_exc()
    status = 1 
