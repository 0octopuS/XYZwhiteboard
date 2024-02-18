#include "action.pb.h"
#include "packet.hpp"
#include <cstdint>
#include <cstdio>
#include <google/protobuf/arena.h>

void WhiteboardPacket::new_create_whiteboard_request(uint32_t user_id) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_create_whiteboard_request()\n");
#endif
  whiteboard::PacketAction action;
  // type = WhiteboardPacketType::createWhiteboard;
  whiteboard::CreateWhiteBoardRequest *create_whiteboard_request =
      action.mutable_createwhiteboard();
  create_whiteboard_request->set_user_id(user_id);
#ifndef NDEBUG
  printf(">>> create_whiteboard_request: %d\n",
         create_whiteboard_request->user_id());
  // printf(">>> create_whiteboard_request: %s\n", s.DebugString());
#endif
  new_packet(action);
}

void WhiteboardPacket::new_add_element_request(
    const whiteboard::AddElementRequest &request) {
  // type = WhiteboardPacketType::addElement;

  // addElementRequest = request;
}
