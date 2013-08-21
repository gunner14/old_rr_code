<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
                <script language="javascript">
                document.domain="${applicationScope.sysdomain}";
                function $(obj){return parent.document.getElementById(obj);}
                var errcode = <%=request.getParameter("errno")==null?"4":request.getParameter("errno") %>;
                $('load_img_msn').style.display='none';
                $('load_img_email').style.display='none';
                $('notice').innerHTML = "errcode "+errcode;
                $('notice').style.display='block';
                $('email').disabled=false;
                $('msn').disabled=false;
                $('submit_email').disabled=false;
                $('submit_msn').disabled=false;
                
                switch(errcode){
                case 1:
                        $('notice').innerHTML="帐号或密码错误，请尝试重新输入。";
                                break;
                case 2:
                        $('notice').innerHTML="帐号或密码错误，请尝试重新输入。";
                                break;
                case 3:
                        break;
                case 4:
                        $('notice').innerHTML="没有找到在人人网注册的联系人，再试试别的邮箱或msn吧:)";
                        break;
                case 5:
                        $('notice').innerHTML="系统错误，请重试。";
                        break;
                case 6:
                        $('notice').innerHTML="等待超时，请重试。";
                        break;
                }
                </script>
</head>
<body>

</body>
</html>