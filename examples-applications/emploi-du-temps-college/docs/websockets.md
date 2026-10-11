# WebSockets

Abystream provides WebSocket support for applications that require
persistent, bidirectional communication between a client and a server.

The WebSocket implementation is provided by the
`framework/websocket` module and uses the native WebSocket functions
provided by the Abystream native web shim.

---

## 1. Overview

A WebSocket connection begins as an HTTP request and is then upgraded
to a persistent WebSocket connection.

```text
Client
  │
  │ HTTP Upgrade request
  ▼
Abystream Server
  │
  ▼
WebSocket Handshake
  │
  │ HTTP 101
  ▼
WebSocket Connection
  │
  ├──────────────► Message
  │
  ◄────────────── Message
  │
  ├──────────────► Message
  │
  ▼
Connection closed
```

Unlike a normal HTTP request, a WebSocket connection can remain open
and exchange multiple messages.

---

## 2. WebSocket module

The implementation is located in:

```text
framework/
└── websocket/
    ├── websocket.mabc
    └── conf.toml
```

The module depends on:

```text
core
http
native webshim
```

The native WebSocket operations are provided by the web shim.

---

## 3. WebSocket upgrade

A WebSocket connection starts with an HTTP request containing an
upgrade request.

The WebSocket module provides:

```text
is_websocket_upgrade()
```

This function checks whether the request contains a WebSocket upgrade.

The implementation recognizes the `Upgrade` header with the value:

```text
websocket
```

including the corresponding case variation handled by the module.

Conceptually:

```text
HTTP Request
     │
     ▼
Upgrade: websocket
     │
     ▼
WebSocket upgrade detected
```

---

## 4. WebSocket handshake

Once an upgrade request has been detected, Abystream performs the
WebSocket handshake.

The main handshake function is:

```text
ws_handshake()
```

It receives:

```text
client_fd
request
```

The function obtains the client's:

```text
Sec-WebSocket-Key
```

and passes it to the native WebSocket implementation.

The native function:

```text
matlabc_ws_accept
```

computes the value required for the WebSocket handshake.

---

## 5. HTTP 101 response

After the handshake value has been generated, Abystream sends an HTTP
`101 Switching Protocols` response.

Conceptually:

```text
Client
  │
  │ GET /socket
  │ Upgrade: websocket
  │ Sec-WebSocket-Key: ...
  ▼
Abystream
  │
  │ Compute WebSocket acceptance
  ▼
HTTP 101 Switching Protocols
  │
  ▼
WebSocket connection
```

The HTTP upgrade therefore transitions the connection from HTTP
communication to WebSocket communication.

---

## 6. WebSocket serving

After the handshake, the main WebSocket processing function is:

```text
ws_serve()
```

Its handler receives a message and produces a response.

Conceptually:

```text
ws_serve(
    client_fd,
    handler
)
```

where the handler has the form:

```text
funcptr<(str)str>
```

The handler therefore receives a string and returns a string.

---

## 7. Message processing

The WebSocket server processes incoming frames in a loop.

The general flow is:

```text
WebSocket Frame
      │
      ▼
Receive data
      │
      ▼
Decode frame
      │
      ▼
Application handler
      │
      ▼
Encode response
      │
      ▼
Send frame
```

The native decoder is:

```text
matlabc_ws_decode
```

and the native encoder is:

```text
matlabc_ws_encode
```

---

## 8. WebSocket frame decoding

Incoming WebSocket frames are decoded by the native WebSocket
implementation.

The framework passes the received data to:

```text
matlabc_ws_decode
```

The decoded information includes the WebSocket frame payload and its
opcode.

The framework then determines how the frame should be handled.

---

## 9. Close frames

The WebSocket implementation recognizes the close opcode:

```text
8
```

When a close frame is received, the WebSocket serving loop terminates.

Conceptually:

```text
Incoming frame
      │
      ▼
   Opcode
      │
      ├── 8 ──► Close connection
      │
      └── Other ──► Process message
```

