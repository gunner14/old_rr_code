#/bin/sh

function usage()
{
  echo usage v1.11:
  echo "  main function:"
  echo "    ./dep.sh -l \"cd /data && ls\"                 lunch script on remote hosts"
  echo "    ./dep.sh -p xxx.sh                             lunch xxx.sh on remote hosts"
  echo "    ./dep.sh localdir :remotedir                   only copy file to remote host, prefix : for remote host"
  echo "    ./dep.sh -n :remotedir localdir                reduce file from remote hosts, prefix : for remote host, -n autorename"
  echo "  some property:"
  echo "    ./dep.sh -o logfile                            specify logfile for script output"
  echo "    ./dep.sh -u root                               specify user account,default root account"
  echo "    ./dep.sh -f hosts                              specify file for all target hosts,default file name hosts"
  echo "    ./dep.sh -s \"10.22.206.180 10.22.206.190\"    specify target hosts "
  echo "    ./dep.sh                                       show this page"
  exit 0
}

#TARGET_HOST=$(for (( i = 181; i < 200; i++ )); do echo "10.22.206."$i ; done;)

if [ $# -eq 0 ] || [ $1 = "help" ] ; then
  usage
fi

while getopts l:p:rf:s:u:o:hn opt ; do
  case $opt in
    l)  script=$OPTARG;;
    p)  script_file=$OPTARG;;
    u)  account=$OPTARG;;
    f)  TARGET_HOST=`cat $OPTARG`;;
    s)  TARGET_HOST=$OPTARG;;
    o)  LOG_FILE=$OPTARG;;
    n)  AUTO_REN=1;;
    h|\?) usage;;
  esac
done

if [ -z "$account" ] ; then
  echo "<> default specify root account"
  account="root"
fi

if [ -z "$TARGET_HOST" ] ; then
  if [ ! -f hosts ] ; then
    echo "<> default specify hosts file no exist"
    usage
  fi
  TARGET_HOST=`cat hosts`
fi

if [ -n "$script_file" ] && [ ! -f $script_file ]; then
  echo "<> specify $script_file no exist"
  usage
fi

if [ $OPTIND -lt $# ] ; then
  
  shift `expr $OPTIND - 1`
  newcount=1
  newcount0=1
  for x in $* ; do
    if [ $newcount0 -eq $# ]; then
      dst=$x
      if [ `expr index $x ":"` != "1" ]; then
        reduce=1
      fi
    fi
    newcount0=`expr $newcount0 + 1`
  done

  for x in $* ; do
    if [ $newcount -eq $# ]; then
      dst=$x
    else
      if [ -z reduce ] && [ ! -e $x ]; then
        echo "<> src $x no exist!"
        usage
      fi
      src="$src $x"
    fi
    newcount=`expr $newcount + 1`
  done

  for i in $TARGET_HOST ; do
    if [ -n "$reduce" ]; then
      for x in $src ; do
        if [ -z "$AUTO_REN" ]; then
          echo "<> scp -r $account@$i$x $dst"
          scp -r -o "StrictHostKeyChecking no" $account@$i$x $dst 
        else
          echo "<> scp -r $account@$i$x $dst-$i"
          scp -r -o "StrictHostKeyChecking no" $account@$i$x $dst.$i 
        fi 
      done
    else
      echo "<> scp -r $src $account@$i$dst"
      scp -r -o "StrictHostKeyChecking no" $src $account@$i$dst
    fi
  done
fi

if [ -n "$LOG_FILE" ] ; then
  logfile=" > $LOG_FILE"
else
  logfile=""
fi

if [ -n "$script" ] ; then
  for i in $TARGET_HOST ; do
    #env="_localhost=$i _hostindex=$idx "
    echo "<> sh $script on $i ...."
    #echo -e "IPADDR=${i}\n" | cat - $script | ssh -o "StrictHostKeyChecking no" $account@${i}
    ssh -T -o "StrictHostKeyChecking no" $account@$i $script
  done
fi

if [ -n "$script_file" ] ; then
  declare -i size=0
  for i in $TARGET_HOST ; do
    size=size+1
  done
  idx=0
  for i in $TARGET_HOST ; do
    env="dep_local=$i dep_index=$idx  dep_size=$size mytest=who_you_continue_who_you"
    echo "<> on $i run"
    #scp -o "StrictHostKeyChecking no" $script_file $account@$i:/tmp/$script_file
    #ssh -T -o "StrictHostKeyChecking no" $account@$i $env sh /tmp/$script_file
    echo -e "ip=$i\nidx=$idx\n" | cat - $script_file | ssh -T -o "StrictHostKeyChecking no" $account@$i
    idx=`expr $idx + 1`
  done
fi
