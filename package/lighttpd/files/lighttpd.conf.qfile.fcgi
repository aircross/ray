# lighttpd configuration file
# 
## modules to load
# all other module should only be loaded if really neccesary
# - saves some time
# - saves memory
server.modules = ( 
	"mod_fastcgi", 
	"mod_auth", 
	"mod_alias", 
	"mod_webdav"
)
## Add the cgi module
# cgi.assign = (".cgi" => "")

## mimetype mapping
mimetype.assign = ( 
	".pdf"   => "application/pdf",
	".class" => "application/octet-stream",
	".pac"   => "application/x-ns-proxy-autoconfig",
	".swf"   => "application/x-shockwave-flash",
	".wav"   => "audio/x-wav",
	".gif"   => "image/gif",
	".jpg"   => "image/jpeg",
	".jpeg"  => "image/jpeg",
	".png"   => "image/png",
	".mpeg"  => "video/mpeg",
	".mpg"   => "video/mpeg",
	".mp4"   => "video/mpeg",
	".mov"   => "video/quicktime",
	".qt"    => "video/quicktime",
	".avi"   => "video/x-msvideo",
	".asf"   => "video/x-ms-asf",
	".asx"   => "video/x-ms-asf",
	".wmv"   => "video/x-ms-wmv",
	".mp3"   => "audio/mpeg",
	".css"   => "text/css",
	".html"  => "text/html",
	".htm"   => "text/html",
	".js"    => "text/javascript",
	".txt"   => "text/plain",
	".dtd"   => "text/xml",
	".xml"   => "text/xml"
)

# force use of the "write" backend (closes: #2401)
server.network-backend = "linux-sendfile"
server.event-handler = "linux-sysepoll"
fastcgi.debug = 1
fastcgi.server = (
  ".cgi" =>
  (( "socket" => "/tmp/tinyfcgi.socket",
     "bin-path" => "/etc/lighttpd/cgi-bin/stdenv.cgi",
     "bin-environment" => (
        "PHP_FCGI_CHILDREN" => "16",
        "PHP_FCGI_MAX_REQUESTS" => "10000" 
     ),
     "broken-scriptfilename" => "enable" ,
     "allow-x-send-file" => "enable"
  )),
)

#"bin-copy-environment" => ( "PATH", "SHELL", "USER" ),
## a static document-root, for virtual-hosting take look at the 
## server.virtual-* options
server.document-root = "/tmp/mnt"
dir-listing.encoding = "utf-8"

## to help the rc.scripts
server.pid-file = "/var/run/lighttpd.pid"

#### webdav
$HTTP["url"] =~ "^($|/)" {
 server.dir-listing = "enable"
 webdav.activate = "enable"
 alias.url = ( "/cgi-bin" => "/etc/lighttpd/cgi-bin" )
}
server.upload-dirs = (
 "/tmp/mnt/SD/.upload",
 "/tmp/mnt/USB/.upload",
)
