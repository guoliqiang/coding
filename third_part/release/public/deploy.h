// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#ifndef UTIL_RELEASE_DEPLOY_DEPLOY_H_
#define UTIL_RELEASE_DEPLOY_DEPLOY_H_

#include <string>
#include <map>

namespace deploy {

void RegisterDeployment(const std::string&, const std::string&);
std::string GetReleasePath(const std::string&);
}

#define DECLARE_DEPLOY_BINARY(release_path, build_target_path) \
  deploy::RegisterDeployment(#release_path, #build_target_path);
#define DECLARE_DEPLOY_COPY(release_path, copy_item_path)
// Sample usage:
// DECLARE_DEPLOY_BUILD_COPY(frontend/js, php/wwwroot, php/js)
// means that "snake t=php/wwwroot" will be executed and
// ".snake/(build_strategy)/php/js" will be coped into "frontend/js"
// in the release path during installation.
#define DECLARE_DEPLOY_BUILD_COPY(\
    release_path, build_target_path, copy_source_path)
#define DECLARE_OWNER(owner)
#define DECLARE_PACKAGE
#define MKDIR(path)
#define DECLARE_INIT_SCRIPT_PRE_INSTALL(path)
#define DECLARE_INIT_SCRIPT_POST_INSTALL(release_path, source_path)

#endif  // UTIL_RELEASE_DEPLOY_DEPLOY_H_
