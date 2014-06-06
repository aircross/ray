#!/bin/sh

reserv_link()
{
    local chkfolder="$1"
    local filterword="$2"
    local newtarget="$3"
    local chktgt
    for chktgt in $(ls $chkfolder); do
        [ -L "$chkfolder/$chktgt" -o -L "$chktgt" ] && {
            [ -n "$(ls -al $chkfolder/$chktgt | grep $filterword)" ] && {
                echo "do reserv for $chkfolder/$chktgt : ln -sf $newtarget $chkfolder/$chktgt"
                ln -sf $newtarget $chkfolder/$chktgt
            }
        }
    done
}

backup_link()
{
    local tpath="$1"
    local tfile
    local tdir
    for linkfs in $(find "$tpath" -type l); do
        tfile=$(basename $linkfs)
        tdir=$(dirname $linkfs)
        echo cd $tdir
        cd $tdir
        targfs="$(ls -al $linkfs | awk '{print $11}')"
        targfsbn="$(basename $targfs)"
        echo cp $targfs /tmp/$targfsbn
        cp $targfs /tmp/$targfsbn
        echo ln -s /tmp/$targfsbn ${linkfs}_1
        ln -s /tmp/$targfsbn ${linkfs}_1
        echo mv ${linkfs}_1 ${linkfs}
        mv ${linkfs}_1 ${linkfs}
        echo cd -
        cd -
    done
}

backup_one_file_with_link()
{
    local trycount=10
    local link_result=1
    local source_file="$1"
    local target_file="$2"
    [ ! -f "$source_file" ] && return
    cp $source_file $target_file

    while [ "$trycount" -gt "0" -a "$link_result" != "0" ]; do
        ln -sf $target_file $source_file
        link_result=$?
        [ "$link_result" = "0" ] && break
        sleep 1
        trycount=$(($trycount - 1))
    done
}

cp /bin/busybox /tmp/
mv /usr/sbin/nvram /tmp/
ln -s /tmp/nvram /usr/sbin/nvram
backup_one_file_with_link "/usr/sbin/httpd" "/tmp/httpd_cmd"
#reserv_link "/sbin" "busybox" "/tmp/busybox"
#reserv_link "/bin" "busybox" "/tmp/busybox"
#reserv_link "/usr/sbin" "busybox" "/tmp/busybox"
#reserv_link "/usr/bin" "busybox" "/tmp/busybox"
# backup_link "/usr"
backup_link "/usr/sbin"
backup_link "/usr/bin"
backup_link "/bin"
backup_link "/sbin"
# backup_link "/lib"
backup_link "/etc"

