#!/bin/sh

usage() {
echo "$0 --create [-n|--name=container_name] [-i|--ip=192.168.122.x] [--remote-download] [--host-port=9991]"
echo "$0 --<start|stop|destroy> <-n|--name=container_name>"
echo "  -n, --name=container_name"
echo "    specify the container's name, default: \"c\""
echo ""
echo "  -i, --ip=192.168.122.x"
echo "    specify the container's ip, default: 192.168.122.2"
echo ""
echo "  --remote-download" 
echo "    by default, this script will use 10.7.18.21 as software repository, enable this option"
echo "    when 21 is not accessable, then all softwares will be downloaded from remote site"
echo ""
echo "  --host-port=port" 
echo "    the ip of host machine that will be used in NAT forward. All incoming tcp will to this port"
echo "    will be forward to guest's 80 port, default: 9991"
echo ""
exit 0
}

software_repo="10.7.18.21:/data/software"
xiaonei_yum_repo="10.7.18.21:/etc/yum.repos.d/xiaonei-private.repo"
template_tar="centos-6-x86_64.tar.gz" 
template_url="http://download.openvz.org/template/precreated/"$template_tar
lxc_package="lxc-0.7.5"
lxc_tar=$lxc_package".tar.gz"
lxc_url="http://sourceforge.net/projects/lxc/files/lxc/"$lxc_package"/"$lxc_tar
remote_download=false
software_cache="/tmp"
containers_root="/data/lxccontainers"
containers_info=$containers_root/info
container_name="c"
container_ip="192.168.122.2"
container_gw="192.168.122.1"
host_port=9991
guest_port=80
lxc_conf="lxc.conf"
lxc_fstab="lxc.fstab"
lxc_rootfs="rootfs"
cgroup_dir="/data/cgroup"
lxc_lib_dir="/usr/local/var/lib/lxc"

# error codes
err_download=-1
err_mkdir=-2
err_install_libvirt=-3
err_ip_used=-4
err_host_port_used=-5
err_container_not_exist=-6
err_container_has_exist=-7
err_install_libcap=-8

cecho() {
echo -e "\033[31m"$1"\033[0m"
}
warnecho() {
echo -e "\033[33m"$1"\033[0m"
}
install_prerequisite() {
echo "install prerequisits if needed ..."
# create software cache directory
if ! mkdir -p $software_cache; then
	cecho "mkdir failed: "$software_cache", exit"
	exit $err_mkdir
fi
if ! mkdir -p $cgroup_dir; then
	cecho "mkdir failed: "$cgroup_dir", exit"
	exit $err_mkdir
fi
if ! mkdir -p $lxc_lib_dir; then
	cecho "mkdir failed: "$lxc_lib_dir", exit"
	exit $err_mkdir
fi

# install libcap 
if ! yum install -y libcap; then
	cecho "install libcap failed, exit"
	exit $err_install_libcap
fi
	

echo "check libvirt version ..."
libvirtd --version 2>/dev/null
if [ $? -ne 0 ]; then
	# install libvirt
	echo "install libvirt ..."
	yum -y install libvirt.x86_64
	if [ $? -ne 0 ]; then
		cecho "install libvirt error, exit"
		exit $err_install_libvirt
	fi
	# uncomment "#mdns_adv = 0"
	# see /etc/libvirt/libvirtd.conf for more information
	sed -i -e "s/\#mdns_adv\ =\ 0/mdns_adv\ =\ 0/" /etc/libvirt/libvirtd.conf
	/etc/init.d/libvirtd restart
	# start virbr0
	killall -9 dnsmasq 2>/dev/null
	virsh net-start default 2>/dev/null

	# turn off cgconfig service to prevent auto mounting cgroup
	chkconfig cgconfig off 2>/dev/null
	echo "install libvirt done"
fi
cat /etc/fstab | grep cgroup
if [ $? -ne 0 ]; then
	echo "none "$cgroup_dir" cgroup defaults 0 0" >> /etc/fstab
	mount -a
fi

echo "check lxc version ..."
lxc-version 2>/dev/null
if [ $? -ne 0 ]; then
	# install lxc-*
	echo "install lxc ..."
	if [ ! -f $software_cache/$lxc_tar ]; then 
		echo "downloading lxc ..."
		if $remote_download; then
			wget $lxc_url --directory-prefix=$software_cache
		else
			scp -o "StrictHostKeyChecking no" root@$software_repo/$lxc_tar $software_cache
		fi
		if [ $? -ne 0 ]; then
			cecho "download lxc error, exit"
			exit $err_download
		fi
		echo "downloading lxc done"
	fi
	tar zxf $software_cache/$lxc_tar --directory=$software_cache
	cd $software_cache/$lxc_package
	./configure
	make install -j8
	cd -
	echo "install lxc done"
fi 
if ! mkdir -p $containers_root; then
	cecho "create path for containers root failed, exit"
	exit $err_mkdir
fi
touch $containers_info
echo "install prerequisits done"
}

