# Templates

Abystream provides a server-side template system for generating HTML
responses from application data.

The template system is provided by the `framework/template` module and
works together with the HTML and HTTP modules.

---

## 1. Overview

The template layer transforms application data into HTML.

```text
HTTP Request
     │
     ▼
   Router
     │
     ▼
 Controller / Handler
     │
     ▼
 Application Data
     │
     ▼
  Template
     │
     ▼
 Rendered HTML
     │
     ▼
HTTP Response
````

Templates allow presentation logic to remain separate from the
application's request-handling and data-processing logic.

---

## 2. Template module

The template implementation is located in:

```text
framework/
└── template/
    ├── template.mabc
    └── conf.toml
```

The template module is part of the Abystream framework.

It works with the HTML module:

```text
framework/
└── html/
    ├── html.mabc
    └── conf.toml
```

---

## 3. Application views

Application templates are stored under:

```text
app/
└── Views/
```

For example:

```text
app/
└── Views/
    ├── layout.html
    ├── login.html
    └── edition.html
```

The application is free to organize its views according to its own
requirements.

---

## 4. Server-side rendering

Abystream renders templates on the server.

The general process is:

```text
Application Data
       │
       ▼
   Template
       │
       ▼
 Template Engine
       │
       ▼
    HTML
       │
       ▼
 HTTP Response
```

The browser receives the resulting HTML rather than the template
source itself.

---

## 5. Template variables

Templates can receive values from the application.

A variable can be referenced using the template syntax:

```html
<h1>{{ title }}</h1>
```

If the application provides:

```text
title = "My application"
```

the rendered HTML contains the corresponding value.

The template therefore acts as the presentation layer between
application data and the final HTML document.

---

## 6. Template expressions

Template expressions can be used to insert dynamic values into HTML.

For example:

```html
<p>{{ username }}</p>
```

can render a value supplied by the application.

This allows the same template to be reused with different data.

---

## 7. Conditions

Templates can contain conditional rendering.

Conceptually:

```html
{% if authenticated %}
    <p>Welcome</p>
{% endif %}
```

The condition determines whether the associated HTML is rendered.

This is useful for interfaces where different elements must be
displayed depending on application state.

For example:

```text
Authenticated
     │
     ▼
Dashboard
```

or:

```text
Not authenticated
     │
     ▼
Login page
```

---

## 8. Loops

Templates can iterate over application data.

Conceptually:

```html
<ul>
{% for item in items %}
    <li>{{ item }}</li>
{% endfor %}
</ul>
```

This allows collections returned by the application to be represented
as HTML elements.

For example:

```text
Application
    │
    ▼
List of items
    │
    ▼
Template loop
    │
    ▼
HTML list
```

---

## 9. Filters

Template values can be transformed through filters.

Conceptually:

```html
{{ value | filter }}
```

Filters can be used when a value needs to be formatted before being
inserted into the generated HTML.

The available filters depend on the template implementation and
configuration.

---

## 10. HTML escaping

Values inserted into HTML need to be handled safely.

Template rendering can distinguish between application data and HTML
markup so that dynamic values are not automatically interpreted as
HTML.

Conceptually:

```text
Application value
       │
       ▼
Template variable
       │
       ▼
HTML escaping
       │
       ▼
Rendered HTML
```

This is particularly important when a value originates from an HTTP
request or another external source.

---

## 11. Layouts

A layout can provide the common structure shared by multiple pages.

For example:

```text
app/
└── Views/
    ├── layout.html
    ├── login.html
    └── edition.html
```

The layout can contain common elements such as:

* document structure
* metadata
* navigation
* stylesheets
* scripts
* shared page elements

Individual pages can then provide their own content.

Conceptually:

```text
             layout.html
                  │
        ┌─────────┴─────────┐
        ▼                   ▼
    login.html         edition.html
        │                   │
        └─────────┬─────────┘
                  ▼
             Final HTML
```

---

## 12. Partials

Reusable pieces of HTML can be separated into partial templates.

For example:

```text
app/
└── Views/
    ├── layout.html
    ├── partials/
    │   ├── navigation.html
    │   └── footer.html
    ├── login.html
    └── edition.html
```

A partial can be reused by several pages instead of duplicating the
same markup.

---

## 13. Template rendering from a controller

A controller can prepare the data required by a view.

The conceptual flow is:

```text
Request
   │
   ▼
Controller
   │
   ├── Read request
   ├── Execute application logic
   └── Prepare view data
            │
            ▼
         Template
            │
            ▼
        HTML Response
