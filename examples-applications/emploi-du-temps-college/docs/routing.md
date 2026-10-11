# Routing

Abystream provides a routing system for connecting incoming HTTP
requests to application handlers.

The routing system is provided by the `framework/routing` module and
works together with the HTTP and server modules.

---

## 1. Overview

The routing layer sits between the HTTP server and the application.

```text
HTTP Request
     │
     ▼
HTTP Server
     │
     ▼
HTTP Request Parser
     │
     ▼
Router
     │
     ▼
Matched Route
     │
     ▼
Application Handler
     │
     ▼
HTTP Response
````

The router determines which application handler must process an
incoming request.

---

## 2. Routing module

The routing implementation is located in:

```text
framework/
└── routing/
    ├── routing.mabc
    └── conf.toml
```

The routing module is part of the core Abystream framework.

It works with:

```text
framework/
├── core/
├── http/
├── middleware/
└── server/
```

---

## 3. Application routes

Application-specific routes are normally placed under:

```text
app/
└── Routes/
```

For example:

```text
app/
└── Routes/
    └── web.mabc
```

The route file defines the HTTP entry points of the application.

This keeps application routing separate from the framework routing
implementation.

---

## 4. Route resolution

When a request reaches the application, Abystream resolves the
request against the registered routes.

Conceptually:

```text
Request
  │
  ├── Method
  ├── Path
  └── Headers
       │
       ▼
     Router
       │
       ▼
  Route matching
       │
       ├── Match
       │
       └── No match
             │
             ▼
        HTTP response
```

A matching route is associated with an application handler.

---

## 5. HTTP methods

Routes can be associated with HTTP methods.

The main HTTP methods used by web applications include:

```text
GET
POST
PUT
PATCH
DELETE
OPTIONS
HEAD
```

The router uses the request method together with the request path when
resolving a route.

For example, two routes can use the same path while responding to
different methods:

```text
GET  /users
POST /users
```

The method is therefore part of the route matching process.

---

## 6. Paths

A route contains a path identifying the requested resource.

Examples:

```text
/
```

```text
/users
```

```text
/login
```

```text
/calendar
```

The router compares the incoming request path with the registered
routes.

Conceptually:

```text
GET /login
     │
     ▼
"/login"
     │
     ▼
Login handler
```

---

## 7. Route handlers

A route ultimately connects a request to application code.

Conceptually:

```text
Route
  │
  ├── Method
  ├── Path
  └── Handler
          │
          ▼
      Application
```

The handler can then:

* inspect the request
* execute application logic
* access models or services
* render a template
* construct an HTTP response

---

## 8. Controllers and routing

Routing does not require controllers, but controllers provide a
convenient way to organize larger applications.

For example:

```text
app/
├── Controllers/
│   └── EdtControllers.mabc
│
└── Routes/
    └── web.mabc
```

The route layer can connect an HTTP endpoint to controller logic.

The resulting structure can be represented as:

```text
HTTP Request
     │
     ▼
    Route
     │
     ▼
 Controller
     │
     ▼
   Model
     │
     ▼
 Controller
     │
     ▼
   View
     │
     ▼
HTTP Response
```

This separates request routing from application logic.

---

## 9. Route parameters

A route can represent a resource identified by a parameter.

Conceptually, a route such as:

```text
/users/{id}
```

can match requests such as:

```text
/users/1
/users/42
/users/128
```

The parameter can then be provided to the application handler.

This allows applications to expose resources without declaring a
separate route for every possible identifier.

---

## 10. Query parameters

Query parameters are part of the request URL but are distinct from
the route path.

For example:

```text
/search?q=abystream
```

The route path is:

```text
/search
```

and the query string contains:

```text
q=abystream
```

The application can use query parameters for filtering, searching,
pagination and other request options.

---

## 11. Static routes

A static route directly identifies a fixed application endpoint.

Examples:

```text
/
/login
/about
/contact
```

The router can resolve these paths directly to their associated
handlers.

A simple application may therefore contain routes such as:

```text
GET /
GET /login
GET /contact
```

---

## 12. Dynamic routes

Dynamic routes are useful when the path contains values determined by
the client request.

For example:

```text
/users/{id}
```

allows the application to process multiple user identifiers through
one route.

The conceptual flow is:

```text
GET /users/42
       │
       ▼
/users/{id}
       │
       ▼
id = 42
       │
       ▼
Application handler
```

---

## 13. Route organization

As an application grows, routes can be grouped according to their
domain.

For example:

```text
app/
└── Routes/
    ├── web.mabc
    ├── api.mabc
    └── auth.mabc
```

The exact organization is application-dependent.

A smaller project can keep all routes in a single file:

```text
app/
└── Routes/
    └── web.mabc
