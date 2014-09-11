/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APRINITIALIZER_H
#define APRINITIALIZER_H

#include "pool.h"
#include "third_part/apr/include/apr-1/apr_pools.h"
#include "third_part/apr/include/apr-1/apr_thread_proc.h"

class APRInitializer {
  public:
    static apr_pool_t* getRootPool();
    static bool isDestructed;

  private:
    APRInitializer();
    APRInitializer(const APRInitializer&);
    APRInitializer& operator=(const APRInitializer&);
    apr_pool_t* p;
    apr_time_t startTime;
    static APRInitializer& getInstance();
 
  public:
    ~APRInitializer();
};
#endif  // APRINITIALIZER_H
