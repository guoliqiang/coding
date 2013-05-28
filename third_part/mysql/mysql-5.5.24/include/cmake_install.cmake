# Install script for directory: /home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/guoliqiang/git-coding/coding/third_part/mysql")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql_com.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql_time.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_list.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_alloc.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/typelib.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql/plugin.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql/plugin_audit.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql/plugin_ftparser.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_dbug.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/m_string.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_sys.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_xml.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql_embed.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_pthread.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/decimal.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/errmsg.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_global.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_net.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_getopt.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/sslopt-longopts.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_dir.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/sslopt-vars.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/sslopt-case.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/sql_common.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/keycache.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/m_ctype.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_attribute.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_compiler.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql_version.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/my_config.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysqld_ername.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysqld_error.h"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/sql_state.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/include/mysql/" FILES_MATCHING REGEX "/[^/]*\\.h$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