create_rootfs() {
echo "create rootfs"
if [ ! -f $software_cache/$template_tar ]; then 
	echo "downloading template ..."
	if $remote_download; then
		wget $template_url --directory-prefix=$software_cache
	else
		scp -o "StrictHostKeyChecking no" root@$software_repo/$template_tar $software_cache
	fi
	if [ $? -ne 0 ]; then
		cecho "download template error, exit"
		exit $err_download
	fi
	echo "downloading template done"
fi 

mkdir -p $containers_root/$container_name 
mkdir -p $containers_root/$container_name/$lxc_rootfs

echo "untar "$template_tar" ..."
tar zxf $software_cache/$template_tar --directory=$containers_root/$container_name/$lxc_rootfs 
echo "untar "$template_tar" done"
if ! $remote_download; then
	scp -o "StrictHostKeyChecking no" root@$xiaonei_yum_repo $containers_root/$container_name/$lxc_rootfs/etc/yum.repos.d
fi

echo "create rootfs done"
}

create_lxc_fstab() {
echo "create lxc fstab"
rootfs=$containers_root/$container_name/$lxc_rootfs
cat > $containers_root/$container_name/$lxc_fstab << EOF
none $rootfs/proc              proc    defaults 0 0
none $rootfs/sys               sysfs   defaults 0 0
none $rootfs/dev/pts   devpts  defaults 0 0
none $rootfs/dev/shm tmpfs  defaults 0 0

EOF
echo "create lxc fstab done"
}

create_lxc_conf() {
echo "create lxc conf"
cat > $containers_root/$container_name/$lxc_conf << EOF
lxc.utsname = $container_name
lxc.tty = 4
lxc.network.type = veth
lxc.network.flags = up
lxc.network.link = virbr0
lxc.network.name = eth0
lxc.network.mtu = 1500
lxc.network.ipv4 = $container_ip/24
lxc.rootfs = $containers_root/$container_name/rootfs
# can use lxc.mount and lxc.mount.entry at the same time
lxc.mount = $containers_root/$container_name/lxc.fstab
#lxc.mount.entry=/path/to/host/shared/directory /path/to/container/rootfs/somedir none defaults,bind 0 0
lxc.cgroup.devices.deny = a
# /dev/null and zero
lxc.cgroup.devices.allow = c 1:3 rwm
lxc.cgroup.devices.allow = c 1:5 rwm
# consoles
lxc.cgroup.devices.allow = c 5:1 rwm
lxc.cgroup.devices.allow = c 5:0 rwm
lxc.cgroup.devices.allow = c 4:0 rwm
lxc.cgroup.devices.allow = c 4:1 rwm
# /dev/{,u}random
lxc.cgroup.devices.allow = c 1:9 rwm
lxc.cgroup.devices.allow = c 1:8 rwm
# /dev/pts/* - pts namespaces are "coming soon"
lxc.cgroup.devices.allow = c 136:* rwm
lxc.cgroup.devices.allow = c 5:2 rwm
# rtc
lxc.cgroup.devices.allow = c 254:0 rwm
#lxc.network.hwaddr = a1:b2:c3:d4:e5:f6
# cpu 
lxc.cgroup.cpu.shares = 2
# cpuset, eg = 0-2,14,15
lxc.cgroup.cpuset.cpus = 0-1
#lxc.cgroup.cpuset.mems = 0-1
# memory 
# NOTE: set limit_in_bytes before memsw.limit_in_bytes
lxc.cgroup.memory.soft_limit_in_bytes = 300M
lxc.cgroup.memory.limit_in_bytes = 1000M
lxc.cgroup.memory.memsw.limit_in_bytes = 2000M


EOF
echo "create lxc conf done"
}

