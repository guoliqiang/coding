#
# export_vars.sh
#
# This shell script is used to export vars to the application using the
# APRUTIL library. This script should be "sourced" to ensure the variable
# values are set within the calling script's context. For example:
#
#   $ . path/to/apr-util/export_vars.sh
#

APRUTIL_EXPORT_INCLUDES="-I/home/guoliqiang/git-coding/coding/third_part/apr_util/apr-util-1.5.3/xml/expat/lib"
APRUTIL_EXPORT_LIBS="/home/guoliqiang/git-coding/coding/third_part/apr_util/apr-util-1.5.3/xml/expat/libexpat.la"
APRUTIL_LDFLAGS=""
