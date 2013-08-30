// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: meixuancao@yunrang.com (Meixuan Cao)

#include "third_part/release/public/deploy.h"
#include <map>
#include <string>

namespace deploy {

using std::map;
using std::string;

class DeploymentRegistry {
 public:
  void RegisterDeployment(const string&, const string&);
  static DeploymentRegistry* GlobalRegistry();

 private:
  friend string GetReleasePath(const string&);
  static DeploymentRegistry* global_registry_;
  map<string, string> deploy_item_;
  DeploymentRegistry() {}
  DeploymentRegistry(const DeploymentRegistry&);
  DeploymentRegistry& operator=(const DeploymentRegistry&);
};

DeploymentRegistry* DeploymentRegistry::global_registry_ = NULL;

DeploymentRegistry* DeploymentRegistry::GlobalRegistry() {
  if (!global_registry_) {
    global_registry_ = new DeploymentRegistry;
  }
  return global_registry_;
}

void DeploymentRegistry::RegisterDeployment(const string& binary,
                                            const string& path) {
  if (deploy_item_.count(binary)) {
    return;
  }
  deploy_item_[binary] = path;
}

void RegisterDeployment(const string& release_path,
                        const string& build_target_path) {
  int sep_index = build_target_path.rfind('/');
  if (sep_index < 0) {
    return;
  }
  string binary_name = build_target_path.substr(sep_index + 1);
  DeploymentRegistry::GlobalRegistry()->RegisterDeployment(binary_name,
                                                           release_path);
}

string GetReleasePath(const string& binary) {
  DeploymentRegistry::GlobalRegistry();
  map<string, string>::iterator iter =
    DeploymentRegistry::global_registry_->deploy_item_.find(binary);
  string release_path("");
  if (DeploymentRegistry::global_registry_->deploy_item_.end() != iter) {
    release_path = iter->second;
  }
  return release_path;
}
}