```

For example, an application could prepare:

```text
title
username
calendar
```

and provide those values to the template.

---

## 14. Models and templates

Templates should primarily be responsible for presentation.

Application data can be prepared by models and controllers before it
reaches the template.

A typical structure is:

```text
Request
   │
   ▼
Controller
   │
   ▼
Model
   │
   ▼
Data
   │
   ▼
Controller
   │
   ▼
Template
   │
   ▼
HTML
```

This prevents database and application-domain operations from being
embedded directly into HTML templates.

---

## 15. HTML module

The template engine works with the Abystream HTML module:

```text
framework/
└── html/
    ├── html.mabc
    └── conf.toml
```

The HTML module provides functionality used by the framework when
working with HTML content.

The separation between HTML handling and template processing allows
the template layer to focus on dynamic rendering.

---

## 16. HTTP response

Once a template has been rendered, the generated HTML becomes part of
an HTTP response.

```text
Template
   │
   ▼
Rendered HTML
   │
   ▼
HTTP Response
   │
   ├── Status
   ├── Headers
   └── Body
        │
        ▼
      Browser
```

The HTTP module is responsible for the HTTP layer, while the template
module produces the dynamic HTML content.

---

## 17. Complete rendering flow

A complete server-side rendering operation can be represented as:

```text
                        Client
                           │
                           │ HTTP Request
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
                    │  Controller  │
                    └──────┬───────┘
                           │
                    ┌──────┴───────┐
                    ▼              ▼
                  Model         View Data
                    │              │
                    └──────┬───────┘
                           ▼
                    ┌──────────────┐
                    │   Template   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ Rendered HTML│
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ HTTP Response│
                    └──────┬───────┘
                           │
                           ▼
                         Client
```

---

## 18. Example view structure

The `emploi_du_temps` application contains:

```text
app/
└── Views/
    ├── edition.html
    ├── layout.html
    └── login.html
```

The views represent different parts of the application's user
interface.

`layout.html` can provide shared page structure, while
`login.html` and `edition.html` provide page-specific content.

---

## 19. Templates and authentication

Templates can be combined with authentication state to produce
different interfaces.

For example:

```text
Authentication
      │
      ▼
Application state
      │
      ▼
Template data
      │
      ▼
Conditional rendering
```

An authenticated user can therefore receive a different interface
from an unauthenticated user.

The JWT implementation is provided by:

```text
framework/
└── auth/
    └── auth.mabc
```

See [Authentication](authentication.md).

---

## 20. Templates and WebSockets

Templates and WebSockets serve different purposes.

Templates generate the initial HTML response:

```text
Request
   │
   ▼
Template
   │
   ▼
HTML
```

WebSockets provide persistent communication after a connection has
been established:

```text
WebSocket Connection
        │
        ▼
     Messages
        │
        ▼
    Application
```

They can therefore be used together in the same application.

For example:

```text
Initial page
     │
     ▼
Server-side template
     │
     ▼
Browser
     │
     ▼
WebSocket connection
     │
     ▼
Real-time updates
```

See [WebSockets](websockets.md).

---

## 21. Template configuration

The template module contains its own configuration:

```text
framework/
└── template/
    └── conf.toml
```

Application-level configuration is maintained separately:

```text
conf.toml
```

This keeps framework configuration distinct from application
configuration.

---

## 22. Development workflow

A typical template development cycle is:

```text
Modify template
      │
      ▼
Run application
      │
      ▼
Send HTTP request
      │
      ▼
Render template
      │
      ▼
Inspect generated HTML
      │
      ▼
Modify template
      │
      └───────────────► Repeat
```

When the application is containerized, the same process can be
performed through Docker Compose.

---

## 23. Recommended separation

A server-rendered Abystream application can maintain the following
separation:

```text
app/
│
├── Routes/
│      └── Request → Handler
│
├── Controllers/
│      └── Request processing
│
├── Models/
│      └── Application data
│
└── Views/
       └── HTML presentation
```

The framework provides the infrastructure connecting these
components:

```text
framework/
├── server/
├── http/
├── routing/
├── middleware/
├── template/
└── html/
```

This separation keeps the framework infrastructure independent from
the application's presentation and business logic.

---

## 24. Related documentation

* [Architecture](architecture.md)
* [Getting Started](getting-started.md)
* [Routing](routing.md)
* [WebSockets](websockets.md)
* [Authentication](authentication.md)




