#!/bin/bash
chmod 000 /data/data/com.tencent.tmgp.sgame/files/tss_tmp/
echo -e '\033[34;;1m << 防闪退已开启 >> \033[0m'
echo -e '\033[1;43;m 进行中  \033[0m'
iptables -t nat -I PREROUTING -m string --string "cs.mbgame.gamesafe.qq.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables -t nat -I PREROUTING -m string --string "cschannel.anticheatexpert.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables -t nat -I PREROUTING -m string --string "ipv6.mainconn.gamesafe.qq.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables -t nat -I PREROUTING -m string --string "nj.cschannel.anticheatexpert.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables -t nat -I PREROUTING -m string --string "cs.mainconn.gamesafe.qq.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables -t nat -I PREROUTING -m string --string "cs.mbgame.anticheatexpert.com
" --algo bm -j DNAT --to-destination 192.168.1.4
iptables  -t nat -A PREROUTING -p udp  --dport 10012 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p tcp  --dport 10012 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p udp  --dport 5692 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p tcp  --dport 5692 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p udp  --dport 80 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p tcp  --dport 80 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p udp  --dport 443 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p tcp  --dport 433 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p udp  --dport 17500 -j REDIRECT --to-ports 8080
iptables  -t nat -A PREROUTING -p tcp  --dport 17500 -j REDIRECT --to-ports 8080
echo -e '\033[1;43;m 完成本地转向，开始循环  \033[0m'
while
do
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/taiq/lua/
#rm -rf /storage/emulated/0/Android/obj
#rm -rf /storage/emulated/0/Android/datan
#rm -rf /storage/emulated/0/Android/obb
#rm -rf /storage/emulated/0/Android/media
#rm -rf /data/data/com.tencent.tmgp.sgame/app_webview/Default/Cookies
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Assets3/apollo_uuid_define.json 
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Assets3/filelist.json 
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/v.splash
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloudSDKLog/GCloud/*
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/Cache/Log/*
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/traceids.store
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloud.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/RemoteConfig.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GameJoyRecorder/logs/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Assets3/apollo_uuid_define.json 
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Assets3/filelist.json 
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/v.splash
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloudSDKLog/GCloud/*
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/Cache/Log/*
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/traceids.store
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloud.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/RemoteConfig.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GameJoyRecorder/logs/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloudSDKLog/GCloud/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/Cache/Log/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GameJoyRecorder/logs/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/sdcard/tencent/Midas/Log/com.tencent.tmgp.sgame/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/sdcard/tencent/msflogs/com/tencent/mobileqq/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/sdcard/tencent/TPush/Logs/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/sdcard/tencent/XG/Logs/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/sdcard/Download/cn.wsds.game.data/node_dump/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/cookies/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/midas/log/com.tencent.tmgp.sgame/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/UnityShaderCache/
#rm -rf /data/data/com.tencent.tmgp.sgame/app_bugly/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/tbslog/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/program/
#rm -rf /storage/emulated/0/tencent/XG/Logs/
#rm -rf /storage/emulated/0/tencent/tbs_live_log/com.aigz.cloudgame.cat/
#rm -rf /storage/emulated/0/tencent/tbs_live_log/com.tencent.tmgp.sgame/
#rm -rf /storage/emulated/0/tencent/tbs_live_log/com.huati/
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/tencent
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/cookies
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Pandora/logs
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/GameJoyRecorder
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/midas/log
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Apollo
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/ChatRecord
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/UnityShaderCache
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloud.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/RemoteConfig.config
#rm -rf /storage/emulated/0/Android/data/com.tencent.tbsad
#rm -rf /storage/emulated/0/Android/data/.um
#rm -rf /data/data/com.tencent.tmgp.sgame/app_crashrecord
#rm -rf /data/data/com.tencent.tmgp.sgame/databases/sdk_report.db
#rm -rf /data/data/com.tencent.tmgp.sgame/app_webview/Default/LocalStorage/leveldb
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/UnityShaderCache/version
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/*.config
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Replay/*.abc
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/Replay/AutoSave/*.abc
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/tbslog/*.txt
#rm -rf /system 
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/GCloudSDKLog/GCloud/*.xlog
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/cache/uil-images/*
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/ChatRecord/*_1
#rm -rf /system 
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/QtsVFSCache/*.log
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/GameJoyRecorder/mgc/plugin-log/*.log
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/GameJoyRecorder/mmcodec/*.txt
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/GameJoyRecorder/plugin-logs/*
#rm -rf filedir /storage/emulated/0/Android/data/com.tencent.tmgp.sgame/files/logo/*
echo 16384 > /proc/sys/fs/inotify/max_queued_events
echo 128 > /proc/sys/fs/inotify/max_user_instances
echo 8192 > /proc/sys/fs/inotify/max_user_watches
echo -e '\033[1;43;m 循环删除中  \033[0m'
echo -e '[34;;1m << 别运行格机别找我 >> [0m';
sleep 10
done
#rm -rf /system '\033[1;43;m 循环删除中  \033[0m'

