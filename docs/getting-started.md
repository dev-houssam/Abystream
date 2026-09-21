# Getting Started

This guide explains how to build and run an Abystream application.

Abystream applications are written in MATL-ABC and use the Abystream
framework modules for HTTP, routing, templates, WebSockets,
authentication and other web application services.

---

## 1. Prerequisites

An Abystream project requires:

- MATL-ABC
- the MATL-ABC compiler
- the Abystream framework
- a native build environment
- Docker, for containerized deployment

The project may also use a database and other external services
depending on the application.

---

## 2. Project structure

An Abystream application can be organized as follows:

```text
my_application/
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
│
├── build/
│
├── main.mabc
├── conf.toml
├── conf.docker.toml
├── Dockerfile
└── docker-compose.yml
````

The exact structure can vary depending on the application.

---

## 3. The application entry point

The main application entry point is:

```text
main.mabc
```

This file is responsible for starting the application and connecting
the application code with the Abystream framework.

Application-specific code can then be separated into:

```text
app/
├── Controllers/
├── Models/
├── Routes/
├── Views/
├── Config/
└── Components/
```

This keeps the application's business logic separate from the
framework itself.

---

## 4. Framework modules

Abystream is modular.

A project can use the framework modules required by the application.

For example:

```text
framework/
├── core/
├── http/
├── routing/
├── middleware/
├── server/
└── template/
```

Additional functionality is provided by other modules:

```text
framework/
├── auth/
├── database/
├── websocket/
├── plugin/
├── config/
└── io/
```

The framework modules are written in MATL-ABC and are described by
their corresponding `conf.toml` files.

---

## 5. Application configuration

The project contains a main configuration file:

```text
conf.toml
```

A separate configuration can be used for Docker:

```text
conf.docker.toml
```

This allows application configuration to be separated from
container-specific configuration.

Framework modules can also contain their own configuration:

```text
framework/
├── http/
│   └── conf.toml
├── routing/
│   └── conf.toml
├── server/
│   └── conf.toml
└── template/
    └── conf.toml
```

---

## 6. Creating an application structure

An application can be divided into several responsibilities.

### Routes

HTTP routes are stored under:

```text
app/Routes/
```

For example:

```text
app/
└── Routes/
    └── web.mabc
```

Routes define how incoming HTTP requests are connected to application
handlers.

See [Routing](routing.md).

---

### Controllers

Controllers contain application request-handling logic:

```text
app/Controllers/
```

A controller can receive a request and produce a response.

---

### Models

Models contain application data and domain logic:

```text
app/Models/
```

Models can be used by controllers when application data needs to be
read or modified.

---

### Views

HTML views are stored under:

```text
app/Views/
```

For example:

```text
Views/
├── layout.html
├── home.html
└── login.html
```

Views are rendered on the server using the Abystream template engine.

See [Templates](templates.md).

---

## 7. A minimal application

A simple application can be organized around the following structure:

```text
hello_abystream/
│
├── app/
│   ├── Routes/
│   │   └── web.mabc
│   └── Views/
│       └── home.html
│
├── framework/
│
├── native/
│
├── main.mabc
└── conf.toml
```

The route layer connects the incoming request to the application
handler.

Conceptually:

```text
Browser
   │
   │ GET /
   ▼
Abystream Server
   │
   ▼
Router
   │
   ▼
Application Handler
   │
   ▼
Template
   │
   ▼
HTML Response
   │
   ▼
Browser
```

---

## 8. Server-side rendering

A typical Abystream application can render HTML on the server.

The rendering flow is:

```text
Request
   │
   ▼
Route
   │
   ▼
Controller / Handler
   │
   ▼
Template
   │
   ▼
Rendered HTML
   │
   ▼
Response
```

The template engine supports:

* variables
* filters
* conditions
* loops
* partials
* HTML escaping

For example:

```html
<h1>{{ title }}</h1>
```

can receive a value from the application before the response is sent
to the browser.

See [Templates](templates.md).

---

## 9. WebSocket applications

WebSocket support is integrated into Abystream.

A WebSocket application follows this general flow:

```text
Client
   │
   │ HTTP Upgrade
   ▼
