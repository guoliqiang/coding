# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. Example variables are:
#   CPACK_GENERATOR                     - Generator used to create package
#   CPACK_INSTALL_CMAKE_PROJECTS        - For each project (path, name, component)
#   CPACK_CMAKE_GENERATOR               - CMake Generator used for the projects
#   CPACK_INSTALL_COMMANDS              - Extra commands to install components
#   CPACK_INSTALL_DIRECTORIES           - Extra directories to install
#   CPACK_PACKAGE_DESCRIPTION_FILE      - Description file for the package
#   CPACK_PACKAGE_DESCRIPTION_SUMMARY   - Summary of the package
#   CPACK_PACKAGE_EXECUTABLES           - List of pairs of executables and labels
#   CPACK_PACKAGE_FILE_NAME             - Name of the package generated
#   CPACK_PACKAGE_ICON                  - Icon used for the package
#   CPACK_PACKAGE_INSTALL_DIRECTORY     - Name of directory for the installer
#   CPACK_PACKAGE_NAME                  - Package project name
#   CPACK_PACKAGE_VENDOR                - Package project vendor
#   CPACK_PACKAGE_VERSION               - Package project version
#   CPACK_PACKAGE_VERSION_MAJOR         - Package project version (major)
#   CPACK_PACKAGE_VERSION_MINOR         - Package project version (minor)
#   CPACK_PACKAGE_VERSION_PATCH         - Package project version (patch)

# There are certain generator specific ones

# NSIS Generator:
#   CPACK_PACKAGE_INSTALL_REGISTRY_KEY  - Name of the registry key for the installer
#   CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS - Extra commands used during uninstall
#   CPACK_NSIS_EXTRA_INSTALL_COMMANDS   - Extra commands used during install


SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_NSIS "")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "")
SET(CPACK_BINARY_STGZ "")
SET(CPACK_BINARY_TBZ2 "")
SET(CPACK_BINARY_TGZ "")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_GENERATOR "TGZ")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24;MySQL;ALL;/")
SET(CPACK_INSTALL_PREFIX "/home/guoliqiang/git-coding/coding/third_part/mysql")
SET(CPACK_MODULE_PATH "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/cmake")
SET(CPACK_MONOLITHIC_INSTALL "1")
SET(CPACK_NSIS_DISPLAY_NAME "MySQL 5.5.24")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_PACKAGE_NAME "MySQL 5.5.24")
SET(CPACK_OUTPUT_CONFIG_FILE "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/CPackConfig.cmake")
SET(CPACK_PACKAGE_CONTACT "MySQL Release Engineering <mysql-build@oss.oracle.com>")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MySQL built using CMake")
SET(CPACK_PACKAGE_FILE_NAME "mysql-5.5.24-linux-x86_64")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "MySQL 5.5.24")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "MySQL 5.5.24")
SET(CPACK_PACKAGE_NAME "MySQL")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "Oracle Corporation")
SET(CPACK_PACKAGE_VERSION "5.5.24")
SET(CPACK_PACKAGE_VERSION_MAJOR "5")
SET(CPACK_PACKAGE_VERSION_MINOR "5")
SET(CPACK_PACKAGE_VERSION_PATCH "24")
SET(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake-2.8/Templates/CPack.GenericLicense.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake-2.8/Templates/CPack.GenericWelcome.txt")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_IGNORE_FILES "\\.bzr/;\\.bzr-mysql;\\.bzrignore;CMakeCache\\.txt;cmake_dist\\.cmake;CPackSourceConfig\\.cmake;CPackConfig.cmake;/cmake_install\\.cmake;/CTestTestfile\\.cmake;/CMakeFiles/;/version_resources/;/_CPack_Packages/;$\\.gz;$\\.zip;/CMakeFiles/;/version_resources/;/_CPack_Packages/;scripts/make_binary_distribution$;scripts/msql2mysql$;scripts/mysql_config$;scripts/mysql_convert_table_format$;scripts/mysql_find_rows$;scripts/mysql_fix_extensions$;scripts/mysql_install_db$;scripts/mysql_secure_installation$;scripts/mysql_setpermission$;scripts/mysql_zap$;scripts/mysqlaccess$;scripts/mysqld_multi$;scripts/mysqld_safe$;scripts/mysqldumpslow$;scripts/mysqlhotcopy$;Makefile$;include/config\\.h$;include/my_config\\.h$;/autom4te\\.cache/;errmsg\\.sys$")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/guoliqiang/git-coding/coding/third_part/mysql/mysql-5.5.24/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "mysql-5.5.24")
SET(CPACK_SOURCE_TBZ2 "")
SET(CPACK_SOURCE_TGZ "")
SET(CPACK_SOURCE_TZ "")
SET(CPACK_SOURCE_ZIP "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux")
