rm -rf target
mvn compile
mvn package
mvn -U clean package
#scp  target/renrengame.war xce@t12.xntalk.d.xiaonei.com:/data/xce/apache-tomcat-7.0.12/
scp  target/renrengame.war xce@t19.xntalk.d.xiaonei.com:/data/xce/yinghao.du/apache-tomcat-7.0.12/webapps
