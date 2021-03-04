/******************************************************************************
 * Copyright 2020 The ZhiTo Authors. All Rights Reserved.
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

#include "cyber/examples/timer_send_component/timer_send_03.h"
#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"
#include "cyber/examples/proto/examples.pb.h"

bool TimerComponentSender_03::Init() {
	driver_writer_ = node_->CreateWriter<TestPackage>("/group_03/channel_03");
	return true;
}

bool TimerComponentSender_03::Proc() 
{
	static int i = 0;
	static std::string data(8192,'a');
	auto out_msg = std::make_shared<TestPackage>();
	out_msg->set_seq(i++);
	out_msg->set_timestamp(Time::Now().ToNanosecond());
	out_msg->set_content(data);
	driver_writer_->Write(out_msg);
	AINFO << "timer_send_03  TestPackage";

	return true;
}

