#!/bin/sh
. /etc/arch.sh
service_list_file=/usr/share/service_list.txt
cron_new_file=/usr/share/cron_upgrade
kill -9 $(pidof watchdog) ; /usr/sbin/watchdog -c /etc/watchdog.upgrade
for servicename in $(cat $service_list_file); do
  [ -n "$(pidof $servicename)" ] && {
    echo "stop $servicename with pid $(pidof $servicename)"
    kill -9 $(pidof $servicename)
  }
done
remove_file_list="/sbin/server-detect /etc/init.d/ddns /etc/init.d/aximddns /sbin/ddns-detect /etc/init.d/ntpclient"
for remove_file in $remove_file_list; do
    echo "rm $remove_file"
    rm $remove_file
done
arch_led_start boot
# umount_drive_list="/tmp/mnt/SD /tmp/mnt/USB"
# for umount_drive in $umount_drive_list; do
#     echo "umount -l $umount_drive"
#     umount -l $umount_drive
# done

