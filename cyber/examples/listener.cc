/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "cyber/examples/proto/examples.pb.h"
#include "cyber/time/time.h"
#include "cyber/cyber.h"

using apollo::cyber::Time;

void MessageCallback(
	const std::shared_ptr<apollo::cyber::examples::proto::TestPackage>& msg) {
	
	google::protobuf::uint64 delt = Time::Now().ToNanosecond() - (msg->timestamp());
	Time t(delt);
	
	std::cout<<t.ToMicrosecond()<<std::endl;
	AINFO << "Received message seq-> " << msg->seq();
	//AINFO << "msgcontent->" << msg->content();
	//AINFO << "msg length->" << msg->content().length();  //1040 20480 30720 (B)
}

int main(int argc, char* argv[]) {
	apollo::cyber::Init(argv[0]);

	auto listener_node = apollo::cyber::CreateNode("listener_01");
	auto listener = listener_node->CreateReader<apollo::cyber::examples::proto::TestPackage>("/group_01/channel_01", MessageCallback);

	apollo::cyber::WaitForShutdown();

	return 0;
}
