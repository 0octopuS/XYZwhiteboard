
#include "action.pb.h"
#include "packet.hpp"
#include "packet.pb.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <utility>

void WhiteboardPacket::new_temp_id_response(bool success, uint32_t user_id) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_temp_id_response()\n");
#endif
  protobuf::PacketAction action;
  protobuf::TempIDResponse *tempid_response = action.mutable_tempidresponse();
  tempid_response->set_user_id(user_id);
  tempid_response->set_success(success);
  new_packet(action);
}

void WhiteboardPacket::new_action_response(bool success, std::string msg) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_action_response()\n");
#endif
  protobuf::PacketAction action;
  protobuf::ActionResponse *action_response = action.mutable_actionresponse();
  action_response->set_message(msg);
  action_response->set_success(success);
#ifndef NDEBUG
  printf("  >>> action_response: %s\n", action_response->message().c_str());
  // printf(">>> create_whiteboard_request: %s\n", s.DebugString());
#endif
  new_packet(action);
}

void WhiteboardPacket::new_broadcast(protobuf::BroadCast broadcast_msg) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_broadcast()\n");
#endif
  protobuf::PacketAction action;
  auto packet_broadcast = action.broadcast();
  packet_broadcast = std::move(broadcast_msg);
  // std::swap(packet_broadcast, &broadcast_msg);
  new_packet(action);
}