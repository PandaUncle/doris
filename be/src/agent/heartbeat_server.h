// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <mutex>

#include "common/status.h"
#include "gen_cpp/HeartbeatService.h"
#include "olap/olap_define.h"
#include "runtime/exec_env.h"

namespace doris {

const uint32_t HEARTBEAT_INTERVAL = 10;
class StorageEngine;
class Status;
class ThriftServer;

class HeartbeatServer : public HeartbeatServiceIf {
public:
    explicit HeartbeatServer(TMasterInfo* master_info);
    ~HeartbeatServer() override = default;

    virtual void init_cluster_id();

    // Master send heartbeat to this server
    //
    // Input parameters:
    // * master_info: The struct of master info, contains host ip and port
    //
    // Output parameters:
    // * heartbeat_result: The result of heartbeat set
    void heartbeat(THeartbeatResult& heartbeat_result, const TMasterInfo& master_info) override;

    static const int max_be_exec_version;
    static const int min_be_exec_version;
    static int be_exec_version;

    static bool check_be_exec_version(int be_exec_version) {
        if (be_exec_version > max_be_exec_version || be_exec_version < min_be_exec_version) {
            LOG(WARNING) << fmt::format(
                    "Received be_exec_version is not supported, be_exec_version={}, "
                    "min_be_exec_version={}, max_be_exec_version={}, maybe due to FE version not "
                    "match "
                    "with BE.",
                    be_exec_version, min_be_exec_version, max_be_exec_version);
            return false;
        }
        return true;
    }

private:
    Status _heartbeat(const TMasterInfo& master_info);

    StorageEngine* _olap_engine;
    int64_t _be_epoch;

    // mutex to protect master_info and _epoch
    std::mutex _hb_mtx;
    // Not owned. Point to the ExecEnv::_master_info
    TMasterInfo* _master_info;
    int64_t _fe_epoch;

    DISALLOW_COPY_AND_ASSIGN(HeartbeatServer);
}; // class HeartBeatServer

Status create_heartbeat_server(ExecEnv* exec_env, uint32_t heartbeat_server_port,
                               ThriftServer** heart_beat_server, uint32_t worker_thread_num,
                               TMasterInfo* local_master_info);
} // namespace doris
