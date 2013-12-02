{{#HEAD}}
<div class="requests-header" id="requests_lover_header">
	<h2><img src="http://a.xnimg.cn/imgpro/v6/icon/lovehouse.gif">情侣空间邀请<span>{{247_count}}</span></h2>
</div>
{{/HEAD}}
<script type="text/javascript">
object.use('dom,net', function(exports, dom, net){
				function delNotify(nid){
					new XN.NET.xmlhttp({
						url:'http://notify.renren.com/rmessage/remove?type=247&nid=' + nid,
						method:'post',
						onSuccess:function(r){
						},
						onError:function(){
							XN.DO.showError('网络通信失败,请重试');
						}
					});	
				}
        function acceptLoversRequest(acceptUrl, cFun){
        				acceptUrl = acceptUrl.replace("http://","");
                new XN.NET.xmlhttp({
                        url:'http://' + acceptUrl,
                        onSuccess:function(r){
                                var rs = XN.json.parse( r.responseText);
                                var succ = rs.succ;
                                if(succ){
                                        cFun(rs);
                                }else{
                                        XN.DO.showError( rs.msg );
                                }
                        },
                        onError:function(){
                                XN.DO.showError( "操作失败，请稍后再试或者联系管理员" );
                        }
                });
        }
        function ignoreLoversRequest(ignoreUrl, cFun){
        				ignoreUrl = ignoreUrl.replace("http://","");
                new XN.NET.xmlhttp({
                        url:'http://' + ignoreUrl,
                        onSuccess:function(r){
                                var rs = XN.json.parse( r.responseText);
                                var succ = rs.succ;
                                if(succ){
                                        cFun(rs);
                                }else{
                                        XN.DO.showError( rs.msg );
                                }
                        },
                        onError:function(){
                                XN.DO.showError( "操作失败，请稍后再试或者联系管理员" );
                        }
                });
        }
        ReqCenter.regReqType('lover:247', {
                accept: function(info){
                        var el = info.el;
                        var url = info.params[0];
                        var nid = info.params[1];
                        acceptLoversRequest(url, function(r){
                                var detail = dom.getElement('div.detail', el);
                                var btns = dom.getElement('div.btns', el);
                                detail.innerHTML=r.msg; 
                                btns.hide();
                                delNotify(nid);
                        });
                },
                ignore: function(info){
                        var el = dom.wrap(info.el);
                        var url = info.params[0];
                        var nid = info.params[1];
                        ignoreLoversRequest(url, function(r){
                                if(r){
                                        var inviterLink = el.getData('inviter_link');
                                        var inviterName = el.getData('inviter_name');
                                        var btns = dom.getElement('div.btns', el);
                                        var detail = dom.getElement('div.detail', el);
                                        detail.innerHTML=r.msg;
                                        btns.hide();
                                        delNotify(nid);
                                }
                        });
                },
                xxx: function(info){
                        var el = dom.wrap(info.el);
                        var url = info.params[0];
                        var nid = info.params[1];
                        ignoreLoversRequest(url, function(r){
                                if(r){
                                        el.dispose();
                                        delNotify(nid);
                                }
                        });
                }
        });
});   
</script>

<ul id="requests_lover_list" class="requests-list">
{{>LI_247}}
</ul>
{{#MORE}}
<div class="showmore">
	<a data-page="1" data-count="{{247_count}}" onclick="ReqCenter.funCall('req_more', 'lover', this)" class="requests-show-more">显示更多情侣空间邀请
	</a>
</div>
{{/MORE}}
