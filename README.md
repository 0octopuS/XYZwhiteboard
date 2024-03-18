# Whiteboard Protocol - Group F

The Whiteboard Protocol is a communication protocol designed for collaborative whiteboard applications. It facilitates real-time drawing, annotation, and interaction between multiple clients connected to a shared whiteboard server.



## Overview
The Whiteboard Protocol defines a set of rules and conventions for exchanging messages between clients and the server. It enables clients to:

- Draw shapes and lines on the whiteboard.
- Add text annotations.
- Select, move, and resize objects.
- Undo and redo actions.
- Collaborate with other users in real-time.
- The protocol is built on top of Google's Protocol Buffers (protobuf), providing efficient serialization and deserialization of messages over the network.


## Base

### Element.hpp

Define `WhiteboardElement` class for standard APIs for creating and parsing Element.

| Element type | Create method | To protobuf | Print | Test |
| ------------ | ------------- | ----------- | ----- | ---- |
| Path         | ✅             | ✅           | ✅     | ✅    |
| Line         | ✅             | ✅           | ✅     | ✅    |
| Circle       | ✅             | ✅           | ✅     | ✅    |
| Triangle     | ✅             | ✅           | ✅     | ✅    |
| Square       | ✅             | ✅           | ✅     | ✅    |
| Text         | ✅             | ✅           | ✅     | ✅    |
| Picture      |               |             |       |      |
| StickyNote   | ✅             | ✅           | ✅     | ✅    |
| Null         |               |             | ✅     | ✅    |
|              |               |             |       |      |

### Action

| Action           | Server logic | Client logic | Packet logic | Test |
| ---------------- | ------------ | ------------ | ------------ | ---- |
| createWhiteboard | ✅            | ✅            | ✅            |      |
| createSession    | ✅            | ✅            | ✅            |      |
| JoinSession      | ✅            | ✅            | ✅            |      |
| quitSession      | ✅            | ✅            | ✅            |      |
| addElement       |              |              |              |      |
| modifyElement    |              |              |              |      |
| deleteElement    |              |              |              |      |
| login            | ✅            | ✅            | ✅            |      |
| register         | ✅            | ✅            | ✅            |      |
| saveWhiteboard   |              |              |              |      |
| actionResponse   |              |              |              |      |
| broadcast        |              |              |              |      |
| tempIdResponse   | ✅            | ✅            |              |      |