create_container_virt_dev () {
config_script="virt_dev_config"
echo "create container virtual devices"
cat > $containers_root/$container_name/$config_script << EOF
ROOT=\$(pwd)
DEV=\${ROOT}/dev
if [ \$ROOT = '/' ]; then
printf "033[22;35mnDO NOT RUN ON THE HOST NODEnn"
tput sgr0
exit 1
fi
if [ ! -d \$DEV ]; then
printf "033[01;33mnRun this script in rootfsnn"
tput sgr0
exit 1
fi
rm -rf \${DEV}
mkdir \${DEV}
mknod -m 666 \${DEV}/null c 1 3
mknod -m 666 \${DEV}/zero c 1 5
mknod -m 666 \${DEV}/random c 1 8
mknod -m 666 \${DEV}/urandom c 1 9
mkdir -m 755 \${DEV}/pts
mkdir -m 1777 \${DEV}/shm
mknod -m 666 \${DEV}/tty c 5 0
mknod -m 666 \${DEV}/tty0 c 4 0
mknod -m 666 \${DEV}/tty1 c 4 1
mknod -m 666 \${DEV}/tty2 c 4 2
mknod -m 666 \${DEV}/tty3 c 4 3
mknod -m 666 \${DEV}/tty4 c 4 4
mknod -m 600 \${DEV}/console c 5 1
mknod -m 666 \${DEV}/full c 1 7
mknod -m 600 \${DEV}/initctl p
mknod -m 666 \${DEV}/ptmx c 5 2

exit 0

EOF

chmod +x $containers_root/$container_name/$config_script
# change to rootfs to run device config script
cd $containers_root/$container_name/$lxc_rootfs 
sh $containers_root/$container_name/$config_script
rm -f $containers_root/$container_name/$config_script
echo "create container virtual devices done"
}

