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
#include "aprinitializer.h"
#include "third_part/apr/include/apr-1/apr_pools.h"
#include "third_part/apr/include/apr-1/apr_atomic.h"
#include "third_part/apr/include/apr-1/apr_time.h"
#include <assert.h>

bool APRInitializer::isDestructed = false;

APRInitializer::APRInitializer() {
    apr_initialize();
    apr_pool_create(&p, NULL);
    apr_atomic_init(p);
    startTime = apr_time_now();
}

APRInitializer::~APRInitializer() {
  apr_terminate();
  isDestructed = true;
}

APRInitializer& APRInitializer::getInstance() {
  static APRInitializer init;
  return init;
}

apr_pool_t* APRInitializer::getRootPool() {
    return getInstance().p;
}
