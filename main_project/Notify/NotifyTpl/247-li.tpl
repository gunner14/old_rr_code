{{#LI_SEC}}
        <li class="request">
                <div class="figure">
                        <a href="{{inviterLink}}">
                                        <img src="{{inviterPic}}">
                        </a>
                </div>
                <div class="detail">
                        <h4 class="header">
                                {{inviterMsg}}
                        </h4>
                        <div class="section description">
                                <p>{{desc}}</p>
                        </div>
                </div>
                <div class="btns">
                        <button click="lover_accept:{{acpUrl}},{{nid}}" class="accept">接受</button>
                        <button click="lover_ignore:{{ignoreUrl}},{{nid}}" class="ignore">忽略</button>
                </div>
                <div class="btn-x"><a class="x-to-delete" click="lover_xxx:{{ignoreUrl}},{{nid}}"></a></div>
        </li>
{{/LI_SEC}}
