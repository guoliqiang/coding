# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-01-05 15:26:23

service ClusteringService {
  void SendHumanMerge(1:i64 cluster_id1, 2:i64 cluster_id2);
  void SendHumanDelete(1:i64 cluster_id);
  i32 IsAlive();
}

