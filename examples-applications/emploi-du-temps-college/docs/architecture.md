# Abystream Architecture

Abystream is a full-stack web framework built on top of the MATL-ABC
programming language.

The framework is designed as a set of modules written in MATL-ABC,
combined with native components where low-level system or protocol
operations are required.

The main goal is to provide a complete environment for building web
applications with MATL-ABC, from application code to deployment.

---

## 1. Overview

An Abystream application is built on several layers:

```text
┌──────────────────────────────────────────────┐
│                  Application                 │
│                                              │
│  Controllers · Models · Views · Routes       │
│  Components · Configuration · Business Logic │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│                  ABYSTREAM                   │
│                                              │
│  HTTP · Routing · Middleware · Templates     │
│  WebSocket · Authentication · Database      │
│  Configuration · I/O · Plugins · Core        │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│              Native Components               │
│                                              │
│       webshim · posixshim · system APIs     │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│               MATL-ABC Runtime               │
│                                              │
│             Native compiled code             │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│                    OS                        │
└──────────────────────────────────────────────┘
````

MATL-ABC is therefore the language in which both the application and
the framework modules can be implemented, while native shims provide
operations that require direct access to lower-level functionality.

---

## 2. MATL-ABC and Abystream

Abystream is designed specifically for MATL-ABC.

The framework is not a separate runtime that replaces the language.
Instead, it is built as a collection of MATL-ABC modules that operate
within the MATL-ABC execution environment.

The relationship can be represented as:

```text
MATL-ABC Language
       │
       ▼
MATL-ABC Compiler
       │
       ▼
Native Application
       │
       ├─────────────────────┐
       │                     │
       ▼                     ▼
MATL-ABC Runtime        Native shims
       │                     │
       └──────────┬──────────┘
                  ▼
             Abystream
                  │
                  ▼
          Web Application
```

This approach allows the framework to use MATL-ABC features directly
while delegating some low-level operations to native code.

---

## 3. Framework structure

The current Abystream framework is organized into independent modules.

```text
framework/
├── auth/
├── calendar/
├── collections/
├── config/
├── core/
├── database/
├── html/
├── http/
├── io/
├── middleware/
├── plugin/
├── routing/
├── server/
├── template/
├── websocket/
└── framework.mabc
```

Each module generally contains its MATL-ABC implementation and a
`conf.toml` file describing its module configuration.

For example:

```text
framework/
└── websocket/
    ├── websocket.mabc
    └── conf.toml
```

The framework is therefore modular rather than being implemented as one
large source file.

---

## 4. Core framework modules

### `core`

Contains core functionality shared by other framework components.

```text
framework/core/
├── core.mabc
└── conf.toml
```

Other framework modules can depend on the core module.

---

### `http`

Provides HTTP-related functionality.

```text
framework/http/
├── http.mabc
└── conf.toml
```

The HTTP layer provides the structures and operations required by the
web server and routing system.

---

### `server`

Provides the server-side execution layer.

```text
framework/server/
├── server.mabc
└── conf.toml
```

The server is responsible for accepting network connections and
connecting them to the HTTP processing layer.

---

### `routing`

Provides HTTP and WebSocket routing.

```text
framework/routing/
├── routing.mabc
└── conf.toml
```

The routing layer associates incoming requests with application
handlers.

The framework also supports route groups, allowing routes to be
organized under common prefixes.

Conceptually:

```text
Application
    │
    ▼
Route Group
    │
    ├── Route
    ├── Route
    └── Route
```

Route resolution can use either direct path matching or regular
expression based matching.

---

### `middleware`

Provides middleware support for request processing.

```text
framework/middleware/
├── middleware.mabc
└── conf.toml
```

Middleware can participate in the processing pipeline before the final
application handler is executed.

The architecture can therefore be represented as:

```text
Request
   │
   ▼
Middleware
   │
   ▼
Middleware
   │
   ▼
Handler
   │
   ▼