This allows the application to terminate the WebSocket session when
the client closes the connection.

---

## 10. Application handler

The application handler is called after a valid message has been
decoded.

Conceptually:

```text
Client
   │
   │ WebSocket message
   ▼
ws_serve()
   │
   ▼
Decode
   │
   ▼
handler(message)
   │
   ▼
response
   │
   ▼
Encode
   │
   ▼
Client
```

For a simple echo application, the handler can conceptually return the
same message it received:

```text
message → handler → message
```

For a real application, the handler can instead execute application
logic and generate a different response.

---

## 11. Message-oriented communication

WebSockets allow the application to maintain communication over the
same connection.

For example:

```text
Client                         Server
  │                              │
  │──── "hello" ────────────────►│
  │                              │
  │◄──── "hello" ────────────────│
  │                              │
  │──── "calendar" ─────────────►│
  │                              │
  │◄──── "updated" ──────────────│
  │                              │
```

The server does not need to create a new HTTP request for each
message.

---

## 12. Receive buffer

The current WebSocket implementation uses a fixed receive capacity:

```text
RECV_CAP = 8192
```

The received network data is therefore bounded by the configured
buffer size used by the implementation.

Conceptually:

```text
Network
   │
   ▼
Receive buffer
   │
   │ 8192 bytes
   ▼
Frame decoder
```

---

## 13. Payload capacity

The WebSocket implementation also defines:

```text
PAYLOAD_CAP = 8192
```

The decoded payload is checked against this capacity before it is
processed by the application handler.

This prevents an incoming frame from being passed to the application
when its payload exceeds the supported bounded payload size.

---

## 14. Oversized frames

The implementation validates the payload length after decoding.

Conceptually:

```text
Incoming frame
      │
      ▼
Decode
      │
      ▼
Payload length
      │
      ├── Within capacity ──► Handler
      │
      └── Too large ────────► Reject
```

The current implementation therefore uses bounded buffers and rejects
payloads that exceed the supported capacity.

---

## 15. Null termination

After receiving and validating the payload, the implementation
ensures that the resulting string is properly terminated before it is
passed to the application handler.

The resulting value can therefore be treated as a MATL-ABC string by
the application.

---

## 16. Response encoding

The string returned by the application handler is converted into a
WebSocket frame.

The framework uses:

```text
matlabc_ws_encode
```

to encode the response.

The complete processing chain is:

```text
Incoming WebSocket frame
          │
          ▼
matlabc_ws_decode
          │
          ▼
       Payload
          │
          ▼
   Application handler
          │
          ▼
     Response string
          │
          ▼
matlabc_ws_encode
          │
          ▼
Outgoing WebSocket frame
```

---

## 17. Sending the response

After the response frame has been encoded, Abystream sends it through
the client connection.

Conceptually:

```text
Handler
   │
   ▼
Response string
   │
   ▼
WebSocket encoder
   │
   ▼
Encoded frame
   │
   ▼
Socket
   │
   ▼
Client
```

The application therefore does not need to manually construct the
WebSocket frame format.

---

## 18. Native WebSocket layer

The high-level WebSocket implementation in MATL-ABC relies on native
functions.

The native library is part of:

```text
native/
├── headers/
├── libwebshim.so
└── webshim.c
```

The WebSocket-related native operations include:

```text
matlabc_ws_accept
matlabc_ws_decode
matlabc_ws_encode
```

This separates WebSocket protocol operations from the higher-level
Abystream API.

---

## 19. Architecture

The complete WebSocket architecture can be represented as:

```text
┌───────────────────────────────┐
│       Application             │
│                               │
│   WebSocket handler           │
└───────────────┬───────────────┘
                │
                ▼
┌───────────────────────────────┐
│       Abystream               │
│                               │
│   framework/websocket         │
│                               │
│   ws_handshake()              │
│   ws_serve()                  │
└───────────────┬───────────────┘
                │
                ▼
┌───────────────────────────────┐
│       Native Web Shim         │
│                               │
│   matlabc_ws_accept()         │
│   matlabc_ws_decode()         │
│   matlabc_ws_encode()         │
└───────────────┬───────────────┘
                │
                ▼
┌───────────────────────────────┐
│       Network Socket          │
└───────────────────────────────┘
```

---

## 20. WebSocket and HTTP

WebSocket communication begins with HTTP but does not remain an
ordinary HTTP request/response exchange.

```text
                HTTP
                 │
                 │ Upgrade
                 ▼
        WebSocket Handshake
                 │
                 ▼
          WebSocket Mode
                 │
       ┌─────────┴─────────┐
       ▼                   ▼
    Receive              Send
       │                   │
       ▼                   ▼
     Frame               Frame
```

The HTTP module is therefore involved during the initial connection,
while the WebSocket module handles subsequent frame-based
communication.

---

## 21. WebSocket and routing

A WebSocket endpoint can be associated with an application route.

The initial request can be identified through its HTTP path and
upgrade headers.

Conceptually:

```text
Client
  │
  │ GET /ws
  │ Upgrade: websocket
  ▼
HTTP Server
  │
  ▼
Routing
  │
  ▼
WebSocket endpoint
  │
  ▼
Handshake
  │
  ▼
WebSocket handler
```

This allows WebSocket communication to coexist with ordinary HTTP
routes in the same application.

---

## 22. WebSocket and templates

WebSockets and templates can be used together.

Templates can generate the initial user interface:

```text
HTTP Request
     │
     ▼
Template
     │
     ▼
HTML
```

The resulting page can then establish a WebSocket connection:

```text
HTML Page
    │
    ▼
WebSocket Connection
    │
    ▼
Real-time messages
```

For example, an application can render a calendar page and then use a
WebSocket connection to receive updates without reloading the page.

---

## 23. Example architecture

A complete real-time application can therefore be structured as:

```text
Browser
   │
   ├──────────── HTTP ────────────► Server
   │                                 │
   │                                 ▼
   │                              Router
   │                                 │
   │                                 ▼
   │                              Template
   │                                 │
   │◄──────────── HTML ──────────────┘
   │
   │
   └────────── WebSocket ──────────► Server
                                      │
                                      ▼
                                WebSocket Handler
                                      │
                                      ▼
                                Application Logic
                                      │
                                      ▼
                                WebSocket Response
                                      │
   ◄──────────────────────────────────┘
```

---

## 24. WebSocket lifecycle

The lifecycle of a WebSocket connection is:

```text
1. Client creates HTTP upgrade request
              │
              ▼
2. Abystream detects WebSocket upgrade
              │
              ▼
3. WebSocket handshake
              │
              ▼
4. HTTP 101 response
              │
              ▼
5. WebSocket connection established
              │
              ▼
6. Receive frame
              │
              ▼
7. Decode frame
              │
              ▼
8. Execute application handler
              │
              ▼
9. Encode response
              │
              ▼
10. Send response frame
              │
              │
              └──────► Repeat
                         │
                         ▼
11. Close frame
              │
              ▼
12. End connection
```

---

## 25. Current implementation characteristics

The current Abystream WebSocket implementation has the following
characteristics:

* HTTP-based WebSocket upgrade
* WebSocket handshake support
* native WebSocket acceptance calculation
* WebSocket frame decoding
* WebSocket frame encoding
* application message handlers
* close-frame handling
* bounded receive buffers
* bounded payload processing
* native WebSocket operations through `libwebshim.so`

The framework therefore keeps protocol-specific operations in the
native layer while exposing a higher-level API to MATL-ABC
applications.

---

## 26. Related documentation

* [Architecture](architecture.md)
* [Getting Started](getting-started.md)
* [Routing](routing.md)
* [Templates](templates.md)
* [Authentication](authentication.md)



