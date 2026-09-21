# Abystream
Un écosystème logiciel cohérent autour de MATL-ABC

---
- EN :


# Abystream : Full-stack web framework for MATL-ABC.

Abystream is a full-stack web framework designed for the MATL-ABC
programming language.

It provides the foundations required to build web applications with
MATL-ABC, including HTTP routing, server-side rendering, WebSockets,
authentication and application structure.

## Features

- HTTP server and routing
- Server-side rendering
- Template engine
- WebSocket support
- JWT authentication
- Middleware
- MVC application architecture
- Modular services
- Environment-based configuration
- Database integration
- Deployment-oriented runtime

## Architecture

```text
Application
│
├── Routes
│
├── Controllers
│
├── Models
│
├── Views
│
└── Services
       │
       ▼
   Abystream
       │
       ▼
   MATL-ABC Runtime
       │
       ▼
      LLVM
````

## Example

```matl
// Example application

route("/hello", GET, hello)

func hello(request: Request) -> Response {
    return Response.text("Hello, Abystream!")
}
```

## Why Abystream?

Abystream is developed alongside MATL-ABC.

Instead of treating the web framework as an external library,
the project explores what a web development ecosystem can look like
when the programming language, runtime and framework are designed
together.

```text
MATL-ABC
   │
   ▼
Compiler / LLVM
   │
   ▼
Runtime
   │
   ▼
Abystream
   │
   ▼
Web Applications
```

## Project status

Abystream is an experimental and evolving project.

The API, runtime and framework architecture may change as MATL-ABC
continues to evolve.

## Documentation

Documentation is currently being developed.

* [Architecture](docs/architecture.md)
* [Getting Started](docs/getting-started.md)
* [Routing](docs/routing.md)
* [Templates](docs/templates.md)
* [WebSockets](docs/websockets.md)
* [Authentication](docs/authentication.md)

## Related projects

* **MATL-ABC** — programming language and compiler
* **Abystream Blog** — reference application built with Abystream

## License

Abystream is released under the MIT License.

See [LICENSE](LICENSE) for the full license text.

## Author

**Houssam BACAR**

* Medium: [https://medium.com/@houssambacar](https://medium.com/@houssambacar)
* GitHub: [https://github.com/dev-houssam](https://github.com/dev-houssam)
* LinkedIn: [https://linkedin.com/in/houssambacar/](https://linkedin.com/in/houssambacar/)


