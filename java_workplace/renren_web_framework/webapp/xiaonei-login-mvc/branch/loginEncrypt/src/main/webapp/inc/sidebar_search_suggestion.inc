<%@ page pageEncoding="UTF-8" %>
<style>
.search_container_sidebar {
	position:absolute;
	z-index:20000;
}
.search_com_sidebar {
	overflow:hidden;
	border:1px solid #BDC7D8;
	background:#fff;
	position:absolute;
	top:20px;
}
.com_container_sidebar {
	padding:2px 0px;
	overflow:hidden;
	color:#000000;
}
.com_list_sidebar, .com_list_selected_sidebar {
	padding:3px 5px;
	cursor:pointer;
	overflow:hidden;
	color:#555555;
	height:15px;
	border-bottom:1px solid #DBDBDC;
	margin:0;
}
.com_list_selected_sidebar {
	background:#3B5998;
	color:#fff;
}
.search_highlight_sidebar {
	background:#DCDCDC;
	color:#000000;
	font-weight:bold;
}
</style>
<script type="text/javascript">
var matchedCom_sidebar = {};
var invalidCom_sidebar = {};
function _autoSearch_sidebar(src, container, photoNode, prompt) {
	this.menu = null;
	this.mindex = -1;
	this.src = src;
	this.container = container;
	this.node = container.parentNode;
	this.photoNode = photoNode;
	this.prompt = prompt;
	this.tmpcontent = '';
	this.beginflag = false;
	this.com_sel = "";
	var instance = this;
	this.listencall = function() {
		if(instance.beginflag) {
			instance.doSearch();
		}
	}
	this.mylisten = new Form.Element.Observer(this.src, 0.1, this.listencall);
	this.release = function() {
		instance.menu = null;
		instance.mindex = -1;
		instance.node.style.display = 'none';
		if(instance.com_sel == "") {
			
			instance.photoNode.style.backgroundImage = '';
		}
	};
	this.wrap_selectItem = function(next) {
		return function() {
			instance.selectItem(next);
		}
	};
	this.getSelectedByMouse = function(index) {
		return function() {
			instance.getSelected(index);
		}
	};
	this.src.onfocus = function() {
		if(instance.src.value.trim() == instance.prompt) {
			instance.src.value = "";
		}
		instance.beginflag = true;
	};
	this.beginsearch = function() {
		instance.beginflag = true;
	}
	this.src.onkeyup = function() {
		setTimeout(instance.beginsearch, 100);
	};
	this.src.onblur = function() {
		this.beginflag = false;
		instance.release();
	};
	this.src.onkeydown = function(evt) {
			if(evt==null) evt = window.event;
			return instance.selectByKeybord(evt.keyCode);
			
	};
}
_autoSearch_sidebar.prototype.doSearch = function() {
	if(this.src.value.trim() == '')
	{
		this.node.style.display = 'none';
	}
	if(this.tmpcontent==this.src.value.trim()) {
		return;
	}
	this.tmpcontent = this.src.value.trim();
	if(this.tmpcontent == this.prompt) {
		return;
	}
	if(this.tmpcontent == "") {
		this.node.style.display = 'none'; return;
	}
	this.com_sel = "";
	if(typeof(invalidCom_sidebar[this.tmpcontent]) != "undefined") { 
		this.displaynone(invalidCom_sidebar[this.tmpcontent]); return;
	}
	else if(typeof(matchedCom_sidebar[this.tmpcontent]) != "undefined") { 
		this.display(matchedCom_sidebar[this.tmpcontent]); return;
	}
	else {
		var records = new Array();
		for(var i=0,count=0; i<friendLists.length&&count<1000; ++i) {
			try {
				if(friendLists[i].name.toLowerCase().indexOf(this.tmpcontent.toLowerCase())>-1) {
					records.push(friendLists[i]);
					++count;
				}
			} catch(e) {}
		}
		if(records.length > 0) {    
			this.display(records);
			matchedCom_sidebar[this.tmpcontent] = records;
		}else
		{
		this.node.style.display = 'none';
		}
	}
}
_autoSearch_sidebar.prototype.display = function(records) {
	this.menu = new Array();
	this.mindex = -1;
	this.container.innerHTML = '';
	var limit = records.length;
	if(records.length > 20 )
	{
		limit = 20;
	}
	for(var i=0; i<limit; i++) {  
		var item = document.createElement('div');
		item.className = 'com_list_sidebar';
		var cname = records[i].name;
		var text = cname.toLowerCase().replace(this.tmpcontent.toLowerCase(), "<span class='search_highlight_sidebar'>" + this.tmpcontent.toLowerCase() + "</span>");
		item.innerHTML = text;
		result_id = records[i].uid;
		item.id = 'com_' + result_id;
		if(i == records.length - 1) item.style.borderBottomStyle = 'none';
		Event.observe(item, 'mouseover', this.wrap_selectItem(i), false);
		Event.observe(item, 'mousedown', this.getSelectedByMouse(i), false);
		this.menu[i] = {'item_id':item.id,'cid':result_id,'cname':cname,'photo':records[i].head_url};
		this.container.appendChild(item);
	}
	this.container.parentNode.style.background = '#fff';
	this.node.style.display = '';
}
_autoSearch_sidebar.prototype.displaynone = function(content) {
	this.container.innerHTML = content;
	this.container.parentNode.style.background = '#ddd';
	this.menu = null;
	this.mindex = -1;
	this.node.style.display = '';
}
_autoSearch_sidebar.prototype.selectItem = function(next) {
	if(this.menu == null) return;
	if(next > this.menu.length - 1) next = 0;
	if(next < 0) next = this.menu.length - 1;
	if(this.mindex != -1) $(this.menu[this.mindex]['item_id']).className = 'com_list_sidebar';
	$(this.menu[next]['item_id']).className = 'com_list_selected_sidebar';
	this.photoNode.style.backgroundImage = "url(" + this.menu[next].photo + ")";
	this.mindex = next;
	var tmp = this.node.scrollHeight * this.mindex / this.menu.length - 80;
	if(tmp > 0) this.node.scrollTop = tmp;
	else this.node.scrollTop = 0;
}
_autoSearch_sidebar.prototype.getSelected = function(index) {
	this.beginflag = false;
	if(this.menu==null || this.mindex<0) return;
	var tmp = this.menu[this.mindex];
	this.src.value = tmp['cname'];
	this.com_sel = tmp['cid'];
	this.tmpcontent = tmp['cname'];
	if($('fid_in_sidebar').value=='sidebar')
	{
		$('search_form').action = '${applicationScope.urlProfile}/profile.do?id='+this.getSelFriend();
	}
	this.release();
}
_autoSearch_sidebar.prototype.getSelFriend = function() {
	return this.com_sel;
}
_autoSearch_sidebar.prototype.selectByKeybord = function(keyCode) {
	if(keyCode == 38) {
		this.selectItem(this.mindex - 1); return false;
	} else if(keyCode == 40) {
		this.selectItem(this.mindex + 1); return false;
	} else if(keyCode == 13) {
		this.getSelected(this.index);
		if($('fid_in_sidebar').value=='sidebar')
		{
			$('search_form').submit();
		}else if($('fid_in_sidebar').value=='friend_search')
		{
			$('searchNameForm').submit();
		}
		return false;
	} else return true;
}
</script>


	  <div class="search_container_sidebar" style="left:0px;top:0px;">
		<div id="search_f1_sidebar" class="search_com_sidebar" style="display:none;width:102px;">
			<div id="f1_container_sidebar" class="com_container_sidebar"></div>
		</div>
		<div id="friendPhoto_sidebar" style="top:-10px;left:220px;"></div>
	 </div>

<input type="hidden" name="fid" id="fid_in_sidebar"/>