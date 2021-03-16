/******************************************************************************
 * Copyright 2018 The ZhiTo Authors. All Rights Reserved.
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

#include "cyber/cyber.h"
#include "cyber/time/rate.h"
#include "cyber/time/time.h"

using apollo::cyber::Rate;
using apollo::cyber::Time;
using apollo::cyber::examples::proto::Chatter;
using apollo::cyber::examples::proto::TestPackage;


int main(int argc, char *argv[]) {
	double rete_param;
	std::string DATA_10KB(10240,'a');
	std::string DATA_20KB(20480,'a');
	std::string DATA_30KB(30720,'a');
	std::string DATA_100KB(102400,'a');
	
	std::cout<<argc<<std::endl;
	if(argc<2)
	{
		std::cout<<"Please select the rete to send the package"<<std::endl;
		return 0;
	} else {
		rete_param =  atof(argv[1]);
		std::cout<<"Select "<<rete_param<<" to send the package"<<std::endl;
	}
	
	// init cyber framework
	apollo::cyber::Init(argv[0]);
	// create talker node
	auto talker_node = apollo::cyber::CreateNode("talker_01");
	// create talker
	auto talker = talker_node->CreateWriter<Chatter>("channel/chatter_01");
	Rate rate(rete_param);
	uint64_t seq = 0;
	while (apollo::cyber::OK()) {
		auto msg = std::make_shared<Chatter>();
		msg->set_timestamp(Time::Now().ToNanosecond());
		msg->set_lidar_timestamp(Time::Now().ToNanosecond());
		msg->set_seq(seq);
		msg->set_content(DATA_10KB);
		talker->Write(msg);
		seq++;
		rate.Sleep();
	}
	return 0;
}
