# Install script for directory: /home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man

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

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ManPages")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql.server.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_tzinfo_to_sql.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_config.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/innochecksum.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_client_test.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/myisam_ftdump.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/replace.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlimport.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlcheck.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_client_test_embedded.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/msql2mysql.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_setpermission.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlman.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/my_print_defaults.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/comp_err.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_upgrade.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql-stress-test.pl.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqld_safe.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_plugin.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlaccess.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlhotcopy.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqladmin.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlbug.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_secure_installation.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/resolveip.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlslap.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql-test-run.pl.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqldumpslow.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_waitpid.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_install_db.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_fix_extensions.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/myisampack.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_zap.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqltest_embedded.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqltest.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_convert_table_format.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysql_find_rows.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/myisamlog.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqldump.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlshow.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/myisamchk.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/resolve_stack_dump.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqlbinlog.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqld_multi.1"
    "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/perror.1"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ManPages")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ManPages")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man8" TYPE FILE FILES "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/man/mysqld.8")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ManPages")