Response
```

A middleware can stop the pipeline when appropriate instead of passing
the request to the next stage.

---

### `template`

Provides the server-side template engine.

```text
framework/template/
├── template.mabc
└── conf.toml
```

The template engine is responsible for turning application data and
HTML templates into rendered HTML.

Its functionality includes:

```text
Variables
Filters
Conditions
Loops
Partials
HTML escaping
```

The template system is documented separately in:

[Templates](templates.md)

---

### `html`

Provides HTML-related functionality used by the framework.

```text
framework/html/
├── html.mabc
└── conf.toml
```

It is separated from the template module so that HTML-related
operations can be provided independently from template processing.

---

### `websocket`

Provides server-side WebSocket support.

```text
framework/websocket/
├── websocket.mabc
└── conf.toml
```

The WebSocket module handles the protocol-level operations required
between the HTTP upgrade and application-level message handling.

The processing flow is:

```text
HTTP Request
     │
     ▼
WebSocket Upgrade
     │
     ▼
Handshake
     │
     ▼
WebSocket Frames
     │
     ▼
Message
     │
     ▼
Application Handler
     │
     ▼
Response Message
     │
     ▼
WebSocket Frame
```

The WebSocket implementation performs the handshake and frame
encoding/decoding while exposing messages to the application.

See:

[WebSockets](websockets.md)

---

### `auth`

Provides authentication utilities.

```text
framework/auth/
├── auth.mabc
└── conf.toml
```

The current authentication implementation provides JWT operations
using HS256.

Its main operations include:

```text
jwt_encode
jwt_verify
jwt_payload_b64
jwt_b64url_decode
```

Cryptographic operations such as HMAC-SHA256 and Base64URL processing
are delegated to the native web shim.

See:

[Authentication](authentication.md)

---

### `database`

Provides the database abstraction used by applications.

```text
framework/database/
├── database.mabc
└── conf.toml
```

The application can therefore keep database-related functionality
separate from controllers and views.

---

### `config`

Provides configuration functionality.

```text
framework/config/
├── config.mabc
└── conf.toml
```

Configuration files are also used throughout the framework modules.

---

### `io`

Provides input/output related functionality.

```text
framework/io/
├── io.mabc
└── conf.toml
```

---

### `plugin`

Provides the framework's plugin-related functionality.

```text
framework/plugin/
├── plugin.mabc
└── conf.toml
```

---

### `collections`

Contains reusable data structures.

```text
framework/collections/
├── linked_list.mabc
├── stack_queue.mabc
└── tree.mabc
```

These components are available independently from the web-specific
parts of the framework.

---

### `calendar`

Provides calendar-related framework functionality.

```text
framework/calendar/
├── calendar.mabc
└── conf.toml
```

This module is part of the current framework source tree but is not a
core HTTP component.

---

## 5. Application architecture

An application using Abystream can be organized using an MVC-oriented
structure.

A typical application has:

```text
app/
├── Controllers/
├── Models/
├── Views/
├── Routes/
├── Config/
└── Components/
```

### Controllers

Controllers contain application logic responsible for handling
requests.

```text
app/Controllers/
```

For example:

```text
Controllers/
└── EdtControllers.mabc
```

---

### Models

Models contain application data and domain-related operations.

```text
app/Models/
├── EdtModel.mabc
└── EdtModels.mabc
```

---

### Views

Views contain the HTML presented to the client.

```text
app/Views/
├── edition.html
├── layout.html
└── login.html
```

Views are rendered using the Abystream template system.

---

### Routes

Routes define the application's HTTP interface.

```text
app/Routes/
└── web.mabc
```

Keeping routes in a dedicated directory separates the application's
HTTP interface from its controllers and models.

---

### Configuration

Application-specific configuration can be kept in:

```text
app/Config/
```

The framework itself also uses TOML configuration files.

---

### Components

The application can contain additional reusable components under:

```text
app/Components/
```

This directory is available for application-specific components that
do not belong directly to controllers, models or views.

---

## 6. MVC architecture

The application-level architecture can be represented as:

```text
                  HTTP Request
                       │
                       ▼
                    Routes
                       │
                       ▼
                 Controllers
                  │        │
                  │        │
                  ▼        ▼
               Models     Views
                  │        │
                  │        │
                  └────┬───┘
                       ▼
                  HTTP Response