```

---

## 14. Middleware and routing

Middleware can be executed around request processing.

The relationship can be represented as:

```text
HTTP Request
     │
     ▼
 Middleware
     │
     ▼
   Router
     │
     ▼
  Handler
     │
     ▼
 Middleware
     │
     ▼
HTTP Response
```

Middleware can therefore be used for cross-cutting request processing
such as:

* authentication
* request validation
* logging
* request transformation
* response processing

The middleware implementation is provided by:

```text
framework/
└── middleware/
    ├── middleware.mabc
    └── conf.toml
```

---

## 15. Authentication-aware routes

An application can combine routing with authentication.

For example:

```text
GET /login
GET /dashboard
```

A public route can be accessible without authentication:

```text
/login
```

while an application can protect another route through middleware or
application-level authentication:

```text
/dashboard
```

The authentication module provides JWT operations that can be used by
the application.

See [Authentication](authentication.md).

---

## 16. Route and template processing

A common server-rendered route can follow this structure:

```text
GET /calendar
      │
      ▼
    Router
      │
      ▼
 Controller
      │
      ▼
    Model
      │
      ▼
   Template
      │
      ▼
 Rendered HTML
      │
      ▼
HTTP Response
```

The route itself is responsible for connecting the request to the
application logic.

The template engine is responsible for producing the final HTML.

See [Templates](templates.md).

---

## 17. HTTP errors

When a request does not correspond to a registered route, the
application must produce an appropriate HTTP response.

Conceptually:

```text
Request
  │
  ▼
Router
  │
  ├── Route found
  │      │
  │      ▼
  │   Handler
  │
  └── Route not found
         │
         ▼
     404 Response
```

A route handler can also produce other HTTP status codes according to
the application's logic.

Examples include:

```text
200 OK
201 Created
204 No Content
400 Bad Request
401 Unauthorized
403 Forbidden
404 Not Found
500 Internal Server Error
```

---

## 18. WebSocket routes

WebSocket endpoints use the routing and server infrastructure but
follow a different protocol after the initial HTTP upgrade.

The initial request is an HTTP request containing a WebSocket upgrade:

```text
HTTP Request
     │
     ▼
Router / Server
     │
     ▼
WebSocket Upgrade
     │
     ▼
Handshake
     │
     ▼
WebSocket Connection
```

Once the handshake has completed, communication uses WebSocket frames
instead of ordinary HTTP request/response exchanges.

The WebSocket implementation is provided by:

```text
framework/
└── websocket/
    ├── websocket.mabc
    └── conf.toml
```

See [WebSockets](websockets.md).

---

## 19. Routing and the HTTP module

Routing relies on the HTTP layer to obtain the information required
for route resolution.

The relationship between the modules is:

```text
Application
     │
     ▼
 Routing
     │
     ▼
   HTTP
     │
     ▼
   Server
```

The HTTP module handles HTTP-specific structures and processing,
while the routing module determines which application handler should
receive the request.

---

## 20. Routing and the server

The server module is responsible for accepting network connections
and receiving requests.

The router operates at a higher level:

```text
Network
   │
   ▼
Server
   │
   ▼
HTTP
   │
   ▼
Router
   │
   ▼
Application
```

This separation allows the networking layer and application routing
layer to remain independent.

---

## 21. Example application structure

The `emploi_du_temps` project uses:

```text
app/
├── Controllers/
│   └── EdtControllers.mabc
├── Models/
│   ├── EdtModel.mabc
│   └── EdtModels.mabc
├── Routes/
│   └── web.mabc
└── Views/
    ├── edition.html
    ├── layout.html
    └── login.html
```

The routing entry point is:

```text
app/Routes/web.mabc
```

The controller implementation is:

```text
app/Controllers/EdtControllers.mabc
```

The models are located under:

```text
app/Models/
```

and the HTML views under:

```text
app/Views/
```

This provides a clear separation between route declaration,
application processing, data and presentation.

---

## 22. Complete request flow

A complete server-rendered request can therefore be represented as:

```text
                   Client
                     │
                     │ HTTP request
                     ▼
              ┌──────────────┐
              │    Server    │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │     HTTP     │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │    Router    │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │   Handler    │
              └──────┬───────┘
                     │
              ┌──────┴───────┐
              ▼              ▼
           Model          Template
              │              │
              └──────┬───────┘
                     ▼
              HTTP Response
                     │
                     ▼
                   Client
```

This is the principal path followed by a typical Abystream HTTP
request.

---

## 23. Related documentation

* [Architecture](architecture.md)
* [Getting Started](getting-started.md)
* [Templates](templates.md)
* [WebSockets](websockets.md)
* [Authentication](authentication.md)





