#include "action.pb.h"
#include "element.pb.h"
#include "packet.hpp"
#include <cstdint>
#include <cstdio>
#include <google/protobuf/arena.h>
#include <sys/types.h>

void WhiteboardPacket::new_create_whiteboard_request(uint32_t user_id) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_create_whiteboard_request()\n");
#endif
  protobuf::PacketAction action{};
  // type = WhiteboardPacketType::createWhiteboard;
  protobuf::CreateWhiteBoardRequest *create_whiteboard_request =
      action.mutable_createwhiteboard();
  create_whiteboard_request->set_user_id(user_id);
#ifndef NDEBUG
  printf(">>> create_whiteboard_request: %d\n",
         create_whiteboard_request->user_id());
  // printf(">>> create_whiteboard_request: %s\n", s.DebugString());
#endif
  new_packet(action);
}

void WhiteboardPacket::new_create_session_request(uint32_t user_id) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_create_whiteboard_request()\n");
#endif
  protobuf::PacketAction action;
  // type = WhiteboardPacketType::createWhiteboard;
  protobuf::CreateSessionRequest *create_session_request =
      action.mutable_createsession();
  create_session_request->set_user_id(user_id);
#ifndef NDEBUG
  printf(">>> create_whiteboard_request: %d\n",
         create_session_request->user_id());
  // printf(">>> create_whiteboard_request: %s\n", s.DebugString());
#endif
  new_packet(action);
}

void WhiteboardPacket::new_quit_session_request(uint32_t user_id) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_quit_session_request()\n");
#endif
  protobuf::PacketAction action;
  // type = WhiteboardPacketType::createWhiteboard;
  protobuf::QuitSessionRequest *quit_session_request =
      action.mutable_quitsession();
  quit_session_request->set_user_id(user_id);
#ifndef NDEBUG
  printf(">>> quit_session_request: %d\n", quit_session_request->user_id());
  // printf(">>> create_whiteboard_request: %s\n", s.DebugString());
#endif
  new_packet(action);
}

void WhiteboardPacket::new_add_element_request(WhiteboardElements _element) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_quit_session_request()\n");
#endif
  protobuf::PacketAction action;
  protobuf::AddElementRequest *add_element_request =
      action.mutable_addelement();
  WhiteboardElements element;
  auto proto_element = element.get_protobuf();
  // WhiteboardElementsType _ele_type = _element.get_type();
  add_element_request->set_allocated_element(&proto_element);
  // type = WhiteboardPacketType::addElement;
  // addElementRequest = request;
}