```

The controller acts as the connection between the incoming request,
application logic and rendered response.

A model can provide or manipulate application data, while a view
represents that data as HTML.

---

## 7. Request processing

An HTTP request passes through several framework layers.

The general architecture is:

```text
Client
  │
  │ HTTP request
  ▼
Server
  │
  ▼
HTTP layer
  │
  ▼
Router
  │
  ▼
Middleware
  │
  ▼
Application handler
  │
  ├──────────────► Model / Database
  │
  ▼
Template / View
  │
  ▼
Response
  │
  ▼
HTTP layer
  │
  ▼
Client
```

The routing layer determines which application handler should receive
the request.

Middleware can process the request before the handler.

The handler can then interact with models, databases or other
application services before producing a response.

When server-side rendering is used, the resulting data can be passed
to the template engine to generate HTML.

---

## 8. WebSocket processing

WebSocket communication follows a different path after the initial
HTTP upgrade.

```text
Client
  │
  │ HTTP Upgrade
  ▼
HTTP / WebSocket layer
  │
  ▼
Handshake
  │
  ▼
WebSocket connection
  │
  ▼
Frame decoding
  │
  ▼
Application handler
  │
  ▼
Message response
  │
  ▼
Frame encoding
  │
  ▼
Client
```

The application handler works with messages instead of directly
manipulating WebSocket frames.

This separates application-level communication from protocol-level
framing.

---

## 9. Native layer

Some operations cannot be implemented conveniently at the framework
level and are delegated to native code.

The current application structure contains:

```text
native/
├── headers/
│   ├── extra_file.mabh
│   ├── posixshim.mabh
│   └── webshim.mabh
│
├── libposixshim.so
├── libwebshim.so
└── webshim.c
```

Two native libraries are currently present:

```text
libposixshim.so
libwebshim.so
```

### POSIX shim

The POSIX shim provides native functionality required by the
application/runtime environment.

### Web shim

The web shim provides low-level operations used by the web framework.

For example, the authentication module uses native operations for:

```text
HMAC-SHA256
Base64URL encoding
Base64URL decoding
```

The WebSocket module also relies on native functionality for operations
such as:

```text
WebSocket accept key generation
WebSocket frame decoding
WebSocket frame encoding
```

This creates a separation between framework-level MATL-ABC code and
low-level native operations.

---

## 10. Framework and native boundary

The relationship between MATL-ABC code and native code can therefore
be represented as:

```text
┌──────────────────────────────────┐
│          Application             │
│             MATL-ABC             │
└────────────────┬─────────────────┘
                 │
                 ▼
┌──────────────────────────────────┐
│           Abystream              │
│             MATL-ABC              │
│                                  │
│ HTTP · Routing · Templates       │
│ Middleware · Auth · WebSocket    │
└────────────────┬─────────────────┘
                 │
                 ▼
┌──────────────────────────────────┐
│          Native shims            │
│                                  │
│       webshim / posixshim        │
└────────────────┬─────────────────┘
                 │
                 ▼
┌──────────────────────────────────┐
│        Operating System          │
└──────────────────────────────────┘
```

The framework therefore does not need to reimplement every low-level
operation in MATL-ABC.

---

## 11. Configuration

Abystream uses TOML configuration files.

The framework modules contain their own configuration:

```text
framework/
├── auth/
│   └── conf.toml
├── database/
│   └── conf.toml
├── http/
│   └── conf.toml
├── routing/
│   └── conf.toml
├── server/
│   └── conf.toml
├── template/
│   └── conf.toml
└── websocket/
    └── conf.toml
