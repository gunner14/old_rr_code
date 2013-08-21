location ~ ^/home.do {
proxy_pass	http://home;
include	proxy_setting.inc;
}