config_rsa_auth() {
if [ ! -f /root/.ssh/id_rsa ]; then
  ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
fi
if [ ! -f /root/.ssh/id_rsa.pub ]; then
  ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
fi
rootfs=$containers_root/$container_name/$lxc_rootfs
mkdir -p $rootfs/root/.ssh
cat /root/.ssh/id_rsa.pub >> $rootfs/root/.ssh/authorized_keys
# 
if ! grep -E "^[:space:]*RSAAuthentication[:space:]+yes[:space:]*$" $rootfs/etc/ssh/sshd_config; then
	echo "RSAAuthentication yes" >> $rootfs/etc/ssh/sshd_config
fi
if ! grep -E "^[:space:]*AuthorizedKeysFile[:space:]+\.ssh/authorized_keys[:space:]*$" $rootfs/etc/ssh/sshd_config; then
	echo "AuthorizedKeysFile .ssh/authorized_keys" >> $rootfs/etc/ssh/sshd_config
fi
}
config_guest() {
echo "config guest"

rootfs=$containers_root/$container_name/$lxc_rootfs
# remove fstab 
rm -f $rootfs/etc/fstab
# remove nic config 
rm -f $rootfs/etc/sysconfig/network-scripts/ifcfg-eth0
# set rc.sysinit
cat > $rootfs/etc/rc.d/rc.sysinit << EOF
#!/bin/bash
rm -f \$(find /var/run -name '*pid')
rm -f /var/lock/subsys/*
# Edit gateway address, domain, and nameserver as need be
route add default gw $container_gw

# Initally we don't have any container originated mounts
rm -f /etc/mtab
touch /etc/mtab
EOF

if [ -f /etc/resolv.conf ]; then
  cp /etc/resolv.conf $rootfs/etc
fi

#echo "10.3.18.235 fmn.rrimg.com fmn.rrfmn.com fmn.xnimg.cn fmn.xnpic.com tel.fmn.rrimg.com uni.fmn.rrimg.com edu.fmn.rrimg.com tel.fmn.rrfmn.com uni.fmn.rrfmn.com edu.fmn.rrfmn.com" >> $rootfs/etc/hosts
#echo "10.3.18.235 hdn.xnimg.cn" >> $rootfs/etc/hosts
warnecho "**** config containers hosts if necessary ****"

cat > $rootfs/etc/sysconfig/network << EOF
NETWORKING=yes
HOSTNAME=$container_name
EOF

chroot $rootfs /bin/bash -c "chmod +x /etc/rc.d/rc.sysinit"
# set guest root password
chroot $rootfs /bin/bash -c "echo -e \"root\nroot\" | (passwd --stdin root)"
# close services
# close auditd service
chroot $rootfs /bin/bash -c "chkconfig auditd off 2>/dev/null"
# close httpd service
chroot $rootfs /bin/bash -c "chkconfig httpd off 2>/dev/null"

echo "config guest done"
}

check_container_exists() {
lxc-ls $container_name 2>/dev/null | grep "\b"$container_name"\b" >/dev/null
if [ $? -eq 0 ]; then
	cecho "Container "\"$container_name\"" has already existed"
	exit $err_container_has_exist
fi
# check guest ip, host port
grep $container_ip $containers_info
if [ $? -eq 0 ]; then
	cecho "ip "$container_ip" has been used"
	echo "used ips:"
	cat $containers_info | awk '{print $2}'
	exit $err_ip_used
fi
# check host port
cat $containers_info | awk '{print $3}' | grep "\b"$host_port"\b"
if [ $? -eq 0 ]; then
	cecho "host port "$host_port" has been used"
	cat $containers_info
	exit $err_host_port_used
fi

}

extract_container_info() {
info=`grep "\b"$container_name"\b" $containers_info`
if [ $? -ne 0 ]; then
	cecho "container \""$container_name"\" does not exist"
	return `expr 256 - $err_container_not_exist`
fi
info=($info)
host_port=${info[2]}
container_ip=${info[1]}
return 0
}

lxc_create_container() {
echo "create container: name: "$container_name", ip: "$container_ip", host port: "$host_port
install_prerequisite
check_container_exists
create_rootfs
create_lxc_fstab
create_lxc_conf
create_container_virt_dev
config_rsa_auth
config_guest
# create
lxc-create -n $container_name -f $containers_root/$container_name/$lxc_conf
# add container info to it
echo $container_name" "$container_ip" "$host_port >> $containers_info
}
lxc_start_container() {
if
extract_container_info
then
iptables -t nat -A PREROUTING -p tcp --dport ${host_port} -j DNAT --to ${container_ip}:${guest_port}
iptables -I FORWARD -d ${container_ip}/32 -p tcp -m state --state NEW -m tcp --dport ${guest_port} -j ACCEPT
lxc-start -n $container_name -d
else
echo "start container "$container_name" failed"
exit $err_container_not_exist
fi
}

lxc_stop_container() {
lxc-stop -n $container_name
# config iptable
if
extract_container_info
then
iptables -t nat -D PREROUTING -p tcp --dport ${host_port} -j DNAT --to ${container_ip}:${guest_port}
iptables -D FORWARD -d ${container_ip}/32 -p tcp -m state --state NEW -m tcp --dport ${guest_port} -j ACCEPT
fi
}

lxc_destroy_container() {
lxc-destroy -n $container_name
rm -rf $containers_root/$container_name
# remove container info
sed -i -e "/^"$container_name" /d" $containers_info
}

# main entry
if [ $# -eq 0 ]; then
	usage
fi

if ! options=$(getopt -o n:i:h -l create,start,stop,destroy,help,remote-download,name:,ip:,host-port: -- "$@")
then 
	exit 1
fi
set -- $options

action=none
while [ $# -gt 0 ]
do
	case $1 in
		--create) action=create;;
		--start) action=start;;
		--stop) action=stop;;
		--destroy) action=destroy;;
		--remote-download) remote_download=true;;
		--host-port) host_port=`echo $2 | tr -d "'"`; shift;;
		#-n|--name) container_name=$2; shift;;
		-n|--name) container_name=`echo $2 | tr -d "'"`; shift;;
		-i|--ip) container_ip=`echo $2 | tr -d "'"`; shift;;
		-h|--help) usage;;
		(--) shift; break;;
		(-*) echo "$0: error - unrecognized option $1" 1>&2; exit 1;;
		(*) break;;
	esac
	shift
done

case $action in
	create) lxc_create_container;;
	start) lxc_start_container;;
	stop) lxc_stop_container;;
	destroy) lxc_destroy_container;;
	*) usage; break;
esac