```

An application can also contain its own configuration:

```text
conf.toml
conf.docker.toml
```

This allows application configuration and deployment configuration to
remain separate.

---

## 12. Project structure

A complete Abystream application can contain both application code,
framework sources and deployment resources.

For example:

```text
emploi_du_temps/
│
├── app/
│   ├── app.mabc
│   ├── Components/
│   ├── Config/
│   ├── Controllers/
│   ├── Models/
│   ├── Routes/
│   └── Views/
│
├── compiler/
│   └── bin/
│       └── matlabc
│
├── framework/
│   ├── auth/
│   ├── calendar/
│   ├── collections/
│   ├── config/
│   ├── core/
│   ├── database/
│   ├── html/
│   ├── http/
│   ├── io/
│   ├── middleware/
│   ├── plugin/
│   ├── routing/
│   ├── server/
│   ├── template/
│   └── websocket/
│
├── native/
│   ├── headers/
│   ├── libposixshim.so
│   ├── libwebshim.so
│   └── webshim.c
│
├── database/
│   └── edt.db
│
├── build/
│   └── edt_app
│
├── main.mabc
├── conf.toml
├── conf.docker.toml
├── Dockerfile
└── docker-compose.yml
```

The exact structure can vary between applications.

---

## 13. Compilation and deployment

An Abystream application is ultimately compiled into a native
executable.

The general flow is:

```text
MATL-ABC source
       │
       ▼
MATL-ABC compiler
       │
       ▼
Native executable
       │
       ▼
Deployment package
       │
       ▼
Docker / target environment
```

A project can contain both:

```text
build/
    application binary
```

and:

```text
Dockerfile
docker-compose.yml
```

This makes it possible to separate the compilation stage from the
runtime deployment environment.

---

## 14. Deployment architecture

A Docker-based deployment can be represented as:

```text
┌─────────────────────────────────────────┐
│              Docker Host                │
│                                         │
│  ┌───────────────────────────────────┐  │
│  │        Abystream Container         │  │
│  │                                   │  │
│  │   Native Application              │  │
│  │          │                        │  │
│  │          ▼                        │  │
│  │      Abystream                   │  │
│  │          │                        │  │
│  │          ▼                        │  │
│  │     HTTP / WebSocket              │  │
│  │                                   │  │
│  └───────────────────────────────────┘  │
│                                         │
└─────────────────────────────────────────┘
```

A `docker-compose.yml` file can be used to define the application's
container environment.

---

## 15. Application dependencies

The framework modules form a dependency structure rather than a flat
collection of unrelated files.

A simplified representation is:

```text
                    Application
                         │
             ┌───────────┼───────────┐
             ▼           ▼           ▼
          Routing     Template      Auth
             │           │           │
             ▼           ▼           ▼
           HTTP         HTML       Core
             │                       │
             └───────────┬───────────┘
                         ▼
                       Core
                         │
                         ▼
                   Native shims
```

The exact dependency graph is defined by the individual module
configuration files.

---

## 16. Separation of concerns

Abystream separates several responsibilities.

### Application layer

Contains application-specific behavior:

```text
Controllers
Models
Views
Routes
Components
```

### Framework layer

Contains reusable web and application infrastructure:

```text
HTTP
Routing
Middleware
Templates
WebSockets
Authentication
Database
Configuration
```

### Native layer

Contains low-level operations:

```text
webshim
posixshim
```

### Deployment layer

Contains the resources required to package and run the application:

```text
Dockerfile
docker-compose.yml
conf.docker.toml
build/
```

This separation can be summarized as:

```text
Application
     │
     ▼
Framework
     │
     ▼
Native layer
     │
     ▼
Operating system

Application
     │
     ▼
Deployment configuration
     │
     ▼
Docker / Runtime environment
```

---

## 17. Design principle

Abystream is built around the idea of providing a complete web
development environment around MATL-ABC.

Instead of treating the programming language, framework and deployment
environment as completely independent components, Abystream connects
them:

```text
        MATL-ABC
           │
           ▼
        Compiler
           │
           ▼
         Runtime
           │
           ▼
       Abystream
           │
     ┌─────┼─────┐
     ▼     ▼     ▼
    HTTP  HTML  WS
     │     │     │
     └─────┼─────┘
           ▼
       Application
           │
           ▼
       Deployment
```

The result is a single ecosystem in which application code, framework
modules, native operations and deployment resources can coexist within
the same project.

---

## Related documentation

* [Getting Started](getting-started.md)
* [Routing](routing.md)
* [Templates](templates.md)
* [WebSockets](websockets.md)
* [Authentication](authentication.md)