Abystream
   │
   ▼
WebSocket Handshake
   │
   ▼
WebSocket Connection
   │
   ▼
Application Handler
   │
   ▼
Messages
```

The application does not need to implement WebSocket frame encoding
and decoding itself.

See [WebSockets](websockets.md).

---

## 10. Authentication

Applications requiring authentication can use the Abystream
authentication module.

The current authentication implementation provides JWT utilities
using HS256.

```text
framework/
└── auth/
    └── auth.mabc
```

The main operations are:

```text
jwt_encode()
jwt_verify()
jwt_payload_b64()
jwt_b64url_decode()
```

See [Authentication](authentication.md).

---

## 11. Compilation

Abystream applications are compiled using the MATL-ABC compiler.

The resulting application is a native executable.

The general compilation process is:

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
build/
```

A project can therefore contain:

```text
build/
└── my_application
```

The exact compiler invocation depends on the MATL-ABC compiler version
and project configuration.

---

## 12. Checking a project

The MATL-ABC toolchain provides a project diagnostic command:

```bash
matlabc project doctor
```

This can be used to verify project consistency before building or
deploying the application.

A typical workflow is:

```text
Project
   │
   ▼
matlabc project doctor
   │
   ▼
Build
   │
   ▼
Run
```

---

## 13. Docker deployment

An Abystream application can be packaged for Docker deployment.

A project can contain:

```text
Dockerfile
docker-compose.yml
conf.docker.toml
```

The Docker image can contain the compiled application together with
the runtime components required to execute it.

A typical Docker workflow is:

```bash
docker compose up --build
```

This builds the application image and starts the services defined by
the project's `docker-compose.yml`.

---

## 14. Deployment structure

A deployment-oriented project can look like:

```text
my_application/
│
├── app/
├── framework/
├── native/
├── database/
│
├── build/
│   └── my_application
│
├── main.mabc
├── conf.toml
├── conf.docker.toml
├── Dockerfile
└── docker-compose.yml
```

The source code and framework modules are kept alongside the resources
required to produce and run the application.

---

## 15. Running the application

Once the project has been built, the resulting executable is located
in the project's build directory.

For example:

```text
build/
└── my_application
```

The application can then be started using the project's runtime
configuration.

For containerized execution, Docker Compose can be used:

```bash
docker compose up --build
```

When the application is running, the HTTP server listens on the port
configured by the application.

---

## 16. Development workflow

A typical development workflow is:

```text
        Write MATL-ABC code
                 │
                 ▼
        Configure application
                 │
                 ▼
     matlabc project doctor
                 │
                 ▼
              Build
                 │
                 ▼
        Run the application
                 │
                 ▼
       Test HTTP / WebSocket
                 │
                 ▼
          Modify the code
                 │
                 └──────────────┐
                                │
                                ▼
                         Repeat the cycle
```

For Docker-based development:

```text
        Modify source
             │
             ▼
     docker compose up
          --build
             │
             ▼
      Running container
             │
             ▼
        Test application
```

---

## 17. Example application

The `emploi_du_temps` application demonstrates a complete Abystream
project structure.

Its application layer contains:

```text
app/
├── app.mabc
├── Config/
│   └── app.mabc
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

The project also contains:

```text
database/
└── edt.db
```

and deployment resources:

```text
Dockerfile
docker-compose.yml
conf.toml
conf.docker.toml
```

The compiled application is stored in:

```text
build/
└── edt_app
```

This project demonstrates the separation between application code,
framework code, database resources, compilation artifacts and
deployment configuration.

---

## 18. Next steps

Once the application structure is understood, the following
documentation covers each major part of Abystream:

### Routing

Learn how HTTP and WebSocket routes are declared and resolved.

→ [Routing](routing.md)

### Templates

Learn how server-side HTML templates are rendered.

→ [Templates](templates.md)

### WebSockets

Learn how to build real-time applications.

→ [WebSockets](websockets.md)

### Authentication

Learn how JWT authentication is implemented.

→ [Authentication](authentication.md)

### Architecture

For a detailed description of the framework internals:

→ [Architecture](architecture.md)


---


```
```
